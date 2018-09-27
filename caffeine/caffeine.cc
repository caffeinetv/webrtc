/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "caffeine.h"
#include "logsink.h"

#include <rtc_base/ssladapter.h>

#ifdef _WIN32
#include <rtc_base/win32socketinit.h>
#endif

extern "C" 
bool caff_initialize(caff_log_callback_t log_callback, caff_log_severity min_severity)
{
    static bool initialized = false;
    if (initialized) {
        RTC_LOG(LS_WARNING) << "Caffeine RTC is already initialized";
        return false;
    }

    // Set up logging

    rtc::LogMessage::LogThreads(true);
    rtc::LogMessage::LogTimestamps(true);

    // Send logs only to our log sink. Not to stderr, windows debugger, etc
    rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_NONE);
    rtc::LogMessage::SetLogToStderr(false);

    // TODO: Figure out why this log sink isn't working
    rtc::LogMessage::AddLogToStream(new caff::LogSink(log_callback),
                                    static_cast<rtc::LoggingSeverity>(min_severity));

    // Initialize WebRTC

#ifdef _WIN32
    rtc::EnsureWinsockInit();
#endif
    if (!rtc::InitializeSSL())
    {
        RTC_LOG(LS_ERROR) << "Caffeine RTC failed to initialize SSL";
        return false;
    }

    RTC_LOG(LS_INFO) << "Caffeine RTC initialized";
    initialized = true;
    return true;
}
