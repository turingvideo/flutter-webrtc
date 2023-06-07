import 'dart:async';
import 'dart:typed_data';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_state.dart';
import 'package:rxdart/rxdart.dart';
import 'package:turing_mobile/turing_foundation.dart';
import 'package:uuid/uuid.dart';


import 'webrtc_player_enum.dart';
import 'webrtc_player_publisher.dart';
import 'webrtc_player_stream.dart';

class WebRTCPlayerController {
  final String uuid = const Uuid().v4();
  final WebrtcResolution resolution;

  WebRTCPlayerController({required this.resolution}) {
    _streamer = WebrtcPlayerStream(
      stepCallback: onWebrtcConnectionStep,
      stateCallback: onWebrtcConnectionState,
    );
  }

  factory WebRTCPlayerController.hd() {
    final controller = WebRTCPlayerController(resolution: WebrtcResolution.hd);
    controller.hd.value = true;
    return controller;
  }

  factory WebRTCPlayerController.sd() {
    final controller = WebRTCPlayerController(resolution: WebrtcResolution.sd);
    controller.hd.value = false;
    return controller;
  }

  factory WebRTCPlayerController.normal() {
    final controller =
        WebRTCPlayerController(resolution: WebrtcResolution.normal);
    controller.hd.value = false;
    return controller;
  }

  late final WebrtcPlayerStream _streamer;

  RTCVideoRenderer get video => _streamer.video;

  WebRTCPublisher get publisher => _publisher;
  final WebRTCPublisher _publisher = WebRTCPublisher();

  AlwaysValueNotifier<PlayerState> playerState =
      AlwaysValueNotifier(PlayerState.connecting);
  ValueNotifier<DeviceOrientation> orientation =
      ValueNotifier(DeviceOrientation.portraitUp);
  ValueNotifier<bool> live = ValueNotifier(true);
  ValueNotifier<bool> hd = ValueNotifier(false);
  ValueNotifier<PlayerSpeed> speed = ValueNotifier(PlayerSpeed.x1);
  ValueNotifier<bool> record = ValueNotifier(false);
  ValueNotifier<bool> audio = ValueNotifier(false);
  ValueNotifier<TuringError> error = ValueNotifier(TuringError.none);

  final PublishSubject<int> onCurrentPosition = PublishSubject<int>();
  final PublishSubject onRestartStream = PublishSubject();
  final PublishSubject onTapForward = PublishSubject();
  final PublishSubject onTapRewind = PublishSubject();
  final PublishSubject onReceiveFirstPacket = PublishSubject();

  final PublishSubject<WebrtcConnectionStep> onUpdateConnectionStep =
      PublishSubject();

  /// Public - get value
  bool get isPlaying => playerState.value.isConnected;

  bool get isPaused => playerState.value.isPaused;

  bool get isLivePaused =>
      playerState.value.isError && error.value.em == 'paused';

  bool get isFullscreen =>
      orientation.value == DeviceOrientation.landscapeLeft ||
      orientation.value == DeviceOrientation.landscapeRight;

  /// timer for position update
  Timer? _timer;

  _startTimer(PlayerState state) {
    if (!state.isConnected) {
      stopTimer();
      return;
    }

    final bool isActive = _timer?.isActive ?? false;
    if (!isActive) {
      _timer = Timer.periodic(const Duration(seconds: 1), (timer) {
        if (!onCurrentPosition.isClosed) onCurrentPosition.add(timer.tick);
      });
    }
  }

  stopTimer() {
    _timer?.cancel();
    _timer = null;
  }

  /// WebrtcPlayerStreamCallback
  void onWebrtcConnectionStep(WebrtcConnectionStep step) {
    onUpdateConnectionStep.add(step);

    if (step == WebrtcConnectionStep.peerConnectionFailed ||
        step == WebrtcConnectionStep.streamTimeout) {
      resetDataSource();
      return;
    }

    if (step == WebrtcConnectionStep.reconnectTooManyTimes) {
      setPlayerState(PlayerState.error, webrtcReconnectTooManyTimes);
      return;
    }

    if (step == WebrtcConnectionStep.receiveFirstRender) {
      setRendered();
      return;
    }
  }

  void onWebrtcConnectionState(RTCPeerConnectionState s) {
    if (this.playerState.value.isConnected && s.temporaryDisconnected) return;
    var playerState = s.playerState;
    if (playerState.isError) {
      setPlayerState(playerState, webrtcConnectionError);
    } else {
      setPlayerState(playerState, TuringError.none);
    }
  }

  /// state
  void setPlayerState(PlayerState state, TuringError error) {
    debug("======= state = $state, error = ${error.em}, uuid = $uuid");
    playerState.value = state;
    this.error.value = error;
    state.isConnected ? _startTimer(state) : stopTimer();
  }

  /// data source
  void setDataSource({required String url, bool live = true, String? codec}) {
    this.live.value = live;
    setPlayerState(PlayerState.connecting, TuringError.none);
    _streamer.autoPlay(url: url, codec: codec);
  }

  /// reset data source
  void resetDataSource() {
    setPlayerState(PlayerState.connecting, TuringError.none);
    if (!onRestartStream.isClosed) onRestartStream.add(null);
  }

  /// set record data source
  void setRecordDataSource({required String recordUrl}) {
    record.value = true;
    twoWayAudio(url: recordUrl);
  }

  /// record on
  void setRecordOn() {
    record.value = true;
  }

  /// record off
  void setRecordOff() {
    record.value = false;
  }

  /// play or pause
  void setRendered() {
    setPlayerState(PlayerState.connected, TuringError.none);
    _keepAlive = false;
    _keepAliveTimer?.cancel();
    _keepAliveTimer = null;
    info("========== set rendered, uuid = $uuid");
  }

  /// resume stream from pause
  void setResume() {
    if (!playerState.value.isPaused) return; // if not paused, return
    setPlayerState(PlayerState.resumed, TuringError.none);
  }

  /// pause stream
  void setPause() {
    if (!playerState.value.isConnected) return; // if not connected, return
    setPlayerState(PlayerState.paused, TuringError.none);
  }

  /// enter full screen
  void setEnterFullScreen(DeviceOrientation orientation) {
    this.orientation.value = orientation;
  }

  /// exist full screen
  void setExistFullScreen() {
    orientation.value = DeviceOrientation.portraitUp;
  }

  /// switch hd
  void switchHd() {
    hd.value = !hd.value;
  }

  /// switch audio
  void setAudioOn() {
    audio.value = true;
    _streamer.setAudioOn();
  }

  void setAudioOff() {
    audio.value = false;
    _streamer.setAudioOff();
  }

  /// set player speed
  void setPlayerSpeed(PlayerSpeed speed) {
    this.speed.value = speed;
  }

  /// screenshot
  Future<ByteBuffer> setCaptureFrame() => _streamer.setCaptureFrame();

  void dispose() {
    stopTimer();
    onCurrentPosition.close();
    onRestartStream.close();
    onTapRewind.close();
    onTapForward.close();
    closeStream();
  }

  /// close player connection
  ///
  ///
  Timer? _keepAliveTimer;
  bool _keepAlive = false;

  set keepAlive(bool newValue) {
    if (newValue && _streamer.renderVideo && playerState.value.isConnected) {
      _keepAlive = newValue;
    }
    debug("============ keep alive = $_keepAlive, uuid = $uuid");
  }

  Future<void> closeStream({TuringError error = webrtcConnectionError}) async {
    try {
      audio.value = false;
      record.value = false;

      _publisher.dispose();
      _streamer.dispose();

      _keepAliveTimer?.cancel();
      _keepAliveTimer = null;
      _keepAlive = false;

      final state =
          error.isNotNone ? PlayerState.error : PlayerState.connecting;
      playerState.setValueWithoutNotifier(
          error.isNotNone ? PlayerState.error : PlayerState.connecting);
      this.error.value = error;
      state.isConnected ? _startTimer(state) : stopTimer();
    } catch (E) {
      info("======= close stream error = ${E.toString()}");
    }
    info("================ stream has been closeStream, id = $uuid");
  }

  void prepareCloseStream() {
    if (_keepAlive) {
      _keepAliveTimer?.cancel();
      _keepAliveTimer = Timer(const Duration(milliseconds: 1000), () {
        closeStream();
      });
    } else {
      closeStream();
    }
  }

  /// if stream always connection,
  /// return true to cancel close stream and use current alive stream
  /// instead of start stream.
  bool prepareStartStream() {
    debug("============= prepare start stream, keep alive = $_keepAlive");

    if (_keepAlive) {
      setRendered();
      return true;
    }

    debug("============= prepare start stream false");
    closeStream(error: TuringError.none);
    return false;
  }

  /// two way audio
  void twoWayAudio({required String url}) async {
    try {
      final stream = await navigator.mediaDevices.getUserMedia({
        'video': false,
        'audio': true,
      });
      await _publisher.publish(url, stream);
    } catch (_) {
      setRecordOff();
    }
  }
}
