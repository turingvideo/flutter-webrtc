import 'dart:async';

import 'event_channel.dart';
import 'rtc_video_dewarp.dart';
import 'utils.dart';

/// MEDIASDK_PTZ_MODE_E (fisheye/android/vendor/include/media_sdk_com_def.h)
/// -- covers every mode [FisheyeDisplayMode] maps to (see
/// mediaSdkPtzModeForDisplayMode). The vendor enum has 3 more values
/// (MEDIASDK_FISH_MID_OFF_4PTZ, MEDIASDK_FISH_LEFT_4PTZ, MEDIASDK_1PTZ,
/// MEDIASDK_NOPR_4PTZ) this project has no UI option for, so they're left
/// out here; [fisheyePlus4Ptz] uses MEDIASDK_FISH_MID_ON_4PTZ (fisheye
/// visible, centered) as this project's one 4-PTZ layout.
enum MediaSdkFisheyePtzMode {
  original(0),
  pano180(1),
  threeSixtyPlus1Ptz(2),
  threeSixtyPlus6Ptz(3),
  fisheyePlus3Ptz(4),
  fisheyePlus4Ptz(5),
  fisheyePlus8Ptz(8),
  panoramic(9),
  panoPlus3Ptz(10),
  panoPlus4Ptz(11),
  panoPlus8Ptz(12);

  const MediaSdkFisheyePtzMode(this.value);
  final int value;
}

/// MEDIASDK_FIX_MODE_E
enum MediaSdkFisheyeFixMode {
  topInstallation(0),
  bottomInstallation(1),
  sideInstallation(2);

  const MediaSdkFisheyeFixMode(this.value);
  final int value;
}

/// Maps a [FisheyeDisplayMode] (the GL-dewarp enum the mount/display type
/// panel is built on) to the [MediaSdkFisheyePtzMode] the vendor SDK needs
/// to render the same layout.
MediaSdkFisheyePtzMode mediaSdkPtzModeForDisplayMode(FisheyeDisplayMode mode) {
  switch (mode) {
    case FisheyeDisplayMode.fisheye:
      return MediaSdkFisheyePtzMode.original;
    case FisheyeDisplayMode.pano180:
      return MediaSdkFisheyePtzMode.pano180;
    case FisheyeDisplayMode.panoramic:
      return MediaSdkFisheyePtzMode.panoramic;
    case FisheyeDisplayMode.threeSixtyPlus1Ptz:
      return MediaSdkFisheyePtzMode.threeSixtyPlus1Ptz;
    case FisheyeDisplayMode.threeSixtyPlus6Ptz:
      return MediaSdkFisheyePtzMode.threeSixtyPlus6Ptz;
    case FisheyeDisplayMode.fisheyePlus3Ptz:
      return MediaSdkFisheyePtzMode.fisheyePlus3Ptz;
    case FisheyeDisplayMode.fisheyePlus4Ptz:
      return MediaSdkFisheyePtzMode.fisheyePlus4Ptz;
    case FisheyeDisplayMode.fisheyePlus8Ptz:
      return MediaSdkFisheyePtzMode.fisheyePlus8Ptz;
    case FisheyeDisplayMode.panoPlus3Ptz:
      return MediaSdkFisheyePtzMode.panoPlus3Ptz;
    case FisheyeDisplayMode.panoPlus4Ptz:
      return MediaSdkFisheyePtzMode.panoPlus4Ptz;
    case FisheyeDisplayMode.panoPlus8Ptz:
      return MediaSdkFisheyePtzMode.panoPlus8Ptz;
  }
}

/// Maps a [FisheyeMountType] to the [MediaSdkFisheyeFixMode] the vendor SDK
/// needs to orient its correction correctly -- 1:1, same order as the
/// vendor's own MEDIASDK_FIX_MODE_E.
MediaSdkFisheyeFixMode mediaSdkFixModeForMountType(FisheyeMountType mountType) {
  switch (mountType) {
    case FisheyeMountType.ceiling:
      return MediaSdkFisheyeFixMode.topInstallation;
    case FisheyeMountType.desktop:
      return MediaSdkFisheyeFixMode.bottomInstallation;
    case FisheyeMountType.wall:
      return MediaSdkFisheyeFixMode.sideInstallation;
  }
}

/// One corrected-YUV frame arrival, reported by the native MediaSDK decode
/// callback. There is no rendering pipeline wired up yet (see
/// MediaSdkFisheyePlayer's class doc) -- this exists purely so the
/// RTSP-in/corrected-YUV-out pipeline is observable from Dart while that
/// part is being brought up.
class MediaSdkFisheyeFrameEvent {
  MediaSdkFisheyeFrameEvent({
    required this.port,
    required this.width,
    required this.height,
    required this.dataType,
    required this.frameCount,
  });

  factory MediaSdkFisheyeFrameEvent.fromMap(Map<dynamic, dynamic> map) {
    return MediaSdkFisheyeFrameEvent(
      port: map['port'] as int,
      width: map['width'] as int,
      height: map['height'] as int,
      dataType: map['dataType'] as int,
      frameCount: map['frameCount'] as int,
    );
  }

  final int port;
  final int width;
  final int height;
  final int dataType;
  final int frameCount;

  @override
  String toString() =>
      'MediaSdkFisheyeFrameEvent(port: $port, ${width}x$height, dataType: $dataType, frameCount: $frameCount)';
}

/// Bridges to the vendor MediaSDK (see fisheye/README.md for the reference
/// C call sequence this drives, and fisheye/android/src/main/cpp/
/// mediasdk_jni.cpp / fisheye/darwin/Classes for the native side) --
/// give it an RTSP URL and a fisheye mount/PTZ mode, get corrected YUV
/// frames back.
///
/// This is a standalone spike, not wired into [RTCVideoRenderer] or
/// [WebrtcPlayerController] -- it exists to prove the pipeline (RTSP in,
/// SDK does its own decode + fisheye correction, corrected YUV out) runs
/// at all. [onFrame] fires once per corrected frame the native callback
/// observes, independent of whether a texture is attached via
/// [createTexture]. Rendering is native-only on both platforms but takes
/// different shapes: Android reuses WebRTC's own EGL pipeline
/// (SurfaceTextureRenderer, push-based); iOS implements <FlutterTexture>
/// directly and converts each frame to a BGRA CVPixelBuffer (pull-based,
/// matching how Flutter's iOS texture registry itself works) -- see
/// MediaSdkFisheyePlayer's class doc on each platform.
///
/// Native-only (Android/iOS): a no-op everywhere else.
class MediaSdkFisheyePlayer {
  MediaSdkFisheyePlayer() {
    _frameSubscription = FlutterWebRTCEventChannel
        .instance.handleEvents.stream
        .where((event) => event.containsKey('onFisheyeMediaSdkFrame'))
        .listen((event) {
      final map = event['onFisheyeMediaSdkFrame'] as Map<dynamic, dynamic>;
      onFrame.add(MediaSdkFisheyeFrameEvent.fromMap(map));
    });
  }

  StreamSubscription? _frameSubscription;

  /// Fires once per corrected YUV frame reported by the native decode
  /// callback. Useful for confirming the pipeline is actually delivering
  /// frames (width/height/frameCount) before any rendering exists.
  final StreamController<MediaSdkFisheyeFrameEvent> onFrame =
      StreamController.broadcast();

  /// Starts pulling [rtspUrl] and applying the given fisheye correction.
  /// Throws if the native side reports any step of Init/CreatPortRes/
  /// SetDecodeVideoDataCB/StartStream failing -- check native logs (logcat
  /// tag "MediaSdkFisheyePlayer" on Android) for which step and the
  /// MediaSDK error code.
  Future<void> start(
    String rtspUrl, {
    MediaSdkFisheyePtzMode ptzMode = MediaSdkFisheyePtzMode.threeSixtyPlus1Ptz,
    MediaSdkFisheyeFixMode fixMode = MediaSdkFisheyeFixMode.topInstallation,
  }) async {
    if (!(WebRTC.platformIsAndroid || WebRTC.platformIsIOS)) return;
    await WebRTC.invokeMethod('fisheyeMediaSdkStart', <String, dynamic>{
      'rtspUrl': rtspUrl,
      'ptzMode': ptzMode.value,
      'fixMode': fixMode.value,
    });
  }

  Future<void> stop() async {
    if (!(WebRTC.platformIsAndroid || WebRTC.platformIsIOS)) return;
    await WebRTC.invokeMethod('fisheyeMediaSdkStop', <String, dynamic>{});
  }

  /// Changes the fisheye correction mode on the already-running stream,
  /// without tearing down and reconnecting the RTSP session -- unlike
  /// calling [start] again, this is cheap enough to call every time the
  /// user picks a different mount type/display mode. No-op if [start]
  /// hasn't completed yet; pass the desired mode to [start] itself in
  /// that case.
  Future<void> setPtzAndFixMode(
    MediaSdkFisheyePtzMode ptzMode,
    MediaSdkFisheyeFixMode fixMode,
  ) async {
    if (!(WebRTC.platformIsAndroid || WebRTC.platformIsIOS)) return;
    await WebRTC.invokeMethod(
      'fisheyeMediaSdkSetPtzAndFixMode',
      <String, dynamic>{'ptzMode': ptzMode.value, 'fixMode': fixMode.value},
    );
  }

  /// EXPERIMENT (2026-10-14): vendor support's suggestion was "after
  /// changing the fisheye correction mode, you need to restart the video
  /// stream" -- unlike [setPtzAndFixMode] (which sets the mode on a live
  /// stream with no restart), this sets the mode and then does a
  /// StopStream+StartStream on the *same* native port (no RTSP
  /// reconnect-from-scratch, no texture/port recreation) to test whether
  /// that's what's needed for the mode change to actually take visual
  /// effect. Android only for now -- this is a diagnostic, not yet a
  /// permanent API.
  Future<bool> restartSamePortForModeChange(
    MediaSdkFisheyePtzMode ptzMode,
    MediaSdkFisheyeFixMode fixMode,
  ) async {
    if (!WebRTC.platformIsAndroid) return false;
    final result = await WebRTC.invokeMethod(
      'fisheyeMediaSdkRestartSamePortForModeChange',
      <String, dynamic>{'ptzMode': ptzMode.value, 'fixMode': fixMode.value},
    );
    return result == true;
  }

  /// Creates a Flutter [Texture] to display corrected frames on, and
  /// returns its textureId (pass straight to a `Texture(textureId: ...)`
  /// widget). Can be called before or after [start] -- frames just have
  /// nowhere to go until this exists.
  Future<int?> createTexture() async {
    if (!(WebRTC.platformIsAndroid || WebRTC.platformIsIOS)) return null;
    final response = await WebRTC.invokeMethod(
      'fisheyeMediaSdkCreateTexture',
      <String, dynamic>{},
    );
    return response['textureId'] as int?;
  }

  Future<void> disposeTexture() async {
    if (!(WebRTC.platformIsAndroid || WebRTC.platformIsIOS)) return;
    await WebRTC.invokeMethod('fisheyeMediaSdkDisposeTexture', <String, dynamic>{});
  }

  Future<void> dispose() async {
    await _frameSubscription?.cancel();
    await onFrame.close();
  }
}
