class WebrtcPlayerMetrics {
  final int? cameraId;
  final String? streamId;
  final String? type; // h264 or h265
  final String? player; // web or native
  final int? bitRate;
  final int? fps;
  final int? interval;
  final int? width;
  final int? height;
  final int? smartEncode; // 0: disable 1: enable
  final int? startTimestamp;
  final int? wsTimestamp; // websocket success
  final int? sdpTimestamp; // peer connection for sdp success
  final int? firstPacketTimestamp; // receive first packet
  final int? firstFrameTimestamp; // first frame rendered

  WebrtcPlayerMetrics({
    this.cameraId,
    this.streamId = '',
    this.type = 'h264',
    this.player = 'native',
    this.bitRate = 1000,
    this.fps = 15,
    this.interval = 30,
    this.width = 1280,
    this.height = 720,
    this.smartEncode = 0,
    this.startTimestamp = -1,
    this.wsTimestamp = -1,
    this.sdpTimestamp = -1,
    this.firstPacketTimestamp = -1,
    this.firstFrameTimestamp = -1,
  });

  WebrtcPlayerMetrics copyWith({
    int? cameraId,
    String? streamId,
    String? type,
    String? player,
    int? bitRate,
    int? fps,
    int? interval,
    int? width,
    int? height,
    int? smartEncode,
    int? startTimestamp,
    int? wsTimestamp,
    int? sdpTimestamp,
    int? firstPacketTimestamp,
    int? firstFrameTimestamp,
  }) {
    return WebrtcPlayerMetrics(
      cameraId: cameraId ?? this.cameraId,
      streamId: streamId ?? this.streamId,
      type: type ?? this.type,
      player: player ?? this.player,
      bitRate: bitRate ?? this.bitRate,
      fps: fps ?? this.fps,
      interval: interval ?? this.interval,
      width: width ?? this.width,
      height: height ?? this.height,
      smartEncode: smartEncode ?? this.smartEncode,
      startTimestamp: startTimestamp ?? this.startTimestamp,
      wsTimestamp: wsTimestamp ?? this.wsTimestamp,
      sdpTimestamp: sdpTimestamp ?? this.sdpTimestamp,
      firstPacketTimestamp: firstPacketTimestamp ?? this.firstPacketTimestamp,
      firstFrameTimestamp: firstFrameTimestamp ?? this.firstFrameTimestamp,
    );
  }

  Map<String, dynamic> toJson() {
    assert(cameraId != null);
    assert(streamId != null);
    assert(type != null);
    assert(player != null);
    assert(bitRate != null);
    assert(fps != null);
    assert(interval != null);
    assert(width != null);
    assert(height != null);
    assert(smartEncode != null);

    return {
      'camera_id': cameraId,
      'stream_id': streamId,
      'type': type,
      'player': player,
      'bitrate': bitRate,
      'fps': fps,
      'interval': interval,
      'width': width,
      'height': height,
      'smart_encode': smartEncode,
      'start_time': startTimestamp ?? -1,
      'ws_time': wsTimestamp ?? -1,
      'sdp_time': sdpTimestamp ?? -1,
      'first_packet': firstPacketTimestamp ?? -1,
      'first_frame': firstFrameTimestamp ?? -1,
    };
  }
}
