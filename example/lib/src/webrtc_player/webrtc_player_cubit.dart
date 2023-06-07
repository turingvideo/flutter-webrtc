import 'dart:async';

import 'package:bloc/bloc.dart';

import 'package:equatable/equatable.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/services.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_state.dart';
import 'package:flutter_webrtc_example/src/webrtc_player/webrtc_player_controller.dart';
import 'package:flutter_webrtc_example/src/webrtc_player/webrtc_player_enum.dart';
import 'package:flutter_webrtc_example/src/webrtc_player/webrtc_player_metrics.dart';
import 'package:turing_mobile/turing_mobile.dart';

import '../custom_error/stream_error.dart';
import '../nest_camera.dart';

part 'webrtc_player_state.dart';

// typedef StreamCallback = void Function(LiveStream stream, bool live);

class WebrtcPlayerCubit extends Cubit<WebrtcPlayerState> {
  WebrtcPlayerCubit({

    WebrtcResolution resolution = WebrtcResolution.sd,
    NestCamera? camera,
    this.canUploadStreamMetrics = false,
  })  : 
        controller =
            ValueNotifier(WebRTCPlayerController(resolution: resolution)),
        newController = ValueNotifier(null),
        super(WebrtcPlayerState(camera: camera)) {
    initializeListeners(playerController: playerController);
  }



  /// webrtc player controller
  final ValueNotifier<WebRTCPlayerController> controller;

  WebRTCPlayerController get playerController => controller.value;

  final ValueNotifier<WebRTCPlayerController?> newController;

  WebRTCPlayerController? get newPlayerController => newController.value;

  /// stream subscription
  StreamSubscription? _streamSubscription;
  StreamSubscription? _onWebrtcConnectionStepSubscription;
  StreamSubscription? _restartSubscription;

  /// private
  Timer? _heartBeatTimer;
  DateTime? restartTime; // current play dateTime
  int _reconnectTimes = 0;

  /// stream metrics
  final bool canUploadStreamMetrics;

  WebrtcPlayerMetrics metrics = WebrtcPlayerMetrics();

  /// video global key
  final GlobalKey _videoGlobalKey = GlobalKey();

  GlobalKey get videoGlobalKey => _videoGlobalKey;

  /// view global key
  final GlobalKey _viewGlobalKey = GlobalKey();

  GlobalKey get viewGlobalKey => _viewGlobalKey;

  /// transformation controller for scale video.
  TransformationController transformationController =
      TransformationController();

  void transformationIdentity() =>
      transformationController.value = Matrix4.identity();

  /// initialize player controller listeners
  void initializeListeners({required WebRTCPlayerController playerController}) {
    playerController.record.addListener(updateTwoWayAudio);
    playerController.playerState.addListener(updatePlayerState);
    // _onWebrtcConnectionStepSubscription =
    //     playerController.onUpdateConnectionStep.listen(updateConnectionStep);
    playerController.hd.addListener(switchStreamQuality);
    _restartSubscription =
        playerController.onRestartStream.listen((_) => restartStream());
  }

  /// close
  @override
  Future<void> close() {
    stopHeartBeatTimer();
    _streamSubscription?.cancel();
    _restartSubscription?.cancel();
    _onWebrtcConnectionStepSubscription?.cancel();
    playerController.playerState.removeListener(updatePlayerState);
    playerController.record.removeListener(updateTwoWayAudio);
    playerController.hd.removeListener(switchStreamQuality);
    playerController.dispose();
    transformationController.dispose();
    controller.dispose();
    newController.dispose();
    return super.close();
  }

  /// switch hd
  void switchStreamQuality() {
    final camera = state.camera;
    if (camera == null) return;

    if (playerController.hd.value &&
        playerController.resolution == WebrtcResolution.hd) return;
    if (playerController.hd.value == false &&
        playerController.resolution == WebrtcResolution.sd) return;

    final newPlayerController = playerController.hd.value
        ? WebRTCPlayerController.hd()
        : WebRTCPlayerController.sd();

    newController.value = newPlayerController;
    newPlayerController.playerState.addListener(updateNewPlayerController);
    newPlayerController.onRestartStream.listen((_) {
      startStream(
        url: '',
      );
    });

    startStream(
        url: '',
      );
  }

  void updateNewPlayerController() {
    final np = newPlayerController;
    if (np == null) return;

    final playerState = np.playerState.value;
    if (playerState.isConnected) {
      playerController.dispose();
      playerController.playerState.removeListener(updatePlayerState);
      playerController.record.removeListener(updateTwoWayAudio);
      playerController.hd.removeListener(switchStreamQuality);
      _onWebrtcConnectionStepSubscription?.cancel();
      _restartSubscription?.cancel();

      controller.value = np;
      newController.value = null;

      initializeListeners(playerController: np);
      np.playerState.removeListener(updateNewPlayerController);
    }

    if (playerState.isError) {
      controller.value.switchHd();
      newController.value = null;
    }
  }

  /// switch record
  void updateTwoWayAudio() {
    if (playerController.record.value) {

    } else {

    }
  }

  /// stream file time collect


  /// stop stream
  void stopStream({TuringError error = streamDisconnectError}) {
    playerController.closeStream(error: error);
    if (!isClosed) {
      emit(state.copyWith(
        status: Status.failure(error: error),
        positionUpdate: false,
      ));
    }
  }

  /// restart stream
  void restartStream({bool force = false}) {
    final camera = state.camera;
    if (camera == null) {
      stopStream();
      return;
    }
    startStream(
      url: ''
    );
  }

  /// start stream
  void startStream({
    required String url,
  }) {
    playerController.setDataSource(
      url: url,
      live: true,
      codec: StreamEncode.h264.label,
    );
  }



  /// Support Two Way Audio
  ///
  /// start two way audio


  void startHeartBeatTimer() {

  }

  void stopHeartBeatTimer() {
    _heartBeatTimer?.cancel();
    _heartBeatTimer = null;
  }

  /// two way audio heart beat


  /// stop two way audio


  /// update player state
  void updatePlayerState() {
    // onPlayerStateUpdate.add(playerController.playerState.value);
    // emit(state.copyWith(playerState: playerState));
    // if (playerState.isConnected || playerState.isError) stopStreamLatency();
    if (isClosed) return;

    if (playerController.playerState.value.isPaused) {
      if (playerController.live.value) {
        stopStream(error: streamPausedError);
      } else {

      }
    }
  }

  /// Stream Latency
  ///
  /// start stream latency


  /// Stream Pause and Resume
  ///
  ///
  bool _isPaused = false;
  StreamSubscription? _pauseStreamSubscription;

  /// pause


  /// resume


  /// stop stream


  /// Speed
  ///
  ///
  /// speed changed


  /// enter fullscreen
  void enterFullscreen(DeviceOrientation orientation) {
    if (!playerController.isFullscreen) {
      playerController.setEnterFullScreen(orientation);
    }
  }

  /// exit fullscreen
  void exitFullscreen() {
    if (playerController.isFullscreen) {
      playerController.setExistFullScreen();
    }
  }

  /// switch audio
  void setAudioOn() => playerController.setAudioOn();

  void setAudioOff() => playerController.setAudioOff();

  void switchAudio() {
    playerController.audio.value ? setAudioOff() : setAudioOn();
  }
}
