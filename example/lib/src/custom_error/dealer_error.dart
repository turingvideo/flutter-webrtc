import 'package:turing_mobile/turing_foundation.dart';

import 'custom_error.dart';

const int noCustomerActiveSession = 8000;
const int duplicatedCustomerEmail = 8001;
const int batchUpgradeAllFailed = 8002;

const noCustomerActiveSessionError = TuringError(
  noCustomerActiveSession,
  'You have no permission to add device.',
  dealerDebugMessage,
);

const duplicatedCustomerEmailError = TuringError(
  duplicatedCustomerEmail,
  'You\'ve already created an account with this email. Use the existing account or choose a different email to create a new account.',
  'duplicate',
);

const batchUpgradeAllFailedError = TuringError(
  batchUpgradeAllFailed,
  'Your upgrades could not be completed.',
  dealerDebugMessage,
);
