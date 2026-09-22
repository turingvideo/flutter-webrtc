import 'package:flutter/material.dart';
import 'package:flutter_webrtc/flutter_webrtc.dart';

/// Minimal test harness for MediaSdkFisheyePlayer's bring-up spike (see
/// fisheye/README.md and MediaSdkFisheyePlayer's class doc in the plugin):
/// give it an RTSP URL, watch whether corrected-YUV frame events actually
/// arrive. No video rendering -- that path doesn't exist yet.
class MediaSdkFisheyeSample extends StatefulWidget {
  static String tag = 'media_sdk_fisheye_sample';

  @override
  _MediaSdkFisheyeSampleState createState() => _MediaSdkFisheyeSampleState();
}

class _MediaSdkFisheyeSampleState extends State<MediaSdkFisheyeSample> {
  final _urlController = TextEditingController(
    text: 'rtsp://admin:NeSmZ8mfqX@192.168.31.33:554/live1s2.sdp',
  );
  final MediaSdkFisheyePlayer _player = MediaSdkFisheyePlayer();

  String _status = 'idle';
  MediaSdkFisheyeFrameEvent? _lastFrame;
  int _frameEventCount = 0;
  Object? _error;

  @override
  void initState() {
    super.initState();
    _player.onFrame.stream.listen((event) {
      setState(() {
        _lastFrame = event;
        _frameEventCount++;
      });
    });
  }

  Future<void> _start() async {
    setState(() {
      _status = 'starting...';
      _error = null;
      _frameEventCount = 0;
      _lastFrame = null;
    });
    try {
      await _player.start(_urlController.text);
      setState(() => _status = 'start() returned -- waiting for frames');
    } catch (e) {
      setState(() {
        _status = 'start() failed';
        _error = e;
      });
    }
  }

  Future<void> _stop() async {
    await _player.stop();
    setState(() => _status = 'stopped');
  }

  @override
  void dispose() {
    _player.dispose();
    _urlController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text('MediaSDK Fisheye (spike)')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            TextField(
              controller: _urlController,
              decoration: InputDecoration(labelText: 'RTSP URL'),
            ),
            const SizedBox(height: 12),
            Row(
              children: [
                ElevatedButton(onPressed: _start, child: Text('Start')),
                const SizedBox(width: 12),
                ElevatedButton(onPressed: _stop, child: Text('Stop')),
              ],
            ),
            const SizedBox(height: 20),
            Text('status: $_status'),
            const SizedBox(height: 8),
            Text('frame events received: $_frameEventCount'),
            const SizedBox(height: 8),
            if (_lastFrame != null) Text('last frame: $_lastFrame'),
            if (_error != null)
              Padding(
                padding: const EdgeInsets.only(top: 12),
                child: Text(
                  'error: $_error',
                  style: TextStyle(color: Colors.red),
                ),
              ),
            const SizedBox(height: 20),
            Text(
              'No video rendering yet -- this only proves whether the '
              'native pipeline (RTSP in -> MediaSDK decode+correct -> YUV '
              'callback) actually runs. Watch adb logcat filtered on '
              '"MediaSdkFisheyePlayer" for the native-side step-by-step '
              'return codes.',
              style: TextStyle(color: Colors.grey),
            ),
          ],
        ),
      ),
    );
  }
}
