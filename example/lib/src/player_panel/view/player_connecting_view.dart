import 'package:flutter/material.dart';

class PlayerConnectingView extends StatelessWidget {
  const PlayerConnectingView({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      width: MediaQuery.of(context).size.width,
      height: MediaQuery.of(context).size.height,
      child: Center(
        child: LayoutBuilder(builder: (context, constraints) {
          return SizedBox(
            width: constraints.maxWidth > 200 ? 40 : 20,
            height: constraints.maxWidth > 200 ? 40 : 20,
            child: CircularProgressIndicator(
              strokeWidth: constraints.maxWidth > 200 ? 3.0 : 2.0,
              valueColor: const AlwaysStoppedAnimation(Colors.white),
            ),
          );
        }),
      ),
    );
  }
}
