import 'dart:async';
import 'dart:collection';
import 'dart:convert';
import 'dart:ffi';
import 'dart:io';
import 'dart:ui';

import 'package:flutter_webrtc/flutter_webrtc.dart' as webrtc;
import 'package:rxdart/rxdart.dart';
import 'package:turing_mobile/turing_foundation.dart';

final client = HttpClient()..badCertificateCallback = (_, __, ___) => true;

class WebRTCPlayer {
  Function(webrtc.MediaStream stream)? _onRemoteStream;
  webrtc.RTCPeerConnection? _pc;
  Function(webrtc.RTCPeerConnectionState state)? _onConnectionState;
  VoidCallback? _onReceiveFirstPacket;
  Timer? _getStatsTimer;

  /// When got a remote stream.
  set onRemoteStream(Function(webrtc.MediaStream stream) v) =>
      _onRemoteStream = v;

  set onConnectionState(Function(webrtc.RTCPeerConnectionState state) s) =>
      _onConnectionState = s;

  set onReceiveFirstPacket(VoidCallback f) => _onReceiveFirstPacket = f;

  /// Initialize the player.
  void initState() {}

  List<webrtc.MediaStream?>? get remoteStreams => _pc?.getRemoteStreams();
  webrtc.MediaStream? getRemoteStream(String streamId) =>
      remoteStreams?.firstWhere((element) => element?.id == streamId);

  Future<List<webrtc.StatsReport>>? getStats(
          [webrtc.MediaStreamTrack? track]) =>
      _pc?.getStats(track);

  /// Start play a url.
  /// [url] must a path parsed by [WebRTCUri.parse] in https://github.com/rtcdn/rtcdn-draft
  Future<void> play(String url) async {
    final urls = url.split('?');

    final testUrl = urls.first;

    try {
      _getStatsTimer?.cancel();
      _getStatsTimer = null;

      await _pc?.close();
      _pc = null;

      _pc = await webrtc.createPeerConnection({
        // AddTransceiver is only available with Unified Plan SdpSemantics
        'sdpSemantics': "unified-plan"
      });

      if (_pc == null) {
        _onConnectionState
            ?.call(webrtc.RTCPeerConnectionState.RTCPeerConnectionStateFailed);
        return;
      }

      _pc!.onConnectionState = (webrtc.RTCPeerConnectionState state) {
        _onConnectionState?.call(state);
        if (state ==
            webrtc.RTCPeerConnectionState.RTCPeerConnectionStateDisconnected) {
          _getStatsTimer?.cancel();
          _getStatsTimer = null;
        }
      };

      _pc!.onIceConnectionState = (webrtc.RTCIceConnectionState state) {};

      _pc!.onAddStream = (webrtc.MediaStream stream) {
        _onRemoteStream?.call(stream);

        final video = stream.getVideoTracks().firstObject;

        if (video != null) {
          _getStatsTimer = Timer.periodic(
            const Duration(milliseconds: 100),
            (timer) async {
              final reports = await _pc?.getStats(video) ?? [];

              if (reports.isNotEmpty) {
                final bytesReceived = reports
                        .firstObjectWhere((e) => e.type == 'inbound-rtp')
                        ?.values['bytesReceived'] ??
                    0;

                // info(
                //   'bytesReceived : $bytesReceived, ${timer.tick}',
                // );

                if (bytesReceived > 0) {
                  _onReceiveFirstPacket?.call();
                  timer.cancel();
                }
              }
            },
          );
        }
      };

      _pc!.onAddTrack =
          (webrtc.MediaStream stream, webrtc.MediaStreamTrack track) {};
      // Create the peer connection.

      // Setup the peer connection.

      // info('WebRTC: Setup PC done, A|V RecvOnly');

      // Start SDP handshake.

      _pc!.addTransceiver(
        kind: webrtc.RTCRtpMediaType.RTCRtpMediaTypeAudio,
        init: webrtc.RTCRtpTransceiverInit(
            direction: webrtc.TransceiverDirection.RecvOnly),
      );

      _pc!.addTransceiver(
        kind: webrtc.RTCRtpMediaType.RTCRtpMediaTypeVideo,
        init: webrtc.RTCRtpTransceiverInit(
          direction: webrtc.TransceiverDirection.RecvOnly,
        ),
      );

      final offer = await _pc!.createOffer({
        'mandatory': {'OfferToReceiveAudio': true, 'OfferToReceiveVideo': true},
      });

      if (testUrl.isNotEmpty) {
        await _pc!.setLocalDescription(offer);
        // info(
        //     'WebRTC: createOffer, ${offer.type} is ${offer.sdp?.replaceAll('\n', '\\n').replaceAll('\r', '\\r')}');

        HttpClass.instance.desMapChanged.listen((value) async {
          final des = value[url];

          if (des == null) {
            info('RTCSessionDescription null');
          } else {
// info('WebRTC: got answer ${answer.type} is ${answer.sdp?.replaceAll('\n', '\\n').replaceAll('\r', '\\r')}');
            info('des: $des');
            await _pc!.setRemoteDescription(des);

            // info("set remote description success");

            _onConnectionState?.call(
              webrtc.RTCPeerConnectionState.RTCPeerConnectionStateConnecting,
            );
          }
        });

        HttpClass.instance.add(url, offer.sdp);
      }
    } catch (e) {
      error('$e', tag: 'webrtc player connection error');

      _onConnectionState
          ?.call(webrtc.RTCPeerConnectionState.RTCPeerConnectionStateFailed);
      _getStatsTimer?.cancel();
      _getStatsTimer = null;
      rethrow;
    }
  }

  /// Handshake to exchange SDP, send offer and got answer.
  // Future<webrtc.RTCSessionDescription> _handshake(
  //   String url,
  //   String? offer,
  // ) async {
  //   try {
  //     // Allow self-sign certificate, see https://api.flutter.dev/flutter/dart-io/HttpClient/badCertificateCallback.html

  //     debug('url: $url');

  //     // Parsing the WebRTC uri form url.
  //     final webRTCUri = _WebRTCUri.parse(url);

  //     debug(
  //       'webRTCUri api: ${webRTCUri.api}\nwebRTCUri streamurl: ${webRTCUri.streamUrl}',
  //     );

  //     // Do signaling for WebRTC.
  //     // @see https://github.com/rtcdn/rtcdn-draft
  //     //
  //     // POST http://d.ossrs.net:11985/rtc/v1/play/
  //     //    {api: "xxx", sdp: "offer", streamurl: "webrtc://d.ossrs.net:11985/live/livestream"}
  //     // Response:
  //     //    {code: 0, sdp: "answer", sessionid: "007r51l7:X2Lv"}

  //     final uri = Uri.parse(webRTCUri.api);

  //     debug('postUrl uri: $uri');

  //     debug('client Object: ${client.hashCode}');

  //     final req = await client.postUrl(uri);

  //     req.headers.set(
  //       'Content-Type',
  //       'application/json',
  //     );

  //     req.add(
  //       utf8.encode(
  //         json.encode(
  //           {
  //             'api': webRTCUri.api,
  //             'streamurl': webRTCUri.streamUrl,
  //             'sdp': offer,
  //           },
  //         ),
  //       ),
  //     );

  //     final res = await req.close();

  //     final stringData = await res.transform(utf8.decoder).join();

  //     debug('stringData: $stringData');

  //     Map<String, dynamic> o = json.decode(stringData);

  //     // info('WebRTC reply: ${o.toString()}');

  //     if (!o.containsKey('code') || !o.containsKey('sdp') || o['code'] != 0) {
  //       throw Future.error(stringData);
  //     }

  //     return Future.value(webrtc.RTCSessionDescription(o['sdp'], 'answer'));
  //   } catch (err) {
  //     error('$err', tag: '_handshake catch');

  //     rethrow;
  //   } finally {
  //     client.close();
  //   }
  // }

  /// Dispose the player.
  void dispose() {
    _onRemoteStream = null;
    _onConnectionState = null;
    _onReceiveFirstPacket = null;
    _getStatsTimer?.cancel();
    _getStatsTimer = null;
    _pc?.close();
    _pc = null;
  }
}

class _WebRTCUri {
  /// The api server url for WebRTC streaming.
  String api;

  /// The stream url to play or publish.
  String streamUrl;

  _WebRTCUri({
    required this.api,
    required this.streamUrl,
  });

  /// Parse the url to WebRTC uri.
  static _WebRTCUri parse(String url) {
    Uri uri = Uri.parse(url);

    var schema = 'https'; // For native, default to HTTPS
    if (uri.queryParameters.containsKey('schema')) {
      schema = uri.queryParameters['schema']!;
    }

    var port = (uri.port > 0) ? uri.port : 443;
    if (schema == 'https') {
      port = (uri.port > 0) ? uri.port : 443;
    } else if (schema == 'http') {
      port = (uri.port > 0) ? uri.port : 1985;
    }

    var api = '/rtc/v1/play/';
    if (uri.queryParameters.containsKey('play')) {
      api = uri.queryParameters['play']!;
    }

    var apiParams = [];
    for (var key in uri.queryParameters.keys) {
      if (key != 'api' && key != 'play' && key != 'schema') {
        apiParams.add('$key=${uri.queryParameters[key]}');
      }
    }

    var apiUrl = '$schema://${uri.host}:$port$api';
    if (apiParams.isNotEmpty) {
      apiUrl += '?${apiParams.join('&')}';
    }

    final r = _WebRTCUri(api: apiUrl, streamUrl: url);

    return r;
  }
}

class HttpClass {
  HttpClass._() {
    _subscription = queueChanged.listen((_) {
      executeQueue();
    });
  }

  StreamSubscription? _subscription;

  static final instance = HttpClass._();

  final queue = <Map<String, String?>>[];

  final queueChanged = PublishSubject<void>();

  bool isExecute = false;

  final desMap = <String, webrtc.RTCSessionDescription?>{};

  final desMapChanged =
      PublishSubject<Map<String, webrtc.RTCSessionDescription?>>();

  void add(String url, String? offer) async {
    queue.add({url: offer});

    queueChanged.add(null);
  }

  Future<webrtc.RTCSessionDescription?> executeQueue() async {
    if (queue.isEmpty || isExecute == true) {
      return Future.value(null);
    }

    isExecute = true;

    final url = queue.first.keys.first;
    final offer = queue.first.values.first;

    webrtc.RTCSessionDescription? des;

    try {
      des = await _handshake(url, offer);
    } catch (e) {
      error('_handshake cache: $e');
    }

    desMap.addAll({url: des});

    desMapChanged.add({url: des});

    isExecute = false;

    queue.removeAt(0);

    queueChanged.add(null);

    return des;
  }

  Future<webrtc.RTCSessionDescription> _handshake(
    String url,
    String? offer,
  ) async {
    try {
      // Allow self-sign certificate, see https://api.flutter.dev/flutter/dart-io/HttpClient/badCertificateCallback.html

      debug('url: $url', tag: 'in _handshake');

      // Parsing the WebRTC uri form url.
      final webRTCUri = _WebRTCUri.parse(url);

      debug(
        'webRTCUri api: ${webRTCUri.api}\nwebRTCUri streamurl: ${webRTCUri.streamUrl}',
      );

      // Do signaling for WebRTC.
      // @see https://github.com/rtcdn/rtcdn-draft
      //
      // POST http://d.ossrs.net:11985/rtc/v1/play/
      //    {api: "xxx", sdp: "offer", streamurl: "webrtc://d.ossrs.net:11985/live/livestream"}
      // Response:
      //    {code: 0, sdp: "answer", sessionid: "007r51l7:X2Lv"}

      final uri = Uri.parse(webRTCUri.api);

      debug('postUrl uri: $uri');

      debug('client Object: ${client.hashCode}');

      final req = await client.postUrl(uri);

      req.headers.set(
        'Content-Type',
        'application/json',
      );

      req.add(
        utf8.encode(
          json.encode(
            {
              'api': webRTCUri.api,
              'streamurl': webRTCUri.streamUrl,
              'sdp': offer,
            },
          ),
        ),
      );

      final res = await req.close();

      final stringData = await res.transform(utf8.decoder).join();

      debug('stringData: $stringData');

      Map<String, dynamic> o = json.decode(stringData);

      // info('WebRTC reply: ${o.toString()}');

      if (!o.containsKey('code') || !o.containsKey('sdp') || o['code'] != 0) {
        throw Future.error(stringData);
      }

      return Future.value(webrtc.RTCSessionDescription(o['sdp'], 'answer'));
    } catch (err) {
      error('$err', tag: '_handshake catch');
      rethrow;
    } finally {}
  }
}
