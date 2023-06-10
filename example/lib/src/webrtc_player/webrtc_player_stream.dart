import 'dart:async';
import 'dart:typed_data';

import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:turing_mobile/turing_components.dart';

import 'webrtc_player.dart';
import 'webrtc_player_enum.dart';

class WebrtcPlayerStream {
  WebrtcPlayerStream({
    this.stepCallback,
    this.stateCallback,
  });

  final WebRTCPlayer _player = WebRTCPlayer();

  WebRTCPlayer get player => _player;

  RTCVideoRenderer? _video;

  RTCVideoRenderer get video => _video ?? RTCVideoRenderer();

  final WebrtcConnectionStepCallback? stepCallback;
  final WebrtcConnectionStateCallback? stateCallback;

  bool get renderVideo => _video?.renderVideo ?? false;

  /// Player
  Timer? reconnectTimer;
  final int reconnectInterval = 6;
  int reconnectTimes = 0;
  static int maxReconnectTimes = 5;

  void autoPlay({required String url, String? codec}) async {
    _video = RTCVideoRenderer();
    await _video?.initialize();

    _video?.onResize = () {};
    
    _video?.onDidFirstRendered = () {
      info('onDidFirstRendered');

      reconnectTimer?.cancel();
      reconnectTimer = null;
      reconnectTimes = 0;

      stepCallback?.call(WebrtcConnectionStep.receiveFirstRender);
    };

    player.onRemoteStream = (MediaStream stream) {
      debug('onRemoteStream MediaStream: ${stream.toString()}');

      if (stream.getAudioTracks().isNotEmpty) {
        stream.getAudioTracks()[0].enabled = false;
      }

      _video?.srcObject = stream;

      stepCallback?.call(WebrtcConnectionStep.onAddRemoteStream);
    };

    player.onConnectionState = (RTCPeerConnectionState s) {
      info('$s', tag: 'onConnectionState');

      stateCallback?.call(s);
    };

    player.onReceiveFirstPacket = () {
      stepCallback?.call(WebrtcConnectionStep.receiveFirstPacket);
      // startReconnect();
    };

    try {
      String uri = url;
      if (codec != null) uri = '$url?codec=$codec';
      debug('codec: $codec,\nurl: $url\n,uri: $uri');
      await player.play(uri);
      stepCallback?.call(WebrtcConnectionStep.peerConnectionSuccess);
    } catch (e) {
      reconnectTimes++;
      stepCallback?.call(WebrtcConnectionStep.peerConnectionFailed);
      return;
    }

    // startReconnect();
  }

  void startReconnect() {
    reconnectTimer?.cancel();

    reconnectTimer = Timer(
      Duration(seconds: reconnectInterval),
      () {
        reconnectTimes++;

        if (reconnectTimes > maxReconnectTimes) {
          stepCallback?.call(WebrtcConnectionStep.reconnectTooManyTimes);

          reconnectTimes = 0;

          player.dispose();
        } else {
          stepCallback?.call(WebrtcConnectionStep.streamTimeout);
        }
      },
    );
  }

  void setAudioOn() =>
      _video?.srcObject?.getAudioTracks().firstObject?.enabled = true;

  void setAudioOff() =>
      _video?.srcObject?.getAudioTracks().firstObject?.enabled = false;

  Future<ByteBuffer> setCaptureFrame() async {
    final track = _video?.srcObject?.getVideoTracks().firstObject;
    if (track == null) throw 'Failed to take snapshot';
    final buffer = await track.captureFrame();
    return buffer;
  }

  void dispose() {
    _player.dispose();
    // _video?.dispose().catchError((_) {});
    // _video = null;
  }
}
