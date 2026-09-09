import 'dart:async';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'package:webrtc_interface/webrtc_interface.dart';

import '../helper.dart';
import '../video_renderer_extension.dart' show AudioControl;
import 'rtc_video_dewarp.dart';
import 'utils.dart';

class RTCVideoRenderer extends ValueNotifier<RTCVideoValue>
    implements VideoRenderer, AudioControl {
  RTCVideoRenderer() : super(RTCVideoValue.empty);
  Completer? _initializing;
  int? _textureId;
  bool _disposed = false;
  MediaStream? _srcObject;
  StreamSubscription<dynamic>? _eventSubscription;

  @override
  Future<void> initialize() async {
    if (_initializing != null) {
      await _initializing!.future;
      return;
    }
    _initializing = Completer();
    final response = await WebRTC.invokeMethod('createVideoRenderer', {});
    _textureId = response['textureId'];
    _eventSubscription = EventChannel('FlutterWebRTC/Texture$textureId')
        .receiveBroadcastStream()
        .listen(eventListener, onError: errorListener);
    _initializing!.complete(null);
  }

  @override
  int get videoWidth => value.width.toInt();

  @override
  int get videoHeight => value.height.toInt();

  @override
  int? get textureId => _textureId;

  @override
  MediaStream? get srcObject => _srcObject;

  @override
  Function? onResize;

  @override
  Function? onFirstFrameRendered;

  @override
  set srcObject(MediaStream? stream) {
    if (_disposed) {
      throw 'Can\'t set srcObject: The RTCVideoRenderer is disposed';
    }
    if (textureId == null) throw 'Call initialize before setting the stream';
    _srcObject = stream;
    WebRTC.invokeMethod('videoRendererSetSrcObject', <String, dynamic>{
      'textureId': textureId,
      'streamId': stream?.id ?? '',
      'ownerTag': stream?.ownerTag ?? ''
    }).then((_) {
      value = (stream == null)
          ? RTCVideoValue.empty
          : value.copyWith(renderVideo: renderVideo);
    }).catchError((e) {
      print('Got exception for RTCVideoRenderer::setSrcObject: ${e.message}');
    }, test: (e) => e is PlatformException);
  }

  Future<void> setSrcObject({MediaStream? stream, String? trackId}) async {
    if (_disposed) {
      throw 'Can\'t set srcObject: The RTCVideoRenderer is disposed';
    }
    if (_textureId == null) throw 'Call initialize before setting the stream';
    _srcObject = stream;
    var oldTextureId = _textureId;
    try {
      await WebRTC.invokeMethod('videoRendererSetSrcObject', <String, dynamic>{
        'textureId': _textureId,
        'streamId': stream?.id ?? '',
        'ownerTag': stream?.ownerTag ?? '',
        'trackId': trackId ?? '0'
      });
      value = (stream == null)
          ? RTCVideoValue.empty
          : value.copyWith(renderVideo: renderVideo);
    } on PlatformException catch (e) {
      throw 'Got exception for RTCVideoRenderer::setSrcObject: textureId $oldTextureId [disposed: $_disposed] with stream ${stream?.id}, error: ${e.message}';
    }
  }

  /// Configure (or clear, by passing `null`) real-time fisheye lens
  /// dewarping for this renderer.
  ///
  /// This only swaps the local GPU rendering path (which shader/kernel
  /// draws the already-decoded frame) -- it never touches [srcObject], the
  /// underlying [MediaStreamTrack], or the peer connection. The video
  /// stream keeps playing uninterrupted; only the on-screen pixels change,
  /// from the raw frame to the dewarped composite (or back). Call this on
  /// the same renderer instance that already has the stream attached --
  /// don't create a second renderer/`RTCVideoView` for the dewarped view.
  ///
  /// Native-only (Android/iOS/macOS): a no-op everywhere else, since the
  /// dewarp is a GPU post-process applied to already-decoded frames on the
  /// native side.
  Future<void> setDewarpConfig(FisheyeDewarpConfig? config) async {
    if (_disposed) {
      throw 'Can\'t set dewarp config: The RTCVideoRenderer is disposed';
    }
    if (_textureId == null) {
      throw 'Call initialize before setting the dewarp config';
    }
    if (!(WebRTC.platformIsAndroid ||
        WebRTC.platformIsIOS ||
        WebRTC.platformIsMacOS)) {
      return;
    }
    await WebRTC.invokeMethod('videoRendererSetDewarpConfig', <String, dynamic>{
      'textureId': _textureId,
      'enabled': config != null,
      if (config != null) ...config.toMap(),
    });
  }

  /// Pans a single PTZ tile of the current [setDewarpConfig] config in
  /// place, for drag-to-scroll interactions.
  ///
  /// Unlike [setDewarpConfig], this does not tear down and recreate the
  /// renderer -- it just mutates that one tile's pan angle, so it's cheap
  /// enough to call on every frame of a drag gesture. Only meaningful after
  /// a config with at least `tileIndex + 1` PTZ tiles has already been
  /// applied via [setDewarpConfig]; calling this first, or with an
  /// out-of-range [tileIndex], is a no-op on the native side.
  ///
  /// Native-only (Android/iOS/macOS): a no-op everywhere else, same as
  /// [setDewarpConfig].
  Future<void> updatePtzTilePan(int tileIndex, double panDeg) async {
    if (_disposed) {
      throw 'Can\'t update PTZ tile pan: The RTCVideoRenderer is disposed';
    }
    if (_textureId == null) {
      throw 'Call initialize before updating a PTZ tile\'s pan';
    }
    if (!(WebRTC.platformIsAndroid ||
        WebRTC.platformIsIOS ||
        WebRTC.platformIsMacOS)) {
      return;
    }
    await WebRTC.invokeMethod('videoRendererUpdatePtzTilePan', <String, dynamic>{
      'textureId': _textureId,
      'tileIndex': tileIndex,
      'panDeg': panDeg,
    });
  }

  /// Same idea as [updatePtzTilePan], but for the base tile's own
  /// independent pan+tilt offset -- only meaningful for a
  /// [FisheyeDisplayMode] whose base tile is a direct rectangular crop of
  /// the raw circle (currently just
  /// [FisheyeDisplayMode.threeSixtyPlus1Ptz]), not a panorama unwrap or
  /// fixed full-arc flatten. Takes both axes in one call since a drag
  /// gesture naturally produces both at once.
  ///
  /// Native-only (Android/iOS/macOS): a no-op everywhere else, same as
  /// [setDewarpConfig].
  Future<void> updateBaseTileOffset(double panDeg, double tiltDeg) async {
    if (_disposed) {
      throw 'Can\'t update base tile offset: The RTCVideoRenderer is disposed';
    }
    if (_textureId == null) {
      throw 'Call initialize before updating the base tile\'s offset';
    }
    if (!(WebRTC.platformIsAndroid ||
        WebRTC.platformIsIOS ||
        WebRTC.platformIsMacOS)) {
      return;
    }
    await WebRTC.invokeMethod('videoRendererUpdateBaseTileOffset', <String, dynamic>{
      'textureId': _textureId,
      'panDeg': panDeg,
      'tiltDeg': tiltDeg,
    });
  }

  @override
  Future<void> dispose() async {
    if (_disposed) return;
    await _eventSubscription?.cancel();
    _eventSubscription = null;
    if (_textureId != null) {
      try {
        await WebRTC.invokeMethod('videoRendererDispose', <String, dynamic>{
          'textureId': _textureId,
        });
        _textureId = null;
        _disposed = true;
      } on PlatformException catch (e) {
        throw 'Failed to RTCVideoRenderer::dispose: ${e.message}';
      }
    }

    return super.dispose();
  }

  void eventListener(dynamic event) {
    if (_disposed) return;
    final Map<dynamic, dynamic> map = event;
    switch (map['event']) {
      case 'didTextureChangeRotation':
        value =
            value.copyWith(rotation: map['rotation'], renderVideo: renderVideo);
        onResize?.call();
        break;
      case 'didTextureChangeVideoSize':
        value = value.copyWith(
            width: 0.0 + map['width'],
            height: 0.0 + map['height'],
            renderVideo: renderVideo);
        onResize?.call();
        break;
      case 'didFirstFrameRendered':
        value = value.copyWith(renderVideo: renderVideo);
        onFirstFrameRendered?.call();
        break;
    }
  }

  void errorListener(Object obj) {
    if (obj is Exception) {
      throw obj;
    }
  }

  @override
  bool get renderVideo => _textureId != null && _srcObject != null;

  @override
  bool get muted => _srcObject?.getAudioTracks()[0].muted ?? true;

  @override
  set muted(bool mute) {
    if (_disposed) {
      throw Exception('Can\'t be muted: The RTCVideoRenderer is disposed');
    }
    if (_srcObject == null) {
      throw Exception('Can\'t be muted: The MediaStream is null');
    }
    if (_srcObject!.ownerTag != 'local') {
      throw Exception(
          'You\'re trying to mute a remote track, this is not supported');
    }
    if (_srcObject!.getAudioTracks().isEmpty) {
      throw Exception('Can\'t be muted: The MediaStreamTrack(audio) is empty');
    }

    Helper.setMicrophoneMute(mute, _srcObject!.getAudioTracks()[0]);
  }

  @override
  Future<bool> audioOutput(String deviceId) async {
    try {
      await Helper.selectAudioOutput(deviceId);
    } catch (e) {
      print('Helper.selectAudioOutput ${e.toString()}');
      return false;
    }
    return true;
  }

  @override
  Future<void> setVolume(double value) async {
    try {
      if (_srcObject == null) {
        throw Exception('Can\'t set volume: The MediaStream is null');
      }
      for (MediaStreamTrack track in _srcObject!.getAudioTracks()) {
        await Helper.setVolume(value, track);
      }
    } catch (e) {
      print('Helper.setVolume ${e.toString()}');
    }
  }
}
