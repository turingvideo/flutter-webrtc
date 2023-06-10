import 'dart:io';

import 'package:flutter/services.dart';
import 'package:orientation/orientation.dart';
import 'package:rxdart/rxdart.dart';

class OrientationHelper {
  static Future<void> setEnabledSystemUIOverlays(
      List<SystemUiOverlay> overlays) {
    if (Platform.isAndroid) {
      return OrientationPlugin.setEnabledSystemUIOverlays(overlays);
    } else {
      return SystemChrome.setEnabledSystemUIMode(SystemUiMode.manual,
          overlays: overlays);
    }
  }

  static Future<void> setPreferredOrientations(
      List<DeviceOrientation> orientations) {
    return OrientationPlugin.setPreferredOrientations(orientations);
  }

  static Future<void> forceOrientation(DeviceOrientation orientation) {
    return OrientationPlugin.forceOrientation(orientation);
  }

  static PublishSubject<DeviceOrientation> get onOrientationChange {
    final stream = OrientationPlugin.onOrientationChange
        .shareValueSeeded(DeviceOrientation.portraitUp)
        .distinct();
    final PublishSubject<DeviceOrientation> subject = PublishSubject();
    subject.addStream(stream);
    return subject;
  }
}
