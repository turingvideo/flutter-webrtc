import 'package:turing_mobile/turing_foundation.dart';

import 'custom_error.dart';

const int streamDefault = 2000;
const int streamNoRecord = 2001;
const int streamNoHistory = 2002;
const int streamCannotPause = 2003;
const int streamCameraNotExist = 2004;
const int streamBoxLimitResource = 2005;
const int streamCannotSwitchSpeedInLive = 2006;
const int streamDefaultLive = 2007;
const int streamDefaultPlayback = 2008;
const int streamCannotSwitchSpeed = 2009;
const int streamDisconnect = 2010;
const int streamPaused = 2011;

const streamDefaultLiveError = TuringError(
  streamDefaultLive,
  'Failed to play the live stream, please retry.',
  streamDebugMessage,
);

const streamNoHistoryError = TuringError(
  streamNoHistory,
  'No recording history found.',
  streamDebugMessage,
);

const streamNoRecordError = TuringError(
  streamNoRecord,
  'No history at this moment.',
  streamDebugMessage,
);

const streamNoCloudRecordError = TuringError(
  streamNoRecord,
  'No history at this moment.\n\nMotion and AI events on “Cloud Storage” may still be displayed but cannot be played if there is no recording.',
  streamDebugMessage,
);

const streamDefaultPlaybackError = TuringError(
  streamDefaultPlayback,
  'Failed to play the playback.',
  streamDebugMessage,
);

const streamCannotPauseError = TuringError(
  streamCannotPause,
  'Failed to pause live stream.',
  streamDebugMessage,
);

const streamCameraNotExistError = TuringError(
  streamCameraNotExist,
  'This camera has been disconnected from your account.',
  streamDebugMessage,
);

const streamBoxLimitResourceError = TuringError(
  streamBoxLimitResource,
  'There are a lot of requests and the system is busy. You can try again later or disconnect some streaming to free up resources.',
  streamDebugMessage,
);

const streamCannotSwitchSpeedInLiveError = TuringError(
  streamCannotSwitchSpeedInLive,
  'Can not switch speed during Live View',
  streamDebugMessage,
);

const streamCannotSwitchSpeedWsError = TuringError(
  streamCannotSwitchSpeed,
  'The camera may be malfunctioning. Please contact us for a service appointment.',
  streamDebugMessage,
);

const streamDisconnectError = TuringError(
  streamDisconnect,
  'The connection has been disconnected. Please reconnect it',
  streamDebugMessage,
);

const streamPausedError = TuringError(
  streamPaused,
  'paused',
  streamDebugMessage,
);
