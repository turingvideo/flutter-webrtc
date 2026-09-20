import 'dart:async';

import 'event_channel.dart';
import 'utils.dart';

/// MEDIASDK_PTZ_MODE_E (fisheye/android/vendor/include/media_sdk_com_def.h)
/// -- only the modes this project currently cares about; the vendor header
/// has more (fisheye+3/4/8-PTZ, panorama+N-PTZ, etc).
enum MediaSdkFisheyePtzMode {
  original(0),
  pano180(1),
  threeSixtyPlus1Ptz(2),
  threeSixtyPlus6Ptz(3);

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
/// at all before building the GPU-upload-to-Flutter-Texture rendering
/// path on top of it. [onFrame] fires once per corrected frame the native
/// callback observes; there is intentionally no on-screen video yet.
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

  Future<void> dispose() async {
    await _frameSubscription?.cancel();
    await onFrame.close();
  }
}
