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

#include "broadcast.h"
#include "interface.h"
#include "logsink.h"

#include "rtc_base/ssladapter.h"

#ifdef _WIN32
#include "rtc_base/win32socketinit.h"
#endif

namespace caff {

extern "C" {

caff_interface_handle caff_initialize(caff_log_callback_t log_callback,
                                      enum caff_log_severity min_severity) {
  RTC_DCHECK(log_callback);

  static bool first_init = true;
  if (first_init) {
    // Set up logging

    rtc::LogMessage::LogThreads(true);
    rtc::LogMessage::LogTimestamps(true);

    // Send logs only to our log sink. Not to stderr, windows debugger, etc
    rtc::LogMessage::LogToDebug(rtc::LoggingSeverity::LS_NONE);
    rtc::LogMessage::SetLogToStderr(false);

    // TODO: Figure out why this log sink isn't working
    rtc::LogMessage::AddLogToStream(
        new LogSink(log_callback),
        static_cast<rtc::LoggingSeverity>(min_severity));

    // Initialize WebRTC

#ifdef _WIN32
    rtc::EnsureWinsockInit();
#endif
    if (!rtc::InitializeSSL()) {
      RTC_LOG(LS_ERROR) << "Caffeine RTC failed to initialize SSL";
      return nullptr;
    }

    RTC_LOG(LS_INFO) << "Caffeine RTC initialized";
    first_init = false;
  }

  return new Interface;
}

caff_broadcast_handle caff_start_broadcast(
    caff_interface_handle interface_handle,
    void* user_data,
    caff_broadcast_started_t started_callback,
    caff_broadcast_failed_t failed_callback) {
  RTC_DCHECK(interface_handle);
  RTC_DCHECK(started_callback);
  RTC_DCHECK(failed_callback);

  // Encapsulate void * inside lambdas
  auto startedCallback = [=] { started_callback(user_data); };
  auto failedCallback = [=](caff_error error) {
    failed_callback(user_data, error);
  };

  auto interface = reinterpret_cast<Interface*>(interface_handle);
  return interface->StartBroadcast(startedCallback, failedCallback);
}

void caff_deinitialize(caff_interface_handle interface_handle) {
  RTC_DCHECK(interface_handle);
  auto interface = reinterpret_cast<Interface*>(interface_handle);
  delete interface;
}

}  // extern "C"

}  // namespace caff
