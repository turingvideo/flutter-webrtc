part of 'webrtc_player_cubit.dart';

class WebrtcPlayerState extends Equatable {
  final Status status; // for start_web_stream
  final String streamId; // id for stream

  final NestCamera? camera;

  final Status speedStatus;

  final Status audioStatus;
  final String audioUrl;
  final String audioStreamId;
  final Status audioHeartBeatStatus;

  final bool positionUpdate;

  const WebrtcPlayerState({
    this.status = const Status.idle(),
    this.camera,
    this.streamId = '',
    this.audioUrl = '',
    this.audioStatus = const Status.idle(),
    this.audioStreamId = '',
    this.audioHeartBeatStatus = const Status.idle(),
    this.positionUpdate = true,
    this.speedStatus = const Status.idle(),
  });

  WebrtcPlayerState copyWith({
    Status? status,
    NestCamera? camera,
    String? streamId,
    String? streamEncode,
    String? audioUrl,
    Status? audioStatus,
    String? audioStreamId,
    Status? audioHeartBeatStatus,
    bool? positionUpdate,
    Status? speedStatus,
  }) {
    return WebrtcPlayerState(
      status: status ?? this.status,
      camera: camera ?? this.camera,
      streamId: streamId ?? this.streamId,
      audioUrl: audioUrl ?? this.audioUrl,
      audioStatus: audioStatus ?? this.audioStatus,
      audioStreamId: audioStreamId ?? this.audioStreamId,
      audioHeartBeatStatus: audioHeartBeatStatus ?? this.audioHeartBeatStatus,
      positionUpdate: positionUpdate ?? this.positionUpdate,
      speedStatus: speedStatus ?? this.speedStatus,
    );
  }

  @override
  List<Object?> get props => [
        status,
        audioStatus,
        audioHeartBeatStatus,
        positionUpdate,
        speedStatus,
      ];
}
