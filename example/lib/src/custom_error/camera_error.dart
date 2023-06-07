import 'package:turing_mobile/turing_foundation.dart';

import 'custom_error.dart';

const int cameraFetchFail = 4000;
const int osdSettingsFailed = 4001;
const int cameraIsPeopleCountingFail = 4002;
const int cloudCameraOffline = 4003;
const int cloudCameraInvalid = 4004;
const int cloudCameraClaimed = 4005;
const int cloudCameraDuplicate = 4006;
const int cameraMayBeRemoved = 4007;

const cameraFetchFailErr = TuringError(
  cameraFetchFail,
  'We could not find this camera. It might have been disconnected from your account.',
  cameraDebugMessage,
);

const cameraIsPeopleCountingErr = TuringError(
  cameraIsPeopleCountingFail,
  'This camera is people counting camera. It does not support Live View.',
  cameraDebugMessage,
);

const osdSettingsFailedErr = TuringError(
  osdSettingsFailed,
  'Sorry, something went wrong. Please go back and try again later.',
  cameraDebugMessage,
);

const cloudCameraOfflineError = TuringError(
  cloudCameraOffline,
  'The camera is offline. The camera must be online in order to be added.',
  cameraDebugMessage,
);

const cloudCameraInvalidError = TuringError(
  cloudCameraInvalid,
  'The MAC address is invalid. Please try again.',
  cameraDebugMessage,
);

const cloudCameraClaimedError = TuringError(
  cloudCameraClaimed,
  'The camera has already been claimed. Either you or someone else owns it. To add the camera, disconnect the camera first. Go to Settings > Devices, select the camera, and "Disconnect".',
  cameraDebugMessage,
);

const cloudCameraDuplicateError = TuringError(
  cloudCameraDuplicate,
  'Duplicate MAC address. The device has already been added to the list.',
  cameraDebugMessage,
);

const cameraMayBeRemovedError = TuringError(
  cameraMayBeRemoved,
  'This camera may have been removed from your Devices List.\n\nDetails about this alert are not available.',
  cameraDebugMessage,
);
