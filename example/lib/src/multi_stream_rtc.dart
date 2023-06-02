import 'package:flutter/material.dart';
import 'package:flutter_webrtc_example/src/player/player_panel/player_live_view.dart';

import 'player/player_panel/player_settings.dart';
import 'player/player_panel/webrtc_player_panel.dart';
import 'player/webrtc_player_controller.dart';
import 'player/webrtc_player_controls.dart';
import 'player/webrtc_player_state.dart';

const streamUrls = <String>[
  'webrtc://dev-stream-go.turingvideo.com/live/8711/4a3c2b1888122c5dbb8d1342b5f27345',
  'webrtc://dev-stream-go.turingvideo.com/live/8719/0d4e10051b8b48d0ee8b8e723ffb574f',
  'webrtc://dev-stream-go.turingvideo.com/live/8706/517de793bc4e41abbe0346c42c4c184f',
  'webrtc://dev-stream-go.turingvideo.com/live/8714/d9ee0642175d064c108c6035d238a580',
  'webrtc://dev-stream-go.turingvideo.com/live/8717/e054fbec22fd46cc03d1491b652b3888',
  'webrtc://dev-stream-go.turingvideo.com/live/8713/024b347d5573384ceba126d35048a523',
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

  // WebRTCPlayerController playerController7 = WebRTCPlayerController();

  // WebRTCPlayerController playerController8 = WebRTCPlayerController();
  // WebRTCPlayerController playerController9 = WebRTCPlayerController();
  // WebRTCPlayerController playerController10 = WebRTCPlayerController();
  // WebRTCPlayerController playerController11 = WebRTCPlayerController();
  // WebRTCPlayerController playerController12 = WebRTCPlayerController();
  // WebRTCPlayerController playerController13 = WebRTCPlayerController();
  // WebRTCPlayerController playerController14 = WebRTCPlayerController();
  // WebRTCPlayerController playerController15 = WebRTCPlayerController();
  // WebRTCPlayerController playerController16 = WebRTCPlayerController();

  void play() {
    // playerController7.setDataSource(url: streamUrls[6]);

    playerController1.setDataSource(url: streamUrls[0]);
    playerController2.setDataSource(url: streamUrls[1]);
    playerController3.setDataSource(url: streamUrls[2]);
    playerController4.setDataSource(url: streamUrls[3]);
    playerController5.setDataSource(url: streamUrls[4]);
    playerController6.setDataSource(url: streamUrls[5]);

    // playerController8.setDataSource(url: streamUrls[7]);
    // playerController9.setDataSource(url: streamUrls[8]);
    // playerController10.setDataSource(url: streamUrls[9]);
    // playerController11.setDataSource(url: streamUrls[10]);
    // playerController12.setDataSource(url: streamUrls[11]);
    // playerController13.setDataSource(url: streamUrls[12]);
    // playerController14.setDataSource(url: streamUrls[13]);
    // playerController15.setDataSource(url: streamUrls[14]);
    // playerController16.setDataSource(url: streamUrls[15]);
  }

  @override
  void dispose() {
    playerController1.dispose();
    playerController2.dispose();
    playerController3.dispose();
    playerController4.dispose();
    playerController5.dispose();
    playerController6.dispose();

    // playerController7.dispose();

    // playerController8.dispose();
    // playerController9.dispose();
    // playerController10.dispose();
    // playerController11.dispose();
    // playerController12.dispose();
    // playerController13.dispose();
    // playerController14.dispose();
    // playerController15.dispose();
    // playerController16.dispose();

    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final w = (MediaQuery.of(context).size.width - 2.0) / 2.0;
    // final w = MediaQuery.of(context).size.width - 10.0;
    // final w = (375.0 - 10.0) / 2.0;
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
                style: TextStyle(fontSize: 2),
                maxLines: 100,
              ),
            Wrap(
              spacing: 2,
              runSpacing: 2,
              children: [
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
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController7,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController7,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController8,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController8,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController9,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController9,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController10,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController11,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController12,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController13,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController14,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController15,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
                // SizedBox(
                //   width: w,
                //   height: h,
                //   child: WebRTCPlayerControls(
                //     code: WebrtcCodeType.h264,
                //     controller: playerController16,
                //     panelBuilder: (context, rect) {
                //       return WebrtcPlayerPanel(
                //         controller: playerController6,
                //         texturePos: rect,
                //         settings: PlayerSettings.live(),
                //         builder: (context) => MultiRTCView(w, h),
                //       );
                //     },
                //   ),
                // ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
