import 'package:flutter/material.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_settings.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_state.dart';


typedef PlayerSpeedCallback = void Function(PlayerSpeed);

abstract class PlayerViewWidget extends StatelessWidget {
  final PlayerSettings settings;

  final VoidCallback? onTapPlay;
  final VoidCallback? onTapFullScreen;
  final VoidCallback? onTapPtz;
  final VoidCallback? onTapRecord;
  final VoidCallback? onTapAudio;
  final VoidCallback? onTapHd;
  final PlayerSpeedCallback? onTapSpeed;
  final VoidCallback? onTapRewind;
  final VoidCallback? onTapForward;
  final VoidCallback? onTapPlayback;
  final VoidCallback? onTapShare;
  final VoidCallback? onTapGrid;
  final VoidCallback? onTapLive;
  final VoidCallback? onTapArchive;
  final VoidCallback? onTapDownload;

  final ValueNotifier<bool>? hd;
  final ValueNotifier<bool>? fullscreen;
  final ValueNotifier<bool>? record;
  final ValueNotifier<bool>? audio;
  final ValueNotifier<PlayerSpeed>? speed;
  final ValueNotifier<bool>? live;
  final ValueNotifier<PlayerState>? state;
  final ValueNotifier<String>? description;

  const PlayerViewWidget({
    super.key,
    required this.settings,
    this.onTapPlay,
    this.onTapFullScreen,
    this.onTapPtz,
    this.onTapRecord,
    this.onTapAudio,
    this.onTapHd,
    this.onTapSpeed,
    this.onTapRewind,
    this.onTapForward,
    this.onTapShare,
    this.onTapGrid,
    this.onTapLive,
    this.onTapArchive,
    this.onTapDownload,
    this.onTapPlayback,
    this.hd,
    this.fullscreen,
    this.record,
    this.audio,
    this.speed,
    this.live,
    this.state,
    this.description,
  });
}
