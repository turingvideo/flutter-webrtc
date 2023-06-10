import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_settings.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_state.dart';
import 'package:flutter_webrtc_example/src/player_panel/view/player_connecting_view.dart';
import 'package:flutter_webrtc_example/src/player_panel/view/player_live_badge.dart';
import 'package:flutter_webrtc_example/src/utils/hit_test_blocker.dart';
import 'package:flutter_webrtc_example/src/webrtc_player/webrtc_player_controller.dart';
import 'package:turing_mobile/turing_mobile.dart';

class WebrtcPlayerPanel extends StatefulWidget {
  final PlayerSettings settings;
  final PlayerSettings? fullscreenSettings;

  final WebRTCPlayerController controller;
  final Rect texturePos;

  final WidgetBuilder? builder;
  final WidgetBuilder? fullscreenBuilder;

  final VoidCallback? onTap;
  final VoidCallback? onDoubleTap;

  const WebrtcPlayerPanel({
    super.key,
    required this.controller,
    required this.texturePos,
    required this.settings,
    this.fullscreenSettings,
    this.builder,
    this.fullscreenBuilder,
    this.onTap,
    this.onDoubleTap,
  });

  @override
  _WebrtcPlayerPanelState createState() => _WebrtcPlayerPanelState();
}

class _WebrtcPlayerPanelState extends State<WebrtcPlayerPanel>
    with DialogMixin {
  PlayerState _state = PlayerState.connecting;
  String? _exception;

  Timer? timer;
  bool _hidden = false;

  late WebRTCPlayerController playerController;

  Timer? _standbyTimer;
  Timer? _sdTimer;
  bool _standby = false;

  @override
  void initState() {
    super.initState();
    playerController = widget.controller;
    playerController.playerState.addListener(_onPlayerControllerChanged);
    playerController.error.addListener(_onPlayerControllerChanged);
  }

  @override
  void dispose() {
    info("====== WebrtcPlayerPanel dispose");
    playerController.playerState.removeListener(_onPlayerControllerChanged);
    playerController.error.removeListener(_onPlayerControllerChanged);
    timer?.cancel();
    timer = null;
    _standbyTimer?.cancel();
    _standbyTimer = null;
    _sdTimer?.cancel();
    _sdTimer = null;
    super.dispose();
  }

  @override
  void didUpdateWidget(covariant WebrtcPlayerPanel oldWidget) {
    super.didUpdateWidget(oldWidget);
    if (playerController != widget.controller) {
      playerController.playerState.removeListener(_onPlayerControllerChanged);
      playerController.error.removeListener(_onPlayerControllerChanged);
      playerController = widget.controller;
      playerController.playerState.addListener(_onPlayerControllerChanged);
      playerController.error.addListener(_onPlayerControllerChanged);
      _onPlayerControllerChanged();
    }
  }

  @override
  void didChangeDependencies() {
    super.didChangeDependencies();
    _onPlayerControllerChanged();
  }

  /// listener
  void _onPlayerControllerChanged() {
    if (!mounted) return;
    setState(() {
      _state = playerController.playerState.value;
      _exception = playerController.error.value.isNotNone
          ? playerController.error.value.em
          : null;
      _hidden = _state.isConnected;
    });
    if (_hidden) startTimer();
    if (_state.isConnected) startStandbyTimer();
  }

  /// auto hidden panel logic
  void revertPanel() {
    if (_hidden) startTimer();
    setState(() {
      _hidden = !_hidden;
    });
  }

  void startTimer() {
    final settings = playerController.isFullscreen
        ? widget.fullscreenSettings ?? widget.settings
        : widget.settings;
    if (settings.canAutoHidden == false) {
      setState(() {
        _hidden = false;
      });
      return;
    }

    timer?.cancel();
    timer = Timer(
      Duration(seconds: settings.autoHiddenInterval),
      () {
        if (_state.isConnected && mounted) {
          setState(() {
            _hidden = true;
          });
        } else {
          startTimer();
        }
      },
    );
  }

  /// standby timer
  void startStandbyTimer() {
    final settings = playerController.isFullscreen
        ? widget.fullscreenSettings ?? widget.settings
        : widget.settings;

    if (settings.canHdStandby == false) return;
    if (playerController.hd.value == false) return;

    _standbyTimer?.cancel();
    _standbyTimer = Timer(Duration(seconds: settings.hdStandbyInterval), () {
      if (_state.isConnected && playerController.hd.value) {
        setState(() {
          _standby = true;
          startSwitchToSdTimer();
        });
      } else {
        restartStandbyTimer();
      }
    });
  }

  void restartStandbyTimer() {
    setState(() {
      _standby = false;
    });
    startStandbyTimer();
    _sdTimer?.cancel();
    _sdTimer = null;
  }

  void startSwitchToSdTimer() {
    _sdTimer?.cancel();
    _sdTimer = Timer(const Duration(seconds: 30), () {
      if (_standby && playerController.hd.value) {
        playerController.switchHd();
        setState(() {
          _standby = false;
        });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    final settings = playerController.isFullscreen
        ? widget.fullscreenSettings ?? widget.settings
        : widget.settings;
    final builder = playerController.isFullscreen
        ? widget.fullscreenBuilder ?? widget.builder
        : widget.builder;

    var builderView = builder?.call(context);

    return SizedBox(
      width: MediaQuery.of(context).size.width,
      height: MediaQuery.of(context).size.height,
      child: !settings.online
          ? Stack(
              children: [
                Container(color: Colors.black54),
                Center(child: Icon(Icons.cloud_off_outlined)),
                _buildDescription(settings),
              ],
            )
          : HitTestBlocker(
              child: Stack(
                children: [
                  /// snapshot
                  // Visibility(
                  //   visible: !_state.isConnected,
                  //   child: Expanded(
                  //     child: Container(color: Colors.black),
                  //   ),
                  // ),

                  /// live badge
                  Visibility(
                    visible: _state.isConnected && settings.liveBadge,
                    child: Positioned(
                      top: 12,
                      right: widget.texturePos.left + 12,
                      child: const PlayerLiveBadge(),
                    ),
                  ),

                  /// core ai, license
                  

                  /// description
                  _buildDescription(settings),

                  /// panel
                  GestureDetector(
                    onTap: () {
                      widget.onTap?.call();
                      if (settings.canAutoHidden) revertPanel();
                    },
                    onDoubleTap: () {
                      if (settings.canDoubleTap) {
                        widget.onDoubleTap?.call();
                      }
                    },
                    child: Container(
                      color: Colors.transparent,
                      height: double.infinity,
                      width: double.infinity,
                      alignment: Alignment.bottomCenter,
                      child: _state.isConnecting
                          // connecting
                          ? const PlayerConnectingView()
                          // non-connecting
                          : Visibility(
                              visible: settings.canAutoHidden ? !_hidden : true,
                              child: builderView ?? const SizedBox(),
                            ),
                    ),
                  ),

                  /// back button
                  Visibility(
                    visible: playerController.isFullscreen
                        ? settings.canAutoHidden
                            ? !_hidden
                            : true
                        : false,
                    child: Positioned(
                      child: Container(
                        height: 44,
                        decoration: const BoxDecoration(
                          gradient: LinearGradient(
                            colors: [Colors.black, Colors.black12],
                            begin: Alignment.topCenter,
                            end: Alignment.bottomCenter,
                          ),
                        ),
                        padding: const EdgeInsets.only(left: 10),
                        child: GestureDetector(
                          behavior: HitTestBehavior.translucent,
                          onTap: () {
                            playerController.isFullscreen
                                ? playerController.setExistFullScreen()
                                : playerController.setEnterFullScreen(
                                    DeviceOrientation.landscapeRight);
                          },
                          child: Container(
                            color: Colors.transparent,
                            child: Row(
                              children: [
                                Icon(Icons.arrow_back),
                                Text("Cameras",
                                    style: const TextStyle().white.w600),
                              ],
                            ),
                          ),
                        ),
                      ),
                    ),
                  ),

                  /// error
                  // if (_exception != null)
                  //   Positioned(
                  //     child: Container(
                  //       color: TuringColor.redF88078,
                  //       height: 30,
                  //       child: Center(
                  //         child: Text(_state.label,
                  //             style: const TextStyle(color: Colors.white)),
                  //       ),
                  //     ),
                  //   )
                  if (_exception != null && _exception != 'paused') ...[
                    Positioned.fill(child: Container(color: Colors.black54)),
                    Center(
                      child: IconButton(
                        icon: Icon(Icons.refresh),
                        onPressed: () => playerController.resetDataSource(),
                      ),
                    ),
                  ],

                  if (_standby)
                    Positioned(
                      child: Container(
                          color: Colors.black54,
                          alignment: Alignment.center,
                          child: Column(
                            mainAxisSize: MainAxisSize.min,
                            children: [
                              Text(
                                'Are you still watching?',
                                style: const TextStyle().size17.w600.white,
                              ),
                              ElevatedButton(
                                style: ElevatedButton.styleFrom(
                                  fixedSize: const Size(161, 32),
                                  minimumSize: const Size(0, 0),
                                ),
                                onPressed: restartStandbyTimer,
                                child: Text(
                                  "Yes, I'm still here",
                                  style: const TextStyle().size16.w600.white,
                                ),
                              ),
                            ],
                          )),
                    )
                ],
              ),
            ),
    );
  }

  Widget _buildDescription(PlayerSettings settings) {
    return Positioned(
      bottom: 0,
      left: 0,
      child: Container(
          height: 15,
          width: MediaQuery.of(context).size.width,
          padding: const EdgeInsets.symmetric(horizontal: 4),
          color: Colors.black54,
          alignment: Alignment.center,
          child: Row(
            mainAxisSize: MainAxisSize.min,
            children: [
              Container(
                width: 8,
                height: 8,
                decoration: BoxDecoration(
                  shape: BoxShape.circle,
                  color: settings.online
                      ?Colors.green
                      : Colors.red,
                ),
              ),
              const SizedBox(width: 8),
              Expanded(
                child: Text(
                  settings.description,
                  style: const TextStyle().black26.size11.w600,
                  overflow: TextOverflow.ellipsis,
                  maxLines: 1,
                ),
              ),
            ],
          )),
    );
  }
}
