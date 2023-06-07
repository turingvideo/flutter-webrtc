import 'package:turing_mobile/turing_foundation.dart';

import 'custom_error.dart';

/// unauthenticated error
const int unauthenticated = 100;
const int unauthenticatedExpire = 101;

const unauthenticatedError = TuringError(
  unauthenticated,
  'Authentication error.',
  defaultDebugMessage,
);

const unauthenticatedExpireError = TuringError(
  unauthenticated,
  'Authentication expired.',
  defaultDebugMessage,
);

/// set notification error
const int notificationWrong = 5000;

const notificationError = TuringError(
  notificationWrong,
  'Please select non-overlapping time ranges.',
  settingDebugMessage,
);

/// contact created error
const int contactCreatedFail = 5100;
const int faceNotDetectedInImage = 5101;
const String dmFaceNotDetectedInImage = 'image_no_face_detected';

const contactCreatedError = TuringError(
  contactCreatedFail,
  'Failed to create a new contact.',
  contactDebugMessage,
);

const faceNotDetectedInImageError = TuringError(
  faceNotDetectedInImage,
  'A face is not detected in the uploaded image.',
  contactDebugMessage,
);

/// take/upload/select photo error
const int uploadPhotoFail = 5200;
const int takePhotoFail = 5300;
const int selectPhotoFail = 5400;

const uploadPhotoError = TuringError(
  uploadPhotoFail,
  'Failed to upload photo to remote server, please retry.',
  uploadDebugMessage,
);

const takePhotoError = TuringError(
  takePhotoFail,
  'Failed to take photo.',
  uploadDebugMessage,
);

const selectPhotoError = TuringError(
  selectPhotoFail,
  'Failed to select photo.',
  uploadDebugMessage,
);

/// Edit Profile Image < Detected Person
const int editProfileImageFail = 5500;

const editProfileImageError = TuringError(
  editProfileImageFail,
  'Failed to update profile image.',
  contactDebugMessage,
);
