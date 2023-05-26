import 'package:flutter/material.dart';
import 'package:flutter_webrtc_example/src/player/player_panel/player_live_view.dart';

import 'player/player_panel/player_settings.dart';
import 'player/player_panel/webrtc_player_panel.dart';
import 'player/webrtc_player_controller.dart';
import 'player/webrtc_player_controls.dart';
import 'player/webrtc_player_state.dart';

const streamUrls = <String>[
  'webrtc://test-srs-2.turingvideo.com/live/21328/1b9977cf6fb4516c02188f0208b9efbf',
  'webrtc://test-srs-2.turingvideo.com/live/21316/0a87fa37d17d3f906af841d16e5d821c',
  'webrtc://test-srs-2.turingvideo.com/live/21327/afba649615e515e3d8105d2b48c41d51',
  'webrtc://test-srs-2.turingvideo.com/live/21318/f18842edf3576b9c733fc42d63c565c3',
  'webrtc://test-srs-2.turingvideo.com/live/21314/122e006caf453a6cbd5db4f47f6d4d53',
  'webrtc://test-srs-2.turingvideo.com/live/21315/32724c702e5f87b1d1acaa2529f34a46',
];

class MultiRTCStream extends StatefulWidget {
  const MultiRTCStream({super.key});

  @override
  State<MultiRTCStream> createState() => _MultiRTCStreamState();
}

class _MultiRTCStreamState extends State<MultiRTCStream> {
  WebRTCPlayerController playerController1 = WebRTCPlayerController();
  WebRTCPlayerController playerController2 = WebRTCPlayerController();
  WebRTCPlayerController playerController3 = WebRTCPlayerController();
  WebRTCPlayerController playerController4 = WebRTCPlayerController();
  WebRTCPlayerController playerController5 = WebRTCPlayerController();
  WebRTCPlayerController playerController6 = WebRTCPlayerController();

  void play() {
    playerController1.setDataSource(url: streamUrls[0]);
    playerController2.setDataSource(url: streamUrls[1]);
    playerController3.setDataSource(url: streamUrls[2]);
    playerController4.setDataSource(url: streamUrls[3]);
    playerController5.setDataSource(url: streamUrls[4]);
    playerController6.setDataSource(url: streamUrls[5]);
  }

  @override
  void dispose() {
    playerController1.dispose();
    playerController2.dispose();
    playerController3.dispose();
    playerController4.dispose();
    playerController5.dispose();
    playerController6.dispose();

    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    // final w = MediaQuery.of(context).size.width / 2.0 - 10.0;
    // final w = MediaQuery.of(context).size.width - 10.0;
    final w = 375 - 10.0;
    final h = w * 9.0 / 16.0;

    return Scaffold(
      appBar: AppBar(
        title: Text('Multi RTC Stream'),
      ),
      body: SingleChildScrollView(
        child: Column(
          // mainAxisSize: MainAxisSize.min,
          children: [
            ElevatedButton(
              onPressed: () {
                play();
              },
              child: Text('Play'),
            ),
            for (var url in streamUrls)
              Text(
                url,
                style: TextStyle(fontSize: 11),
                maxLines: 100,
              ),
            // Wrap(
            //   children: [
            SizedBox(
              width: w,
              height: h,
              child: WebRTCPlayerControls(
                code: WebrtcCodeType.h264,
                controller: playerController1,
                panelBuilder: (context, rect) {
                  return WebrtcPlayerPanel(
                    controller: playerController1,
                    texturePos: rect,
                    settings: PlayerSettings.live(),
                    builder: (context) => MultiRTCView(w, h),
                  );
                },
              ),
            ),
            SizedBox(
              width: w,
              height: h,
              child: WebRTCPlayerControls(
                code: WebrtcCodeType.h264,
                controller: playerController2,
                panelBuilder: (context, rect) {
                  return WebrtcPlayerPanel(
                    controller: playerController2,
                    texturePos: rect,
                    settings: PlayerSettings.live(),
                    builder: (context) => MultiRTCView(w, h),
                  );
                },
              ),
            ),
            SizedBox(
              width: w,
              height: h,
              child: WebRTCPlayerControls(
                code: WebrtcCodeType.h264,
                controller: playerController3,
                panelBuilder: (context, rect) {
                  return WebrtcPlayerPanel(
                    controller: playerController3,
                    texturePos: rect,
                    settings: PlayerSettings.live(),
                    builder: (context) => MultiRTCView(w, h),
                  );
                },
              ),
            ),
            SizedBox(
              width: w,
              height: h,
              child: WebRTCPlayerControls(
                code: WebrtcCodeType.h264,
                controller: playerController4,
                panelBuilder: (context, rect) {
                  return WebrtcPlayerPanel(
                    controller: playerController4,
                    texturePos: rect,
                    settings: PlayerSettings.live(),
                    builder: (context) => MultiRTCView(w, h),
                  );
                },
              ),
            ),
            SizedBox(
              width: w,
              height: h,
              child: WebRTCPlayerControls(
                code: WebrtcCodeType.h264,
                controller: playerController5,
                panelBuilder: (context, rect) {
                  return WebrtcPlayerPanel(
                    controller: playerController5,
                    texturePos: rect,
                    settings: PlayerSettings.live(),
                    builder: (context) => MultiRTCView(w, h),
                  );
                },
              ),
            ),
            SizedBox(
              width: w,
              height: h,
              child: WebRTCPlayerControls(
                code: WebrtcCodeType.h264,
                controller: playerController6,
                panelBuilder: (context, rect) {
                  return WebrtcPlayerPanel(
                    controller: playerController6,
                    texturePos: rect,
                    settings: PlayerSettings.live(),
                    builder: (context) => MultiRTCView(w, h),
                  );
                },
              ),
            ),
          ],
        ),
        //   ],
        // ),
      ),
    );
  }
}
