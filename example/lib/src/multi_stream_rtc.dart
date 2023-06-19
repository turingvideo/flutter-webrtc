import 'package:flutter/material.dart';
import 'package:flutter_bloc/flutter_bloc.dart';
import 'package:flutter_webrtc_example/src/nest_camera.dart';

import 'package:flutter_webrtc_example/src/player_panel/player_settings.dart';

import 'package:flutter_webrtc_example/src/webrtc_player/webrtc_player_cubit.dart';
import 'package:turing_mobile/turing_network.dart';

import 'webrtc_player/webrtc_player_view.dart';

const streamUrls = <String>[
  // 'webrtc://dev-stream-go.turingvideo.com/live/8648/745468d84ca429e88bd6dd4314c8d131',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8711/4a3c2b1888122c5dbb8d1342b5f27345',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8706/517de793bc4e41abbe0346c42c4c184f',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8719/0d4e10051b8b48d0ee8b8e723ffb574f',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8714/d9ee0642175d064c108c6035d238a580',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8717/e054fbec22fd46cc03d1491b652b3888',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8718/79b96277a39f11cf721d7adbf4f3385d',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8702/39b608c5aeb9002220acf0f87ce32266',

  // test stream
  // 'webrtc://dev-stream-go.turingvideo.com/live/8708/8d69e00aab935f213c994ca6f5c23d1d',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8718/06dfcff50e644e7cf6cc24c05893faf0',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8715/236ff525af80a8d0c1963f28097489d6',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8710/06961ab2bb89412b234901d927a05c9a',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8709/d04af62f3732748ae5b0ba09929c1c9c',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8704/65579840d93e1b9e9b07d8d55d027282',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8712/630cd6ee395d2971f1599a1062d5b06b',
  // 'webrtc://dev-stream-go.turingvideo.com/live/8707/0216dda1763af9cfc2dc17ca4bbe8539',

  // dog food
  // 'webrtc://test-srs-1.turingvideo.com/live/20948/7a6e3120ab1c5a8f630a82b033dd37fe',
  // 'webrtc://test-srs-1.turingvideo.com/live/20891/9f2f289f7d366f7408c539444180418b',
  // 'webrtc://test-srs-1.turingvideo.com/live/21065/e30682d57badbd061c375a484998b0c2',
  // 'webrtc://test-srs-1.turingvideo.com/live/20945/a4553662cf30542c761cd6440b9f37c4',
  // 'webrtc://test-srs-1.turingvideo.com/live/20952/3e81e8b48051a50efcc0486400a616f4',
  // 'webrtc://test-srs-1.turingvideo.com/live/20892/d4f9a1da2a88fb7ed895f3b983a59003',

  // sale demo
  'webrtc://srs-1.turingvideo.com/live/61037/4f27082cab1c5da7812429122b832327',
  'webrtc://srs-1.turingvideo.com/live/85381/90d397cbcbd7a34bd181d88ee4aedc41',
  'webrtc://srs-1.turingvideo.com/live/60914/59c9a361f5c301e4d4ca56ac1aad85d1',
  'webrtc://srs-1.turingvideo.com/live/64637/af9225991f72e9c23c0775b27805e731',
  'webrtc://srs-1.turingvideo.com/live/60918/ba296f3a51c158ad6a79db905c52c8a7',
];

class MultiRTCStream extends StatefulWidget {
  const MultiRTCStream({super.key});

  @override
  State<MultiRTCStream> createState() => _MultiRTCStreamState();
}

class _MultiRTCStreamState extends State<MultiRTCStream> {
  final cubit1 = WebrtcPlayerCubit(camera: NestCamera(1, '1'));
  final cubit2 = WebrtcPlayerCubit(camera: NestCamera(2, '2'));
  final cubit3 = WebrtcPlayerCubit(camera: NestCamera(3, '3'));
  final cubit4 = WebrtcPlayerCubit(camera: NestCamera(4, '4'));
  final cubit5 = WebrtcPlayerCubit(camera: NestCamera(5, '5'));

  // final cubit6 = WebrtcPlayerCubit(camera: NestCamera(6, '6'));

  // final cubit7 = WebrtcPlayerCubit(camera: NestCamera(7, '7'));

  // final cubit8 = WebrtcPlayerCubit(camera: NestCamera(8, '8'));
  // final cubit9 = WebrtcPlayerCubit(camera: NestCamera(9, '9'));
  // final cubit10 = WebrtcPlayerCubit(camera: NestCamera(10, '10'));
  // final cubit11 = WebrtcPlayerCubit(camera: NestCamera(11, '11'));
  // final cubit12 = WebrtcPlayerCubit(camera: NestCamera(12, '12'));
  // final cubit13 = WebrtcPlayerCubit(camera: NestCamera(13, '13'));
  // final cubit14 = WebrtcPlayerCubit(camera: NestCamera(14, '14'));

  void play() {
    cubit1.playerController.setDataSource(
      url: streamUrls[0],
      codec: StreamEncode.h264.label,
    );
    cubit2.playerController.setDataSource(
      url: streamUrls[1],
      codec: StreamEncode.h264.label,
    );
    cubit3.playerController.setDataSource(
      url: streamUrls[2],
      codec: StreamEncode.h264.label,
    );
    cubit4.playerController.setDataSource(
      url: streamUrls[3],
      codec: StreamEncode.h264.label,
    );
    cubit5.playerController.setDataSource(
      url: streamUrls[4],
      codec: StreamEncode.h264.label,
    );

    // cubit6.playerController.setDataSource(
    //   url: streamUrls[5],
    //   codec: StreamEncode.h264.label,
    // );

    // cubit7.playerController.setDataSource(
    //   url: streamUrls[6],
    //   codec: StreamEncode.h264.label,
    // );

    // cubit8.playerController.setDataSource(
    //   url: streamUrls[7],
    //   codec: StreamEncode.h264.label,
    // );
    // cubit9.playerController.setDataSource(
    //   url: streamUrls[8],
    //   codec: StreamEncode.h264.label,
    // );

    // cubit10.playerController.setDataSource(
    //   url: streamUrls[9],
    //   codec: StreamEncode.h264.label,
    // );
    // cubit11.playerController.setDataSource(
    //   url: streamUrls[10],
    //   codec: StreamEncode.h264.label,
    // );
    // cubit12.playerController.setDataSource(
    //   url: streamUrls[11],
    //   codec: StreamEncode.h264.label,
    // );
    // cubit13.playerController.setDataSource(
    //   url: streamUrls[12],
    //   codec: StreamEncode.h264.label,
    // );

    // cubit14.playerController.setDataSource(
    //   url: streamUrls[13],
    //   codec: StreamEncode.h264.label,
    // );
  }

  @override
  void dispose() {
    cubit1.close();
    cubit2.close();
    cubit3.close();
    cubit4.close();
    cubit5.close();

    // cubit6.close();

    // cubit7.close();

    // cubit8.close();
    // cubit9.close();
    // cubit10.close();
    // cubit11.close();
    // cubit12.close();
    // cubit13.close();

    // cubit14.close();

    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final w = (MediaQuery.of(context).size.width - 2.0) / 2.0;

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
                style: TextStyle(fontSize: 12),
                maxLines: 100,
              ),
            Wrap(
              spacing: 2,
              runSpacing: 2,
              children: [
                BlocProvider.value(
                  value: cubit1,
                  child: SizedBox(
                    width: w,
                    height: h,
                    child: WebrtcPlayerView(
                      settings: PlayerSettings.live(),
                    ),
                  ),
                ),
                BlocProvider.value(
                  value: cubit2,
                  child: SizedBox(
                    width: w,
                    height: h,
                    child: WebrtcPlayerView(
                      settings: PlayerSettings.live(),
                    ),
                  ),
                ),
                BlocProvider.value(
                  value: cubit3,
                  child: SizedBox(
                    width: w,
                    height: h,
                    child: WebrtcPlayerView(
                      settings: PlayerSettings.live(),
                    ),
                  ),
                ),
                BlocProvider.value(
                  value: cubit4,
                  child: SizedBox(
                    width: w,
                    height: h,
                    child: WebrtcPlayerView(
                      settings: PlayerSettings.live(),
                    ),
                  ),
                ),
                BlocProvider.value(
                  value: cubit5,
                  child: SizedBox(
                    width: w,
                    height: h,
                    child: WebrtcPlayerView(
                      settings: PlayerSettings.live(),
                    ),
                  ),
                ),

                // BlocProvider.value(
                //   value: cubit6,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),

                // BlocProvider.value(
                //   value: cubit7,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit8,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit9,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit10,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit11,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit12,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit13,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
                //   ),
                // ),
                // BlocProvider.value(
                //   value: cubit14,
                //   child: SizedBox(
                //     width: w,
                //     height: h,
                //     child: WebrtcPlayerView(
                //       settings: PlayerSettings.live(),
                //     ),
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
