import 'package:turing_mobile/turing_foundation.dart';

import 'custom_error.dart';

const int boxHasBeenClaimed = 1000;
const int boxOffline = 1001;
const int siteNameAlreadyExist = 1100;
const int disconnectBoxFailed = 1200;
const int boxSystemUpgradeInProgress = 1201;
const int upgradeServiceNotFound = 1202;

const siteNameAlreadyExistError = TuringError(
  siteNameAlreadyExist,
  'This site name is already in use. Try another?',
  boxDebugMessage,
);

const boxHasBeenClaimedError = TuringError(
  boxHasBeenClaimed,
  'This bridge has already been claimed. Find it in your existing list of bridges. If not, call Turing support.',
  boxDebugMessage,
);

const boxOfflineError = TuringError(
  boxOffline,
  'Your bridge is still offline. Try again?',
  boxDebugMessage,
);

const disconnectBoxFailedError = TuringError(
  disconnectBoxFailed,
  'Failed to disconnect bridge, please try again.',
  boxDebugMessage,
);

const boxSystemUpgradeInProgressError = TuringError(
  boxSystemUpgradeInProgress,
  'The bridge upgrade is in progress',
  boxDebugMessage,
);

const upgradeServiceNotFoundError = TuringError(
  upgradeServiceNotFound,
  'The upgrade service is not found',
  boxDebugMessage,
);
