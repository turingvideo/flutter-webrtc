import 'dart:async';
import 'dart:convert';
import 'dart:io';
import 'dart:ui';

import 'package:flutter_webrtc/flutter_webrtc.dart' as webrtc;
import 'package:turing_mobile/turing_foundation.dart';

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
    _getStatsTimer?.cancel();
    _getStatsTimer = null;

    try {
      await _pc?.close();
      _pc = null;

      // Create the peer connection.
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

      // Setup the peer connection.
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

                info(
                  'bytesReceived : $bytesReceived, ${timer.tick}',
                );

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
      // info('WebRTC: Setup PC done, A|V RecvOnly');

      // Start SDP handshake.
      webrtc.RTCSessionDescription offer = await _pc!.createOffer({
        'mandatory': {'OfferToReceiveAudio': true, 'OfferToReceiveVideo': true},
      });
      await _pc!.setLocalDescription(offer);
      // info(
      //     'WebRTC: createOffer, ${offer.type} is ${offer.sdp?.replaceAll('\n', '\\n').replaceAll('\r', '\\r')}');

      webrtc.RTCSessionDescription answer = await _handshake(url, offer.sdp);

      // info(
      //     'WebRTC: got answer ${answer.type} is ${answer.sdp?.replaceAll('\n', '\\n').replaceAll('\r', '\\r')}');

      await _pc!.setRemoteDescription(answer);
      // info("set remote description success");
      _onConnectionState?.call(
          webrtc.RTCPeerConnectionState.RTCPeerConnectionStateConnecting);
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
  Future<webrtc.RTCSessionDescription> _handshake(
    String url,
    String? offer,
  ) async {
    // Setup the client for HTTP or HTTPS.
    final client = HttpClient();

    try {
      // Allow self-sign certificate, see https://api.flutter.dev/flutter/dart-io/HttpClient/badCertificateCallback.html
      client.badCertificateCallback = (
        X509Certificate cert,
        String host,
        int port,
      ) =>
          true;

      // Parsing the WebRTC uri form url.
      final webRTCUri = _WebRTCUri.parse(url);

      // Do signaling for WebRTC.
      // @see https://github.com/rtcdn/rtcdn-draft
      //
      // POST http://d.ossrs.net:11985/rtc/v1/play/
      //    {api: "xxx", sdp: "offer", streamurl: "webrtc://d.ossrs.net:11985/live/livestream"}
      // Response:
      //    {code: 0, sdp: "answer", sessionid: "007r51l7:X2Lv"}

      final uri = Uri.parse(webRTCUri.api);

      debug(
          'url: $url\nwebRTCUri api: ${webRTCUri.api}\nuri: ${uri.toString()}');

      debug(
        'api: ${webRTCUri.api}\nstreamurl: ${webRTCUri.streamUrl}\nsdp: \n$offer',
      );

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
      // info('WebRTC request: ${uri.api} offer=${offer?.length}B');

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
    } finally {
      client.close();
    }
  }

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

    _WebRTCUri r = _WebRTCUri(api: apiUrl, streamUrl: url);
    verbose('Url:$url\napi:${r.api}\nstream:${r.streamUrl}');
    return r;
  }
}
