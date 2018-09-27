/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef CAFFEINE_RTC_CAFFEINE_H
#define CAFFEINE_RTC_CAFFEINE_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

enum caff_log_severity {
    CAFF_LOG_SENSITIVE,
    CAFF_LOG_VERBOSE,
    CAFF_LOG_INFO,
    CAFF_LOG_WARNING,
    CAFF_LOG_ERROR,
    CAFF_LOG_NONE
};

typedef void (* caff_log_callback_t)(enum caff_log_severity severity,
                                     char const * message);

bool caff_initialize(caff_log_callback_t log_callback,
                     enum caff_log_severity min_severity);

#ifdef __cplusplus
}
#endif

#endif /* CAFFEINE_RTC_CAFFEINE_H */
