import 'package:flutter/foundation.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:flutter_webrtc_example/src/player_panel/player_state.dart';
import 'package:turing_mobile/turing_foundation.dart';


extension RTCPeerConnectionStateToPlayerState on RTCPeerConnectionState {
  PlayerState get playerState {
    switch (this) {
      case RTCPeerConnectionState.RTCPeerConnectionStateFailed:
        return PlayerState.error;
      case RTCPeerConnectionState.RTCPeerConnectionStateClosed:
      case RTCPeerConnectionState.RTCPeerConnectionStateConnected:
      case RTCPeerConnectionState.RTCPeerConnectionStateConnecting:
      case RTCPeerConnectionState.RTCPeerConnectionStateDisconnected:
      case RTCPeerConnectionState.RTCPeerConnectionStateNew:
        return PlayerState.connecting;
    }
  }

  bool get temporaryDisconnected =>
      this == RTCPeerConnectionState.RTCPeerConnectionStateConnected ||
      this == RTCPeerConnectionState.RTCPeerConnectionStateDisconnected;
}

const webrtcConnectionError = TuringError(
  ErrorCode.socketException,
  'Connection Error',
  "Connection Error",
);

const webrtcReconnectTooManyTimes = TuringError(
  ErrorCode.other,
  'Reconnect too many times',
  "Stream",
);

enum WebrtcConnectionStep {
  startWebStream,
  startWebStreamSuccess,
  startWebStreamFailed,
  startMultiLiveStream,
  peerConnectionSuccess,
  peerConnectionFailed,
  onAddRemoteStream,
  receiveFirstPacket,
  receiveFirstRender,
  streamTimeout,
  reconnectTooManyTimes;

  bool get isFailed =>
      this == startWebStreamFailed ||
      this == peerConnectionFailed ||
      this == streamTimeout ||
      this == reconnectTooManyTimes;
}

typedef WebrtcConnectionStepCallback = void Function(WebrtcConnectionStep step);
typedef WebrtcConnectionStateCallback = void Function(
    RTCPeerConnectionState state);

class AlwaysValueNotifier<T> extends ChangeNotifier
    implements ValueListenable<T> {
  /// Creates a [ChangeNotifier] that wraps this value.
  AlwaysValueNotifier(this._value);

  /// The current value stored in this notifier.
  ///
  /// When the value is replaced with something that is not equal to the old
  /// value as evaluated by the equality operator ==, this class notifies its
  /// listeners.
  @override
  T get value => _value;
  T _value;

  set value(T newValue) {
    _value = newValue;
    notifyListeners();
  }

  void setValueWithoutNotifier(T newValue) {
    _value = newValue;
  }

  @override
  String toString() => '${describeIdentity(this)}($value)';
}

enum WebrtcResolution { hd, sd, normal }
