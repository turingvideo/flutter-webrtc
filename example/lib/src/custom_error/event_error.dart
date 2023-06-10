import 'package:turing_mobile/turing_foundation.dart';

import 'custom_error.dart';

const int eventsHasReachedMax = 3000;

const eventHasReachedMaxError = TuringError(
  eventsHasReachedMax,
  'Your events have reached end.',
  eventDebugMessage,
);
