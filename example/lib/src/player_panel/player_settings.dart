enum PlayerControlState {
  idle, // default is idle, will not display on UI.
  enabled, // could use
  disabled; // can't use

  bool get isAppear => this != PlayerControlState.idle; // show
  bool get isDisabled => this == PlayerControlState.disabled;

  factory PlayerControlState.fromCan(bool can) {
    return can ? PlayerControlState.enabled : PlayerControlState.disabled;
  }
}

class PlayerSettings {
  final PlayerControlState pause; // support play/pause live stream
  final PlayerControlState ptz; // support ptz
  final PlayerControlState hd; // support hd
  final PlayerControlState record; // support two way audio
  final PlayerControlState audio; // support audio
  final PlayerControlState fullscreen; // support full/exit screen
  final PlayerControlState playback; // support playback
  final PlayerControlState snapshot; // support snapshot
  final PlayerControlState video; // support video
  final PlayerControlState share; // support share

  final bool canAutoHidden; // support auto hidden panel or not
  final int autoHiddenInterval; // auto hidden panel timer interval

  final bool canDoubleTap; // support double tap

  final bool liveBadge; // show live badge or not

  final bool canHdStandby; // standby or not
  final int hdStandbyInterval; // standby time interval (seconds)

  final bool hasLicense; // has license

  final bool canBuildPanel; // build panel or not

  final String description; // description for stream. maybe camera name.

  final bool online; // online for device

  const PlayerSettings({
    this.pause = PlayerControlState.idle,
    this.ptz = PlayerControlState.idle,
    this.hd = PlayerControlState.idle,
    this.record = PlayerControlState.idle,
    this.audio = PlayerControlState.idle,
    this.fullscreen = PlayerControlState.idle,
    this.playback = PlayerControlState.idle,
    this.snapshot = PlayerControlState.idle,
    this.video = PlayerControlState.idle,
    this.share = PlayerControlState.idle,
    this.hasLicense = false,
    this.canAutoHidden = false,
    this.autoHiddenInterval = 3,
    this.canDoubleTap = false,
    this.liveBadge = false,
    this.canHdStandby = false,
    this.hdStandbyInterval = 270,
    this.canBuildPanel = true,
    this.description = '',
    this.online = true,
  });

  /// live
  factory PlayerSettings.live({
    bool canPause = false,
    bool canRecord = false,
    bool canAudio = false,
    bool canHd = false,
    bool canPtz = true,
    bool canPlayback = true,
    bool canShare = false,
    bool canFullscreen = true,
    bool canAutoHidden = false,
    bool canDoubleTap = false,
    bool canHdStandby = false,
    int hdStandbyInterval = 270,
    bool hasLicense = false,
    bool canBuildPanel = true,
    bool canSnapshot = true,
    String description = '',
    bool online = true,
  }) {
    return PlayerSettings(
      pause: PlayerControlState.fromCan(canPause),
      record: PlayerControlState.fromCan(canRecord),
      audio: PlayerControlState.fromCan(canAudio),
      hd: PlayerControlState.fromCan(canHd),
      ptz: PlayerControlState.fromCan(canPtz),
      playback: PlayerControlState.fromCan(canPlayback),
      share: PlayerControlState.fromCan(canShare),
      fullscreen: PlayerControlState.fromCan(canFullscreen),
      snapshot: PlayerControlState.fromCan(canSnapshot),
      canAutoHidden: canAutoHidden,
      canDoubleTap: canDoubleTap,
      canHdStandby: canHdStandby,
      hasLicense: hasLicense,
      canBuildPanel: canBuildPanel,
      description: description,
      online: online,
      hdStandbyInterval: hdStandbyInterval,
    );
  }

  /// copyWith
  PlayerSettings copyWith({
    PlayerControlState? pause,
    PlayerControlState? ptz,
    PlayerControlState? hd,
    PlayerControlState? record,
    PlayerControlState? audio,
    PlayerControlState? fullscreen,
    PlayerControlState? playback,
    PlayerControlState? snapshot,
    PlayerControlState? video,
    PlayerControlState? share,
    bool? hasLicense,
    bool? canAutoHidden,
    int? autoHiddenInterval,
    bool? canDoubleTap,
    bool? liveBadge,
    bool? canHdStandby,
    int? hdStandbyInterval,
    bool? canBuildPanel,
    String? description,
    bool? online,
  }) {
    return PlayerSettings(
      pause: pause ?? this.pause,
      ptz: ptz ?? this.ptz,
      hd: hd ?? this.hd,
      record: record ?? this.record,
      audio: audio ?? this.audio,
      fullscreen: fullscreen ?? this.fullscreen,
      playback: playback ?? this.playback,
      snapshot: snapshot ?? this.snapshot,
      video: video ?? this.video,
      share: share ?? this.share,
      hasLicense: hasLicense ?? this.hasLicense,
      canAutoHidden: canAutoHidden ?? this.canAutoHidden,
      canDoubleTap: canDoubleTap ?? this.canDoubleTap,
      autoHiddenInterval: autoHiddenInterval ?? this.autoHiddenInterval,
      liveBadge: liveBadge ?? this.liveBadge,
      canHdStandby: canHdStandby ?? this.canHdStandby,
      hdStandbyInterval: hdStandbyInterval ?? this.hdStandbyInterval,
      canBuildPanel: canBuildPanel ?? this.canBuildPanel,
      description: description ?? this.description,
      online: online ?? this.online,
    );
  }
}

/// playback
class PlayerPlaybackSettings extends PlayerSettings {
  final PlayerControlState speed; // support switch play speed
  final PlayerControlState forward; // support forward 2 minutes
  final PlayerControlState rewind; // support rewind 2 minutes
  final PlayerControlState download;
  final PlayerControlState archive;
  final PlayerControlState live;

  const PlayerPlaybackSettings({
    this.speed = PlayerControlState.idle,
    this.forward = PlayerControlState.idle,
    this.rewind = PlayerControlState.idle,
    this.download = PlayerControlState.idle,
    this.archive = PlayerControlState.idle,
    this.live = PlayerControlState.idle,
    super.pause = PlayerControlState.idle,
    super.ptz = PlayerControlState.idle,
    super.hd = PlayerControlState.idle,
    super.record = PlayerControlState.idle,
    super.audio = PlayerControlState.idle,
    super.fullscreen = PlayerControlState.idle,
    super.snapshot = PlayerControlState.idle,
    super.video = PlayerControlState.idle,
    super.share = PlayerControlState.idle,
    super.canAutoHidden = false,
    super.autoHiddenInterval = 3,
    super.canDoubleTap = false,
    super.liveBadge = false,
    super.canHdStandby = false,
    super.hdStandbyInterval = 270,
    super.hasLicense,
    super.canBuildPanel,
    super.description,
    super.online,
  });

  factory PlayerPlaybackSettings.playback({
    bool canAudio = false,
    bool canHd = false,
    bool canDownload = true,
    bool canArchive = true,
    bool canPause = true,
    bool canRewind = true,
    bool canForward = true,
    bool canSpeed = true,
    bool hasLicense = false,
    bool canAutoHidden = true,
    bool canBuildPanel = true,
    bool canSnapshot = true,
    String description = '',
    bool online = true,
    bool canHdStandby = false,
  }) {
    return PlayerPlaybackSettings(
      speed: PlayerControlState.fromCan(canSpeed),
      forward: PlayerControlState.fromCan(canForward),
      rewind: PlayerControlState.fromCan(canRewind),
      download: PlayerControlState.fromCan(canDownload),
      archive: PlayerControlState.fromCan(canArchive),
      pause: PlayerControlState.fromCan(canPause),
      audio: PlayerControlState.fromCan(canAudio),
      live: PlayerControlState.enabled,
      hd: PlayerControlState.fromCan(canHd),
      snapshot: PlayerControlState.fromCan(canSnapshot),
      hasLicense: hasLicense,
      canAutoHidden: canAutoHidden,
      canBuildPanel: canBuildPanel,
      description: description,
      online: online,
      canHdStandby: canHdStandby,
    );
  }
}
