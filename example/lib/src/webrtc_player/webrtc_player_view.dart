import 'dart:async';
import 'dart:math';

import 'package:after_layout/after_layout.dart';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:flutter_webrtc_example/src/custom_error/custom_error.dart';

import 'package:flutter_webrtc_example/src/player_panel/player_settings.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_state.dart';

import 'package:flutter_webrtc_example/src/webrtc_player/webrtc_player_panel.dart';

import 'package:turing_mobile/turing_mixin.dart';

import 'package:uuid/uuid.dart';

import 'webrtc_player_controller.dart';
import 'webrtc_player_cubit.dart';

class WebrtcPlayerView extends StatefulWidget {
  final PlayerSettings settings;
  final PlayerSettings? fullscreenSettings;
  final VoidCallback? onTap;
  final VoidCallback? onDoubleTap;
  final bool autoFullscreen;

  const WebrtcPlayerView({
    Key? key,
    required this.settings,
    this.fullscreenSettings,
    this.onTap,
    this.onDoubleTap,
    this.autoFullscreen = false,
  }) : super(key: key);

  @override
  _WebrtcPlayerViewState createState() => _WebrtcPlayerViewState();
}

class _WebrtcPlayerViewState extends State<WebrtcPlayerView>
    with ToastMixin, DialogMixin, AfterLayoutMixin {
  late WebrtcPlayerCubit cubit;
  late WebRTCPlayerController playerController;

  late TransformationController transformationController;

  late RTCVideoRenderer video;

  @override
  void initState() {
    super.initState();
    cubit = context.read<WebrtcPlayerCubit>();
    playerController = cubit.playerController;
    playerController.orientation.addListener(_onFullscreenChanged);
    transformationController = cubit.transformationController;
    playerController.playerState.addListener(onUpdatePlayerState);
    video = playerController.video;

    cubit.controller.addListener(updatePlayerController);
  }

  void updatePlayerController() {
    playerController.orientation.removeListener(_onFullscreenChanged);
    playerController.playerState.removeListener(onUpdatePlayerState);

    playerController = cubit.playerController;
    playerController.orientation.addListener(_onFullscreenChanged);
    playerController.playerState.addListener(onUpdatePlayerState);

    onUpdatePlayerState();
  }

  @override
  void didUpdateWidget(covariant WebrtcPlayerView oldWidget) {
    super.didUpdateWidget(oldWidget);
    final newCubit = context.read<WebrtcPlayerCubit>();
    if (cubit != newCubit) {
      cubit = newCubit;
      cubit.controller.removeListener(updatePlayerController);
      playerController.orientation.removeListener(_onFullscreenChanged);
      playerController.playerState.removeListener(onUpdatePlayerState);

      playerController = cubit.playerController;
      playerController.orientation.addListener(_onFullscreenChanged);
      playerController.playerState.addListener(onUpdatePlayerState);
      cubit.controller.addListener(updatePlayerController);
      transformationController = cubit.transformationController;
      video = playerController.video;
    }
    info("======= webrtc player view did update widget");
  }

  @override
  FutureOr<void> afterFirstLayout(BuildContext context) {
    if (playerController.isPlaying) playerController.setRendered();
  }

  void onUpdatePlayerState() {
    if (playerController.playerState.value.isConnected) {
      if (mounted) {
        setState(() {
          video = playerController.video;
        });
      }
    }
  }

  @override
  void dispose() {
    debug("============== webrtc player view dispose");
    playerController.prepareCloseStream();
    playerController.orientation.removeListener(_onFullscreenChanged);
    playerController.playerState.removeListener(onUpdatePlayerState);
    cubit.controller.removeListener(updatePlayerController);
    super.dispose();
  }

  void _onFullscreenChanged() {
    if (!widget.autoFullscreen) return;
    if (playerController.isFullscreen) {
      Navigator.of(context).push(
        PageRouteBuilder<void>(
          pageBuilder: (context, animation, _) {
            return AnimatedBuilder(
              animation: animation,
              builder: (BuildContext context, Widget? child) {
                return BlocProvider.value(
                  value: cubit,
                  child: _FullscreenView(
                    settings: widget.fullscreenSettings ?? widget.settings,
                  ),
                );
              },
            );
          },
        ),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return MultiBlocListener(
      listeners: [
        BlocListener<WebrtcPlayerCubit, WebrtcPlayerState>(
          listenWhen: (p, n) => p.status != n.status,
          listener: (context, state) {
            if (state.status.isFailure) {
              if (state.status.error.ec == streamCameraNotExist) {
                toastError(context, state.status.error);
              }
            }
          },
        ),
        BlocListener<WebrtcPlayerCubit, WebrtcPlayerState>(
          listenWhen: (p, n) => p.audioStatus != n.audioStatus,
          listener: (context, state) {
            if (state.audioStatus.isFailure) {
              toastStatus(context, state.audioStatus);
            }
          },
        ),
        BlocListener<WebrtcPlayerCubit, WebrtcPlayerState>(
          listenWhen: (p, n) =>
              p.audioHeartBeatStatus != n.audioHeartBeatStatus,
          listener: (context, state) {
            if (state.audioHeartBeatStatus.isFailure) {
              toastFailed(context, message: 'Two way audio failed');
            }
          },
        ),
      ],
      child: BlocBuilder<WebrtcPlayerCubit, WebrtcPlayerState>(
        key: cubit.videoGlobalKey,
        bloc: cubit,
        builder: (context, state) {
          return LayoutBuilder(builder: (context, constraints) {
            final rect = Rect.fromLTWH(
              0,
              0,
              constraints.maxWidth,
              constraints.maxHeight,
            );

            return _build(context, rect, state);
          });
        },
      ),
    );
  }

  Widget _build(BuildContext context, Rect pos, WebrtcPlayerState state) {
    return Container(
      color: Colors.black,
      alignment: Alignment.center,
      child: SizedBox(
        width: pos.width,
        height: pos.height,
        child: Stack(
          children: [
            InteractiveViewer(
              maxScale: 5,
              minScale: 1,
              transformationController: transformationController,
              child: Center(
                child: AspectRatio(
                    aspectRatio: 16 / 9,
                    child: RTCVideoView(
                      video,
                      objectFit:
                          RTCVideoViewObjectFit.RTCVideoViewObjectFitCover,
                    )),
              ),
            ),

            /// webrtc player panel build
            Positioned.fill(
              child: WebrtcPlayerPanel(
                controller: playerController,
                texturePos: pos,
                settings: widget.settings,
                fullscreenSettings: widget.fullscreenSettings,
                builder: widget.settings.canBuildPanel
                    ? (context) => _buildWebrtcLiveView(widget.settings)
                    : null,
                fullscreenBuilder:
                    (widget.fullscreenSettings ?? widget.settings).canBuildPanel
                        ? (context) => _buildWebrtcFullscreenView(
                            widget.fullscreenSettings ?? widget.settings)
                        : null,
                onTap: widget.onTap,
                onDoubleTap: widget.onDoubleTap,
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildWebrtcLiveView(PlayerSettings settings) {
    return const SizedBox();
  }

  Widget _buildWebrtcFullscreenView(PlayerSettings settings) {
    return const SizedBox();
  }
}

class _FullscreenView extends StatefulWidget {
  final PlayerSettings settings;

  const _FullscreenView({super.key, required this.settings});

  @override
  _FullscreenViewState createState() => _FullscreenViewState();
}

class _FullscreenViewState extends State<_FullscreenView>
    with ToastMixin, DialogMixin {
  late WebrtcPlayerCubit cubit;
  late WebRTCPlayerController playerController;

  @override
  void initState() {
    super.initState();
    SystemChrome.setEnabledSystemUIMode(SystemUiMode.manual, overlays: []);
    cubit = context.read<WebrtcPlayerCubit>();
    playerController = cubit.playerController;
    cubit.controller.addListener(_onUpdatePlayerController);
    playerController.orientation.addListener(_onFullscreenChanged);
    playerController.playerState.addListener(_onPlayerStateChanged);

  }

  @override
  void dispose() {
    playerController.orientation.removeListener(_onFullscreenChanged);
    playerController.playerState.removeListener(_onPlayerStateChanged);
    cubit.controller.removeListener(_onUpdatePlayerController);
    super.dispose();
  }

  @override
  void didUpdateWidget(covariant _FullscreenView oldWidget) {
    super.didUpdateWidget(oldWidget);
    if (cubit.playerController != playerController) {
      cubit.controller.removeListener(_onUpdatePlayerController);
      playerController.orientation.removeListener(_onFullscreenChanged);
      playerController.playerState.removeListener(_onPlayerStateChanged);

      playerController = cubit.playerController;

      playerController.orientation.addListener(_onFullscreenChanged);
      playerController.playerState.addListener(_onPlayerStateChanged);
      cubit.controller.addListener(_onUpdatePlayerController);
    }
  }

  void _onFullscreenChanged() {
    if (!playerController.isFullscreen) {
      Navigator.of(context).pop();

    }
  }

  void _onPlayerStateChanged() {
    setState(() {});
  }

  void _onUpdatePlayerController() {
    playerController.orientation.removeListener(_onFullscreenChanged);
    playerController.playerState.removeListener(_onPlayerStateChanged);

    final orientation = playerController.orientation.value;

    playerController = cubit.playerController;

    playerController.orientation.addListener(_onFullscreenChanged);
    playerController.playerState.addListener(_onPlayerStateChanged);

    playerController.setEnterFullScreen(orientation);
    _onPlayerStateChanged();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: WillPopScope(
        onWillPop: () {
          playerController.setExistFullScreen();
          return Future.value(true);
        },
        child: _buildPlayerViewZone(context,
            child: cubit.videoGlobalKey.currentState?.build(context) ??
                const SizedBox()),
      ),
    );
  }

  /// calculate size
  Widget _buildPlayerViewZone(BuildContext context, {required Widget child}) {
    final maxWidth = MediaQuery.of(context).size.width;
    final aspectRationHeight = maxWidth / 16 * 9;

    final maxHeight =
        min(MediaQuery.of(context).size.height, aspectRationHeight) - 1;
    final newConstraints = BoxConstraints.loose(Size(maxWidth, maxHeight));
    final Size childSize = getTxSize(newConstraints);
    final Offset offset = getTxOffset(newConstraints, childSize);
    final Rect pos =
        Rect.fromLTWH(offset.dx, offset.dy, childSize.width, childSize.height);

    return Container(
      color: Colors.black,
      child: Center(
        child: SizedBox(
          width: pos.width,
          height: pos.height,
          child: Stack(
            children: [
              Positioned.fill(
                child: InteractiveViewer(
                  maxScale: 5,
                  minScale: 1,
                  panEnabled: false,
                  child: child,
                ),
              ),
              Positioned.fill(
                child: WebrtcPlayerPanel(
                  controller: playerController,
                  texturePos: pos,
                  settings: widget.settings,
                  fullscreenSettings: widget.settings,
                  builder: widget.settings.canBuildPanel
                      ? (context) => _buildWebrtcLiveView(widget.settings)
                      : null,
                  fullscreenBuilder: (widget.settings).canBuildPanel
                      ? (context) => _buildWebrtcFullscreenView(widget.settings)
                      : null,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildWebrtcLiveView(PlayerSettings settings) {
    return const SizedBox();
  }

  Widget _buildWebrtcFullscreenView(PlayerSettings settings) {
    return const SizedBox();
  }

  /// calculate size
  Size getTxSize(BoxConstraints constraints) {
    Size childSize = applyAspectRatio(constraints, 16 / 9);
    double sizeFactor = 1.0;
    if (-1.0 < sizeFactor && sizeFactor < -0.0) {
      sizeFactor = max(constraints.maxWidth / childSize.width,
          constraints.maxHeight / childSize.height);
    } else if (-2.0 < sizeFactor && sizeFactor < -1.0) {
      sizeFactor = constraints.maxWidth / childSize.width;
    } else if (-3.0 < sizeFactor && sizeFactor < -2.0) {
      sizeFactor = constraints.maxHeight / childSize.height;
    } else if (sizeFactor < 0) {
      sizeFactor = 1.0;
    }
    childSize = childSize * sizeFactor;
    return childSize;
  }

  Size applyAspectRatio(BoxConstraints constraints, double aspectRatio) {
    assert(constraints.hasBoundedHeight && constraints.hasBoundedWidth);
    constraints = constraints.loosen();

    double width = constraints.maxWidth;
    double height = width;

    if (width.isFinite) {
      height = width / aspectRatio;
    } else {
      height = constraints.maxHeight;
      width = height * aspectRatio;
    }

    if (width > constraints.maxWidth) {
      width = constraints.maxWidth;
      height = width / aspectRatio;
    }

    if (height > constraints.maxHeight) {
      height = constraints.maxHeight;
      width = height * aspectRatio;
    }

    if (width < constraints.minWidth) {
      width = constraints.minWidth;
      height = width / aspectRatio;
    }

    if (height < constraints.minHeight) {
      height = constraints.minHeight;
      width = height * aspectRatio;
    }

    return constraints.constrain(Size(width, height));
  }

  Offset getTxOffset(BoxConstraints constraints, Size childSize) {
    final Offset diff = (constraints.biggest - childSize) as Offset;
    return Alignment.center.alongOffset(diff);
  }
}
