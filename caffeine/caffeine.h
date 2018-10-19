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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(CAFFEINE_RTC_LIBRARY)
#if defined(_WIN32)
#define CAFFEINE_API __declspec(dllexport)
#else
#define CAFFEINE_API __attribute__((visibility("default")))
#endif
#elif defined(_WIN32)
#define CAFFEINE_API __declspec(dllimport)
#else
#define CAFFEINE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Log severities mapped from WebRTC library */
typedef enum {
  CAFF_LOG_SENSITIVE,
  CAFF_LOG_VERBOSE,
  CAFF_LOG_INFO,
  CAFF_LOG_WARNING,
  CAFF_LOG_ERROR,
  CAFF_LOG_NONE,
} caff_log_severity;

/* TODO: Errors generated by caffeine layer
 */
typedef enum {
  CAFF_ERROR_UNKNOWN,
} caff_error;

/* Callback type for WebRTC log messages */
typedef void (*caff_log_callback)(caff_log_severity severity,
                                  char const* message);

/* Struct for ICE offers/answers */
typedef struct {
  char const* sdp;
  char const* sdp_mid;
  int sdp_mline_index;
} caff_ice_info;

typedef caff_ice_info const caff_ice_candidates[];

/* Callback types for starting broadcast */

/* TODO: Move SDP & ICE stuff under the hood
 */
typedef char const* (*caff_offer_generated)(void* user_data, char const* offer);
typedef bool (*caff_ice_gathered)(void* user_data,
                                  caff_ice_candidates candidates,
                                  size_t num_candidates);
typedef void (*caff_broadcast_started)(void* user_data);
typedef void (*caff_broadcast_failed)(void* user_data, caff_error error);

/* Opaque handles to internal objects */
struct caff_interface;
typedef struct caff_interface* caff_interface_handle;

struct caff_broadcast;
typedef struct caff_broadcast* caff_broadcast_handle;

/* Initialize the Caffeine library
 *
 * log_callback: a function to call for WebRTC generated log messages
 * min_severity: sets the lowest log severity required for the callback to fire
 *
 * Returns a handle to the caffeine management object to be passed into other
 * functions. If there is an error during initialization this will be NULL
 */
CAFFEINE_API
caff_interface_handle caff_initialize(caff_log_callback log_callback,
                                      caff_log_severity min_severity);

/* Start broadcast on Caffeine
 *
 * Sets up the WebRTC connection with Caffeine asynchronously. Calls
 * into started_callback or failed_callback with the result. This may
 * happen on a different thread than the caller.
 *
 * **TODO**: move SDP offer/answer and ICE negotiation inside the library (i.e.
 * remove offer_generated_callback and ice_gathered_callback)
 *
 * interface_handle: handle to the caffeine interface from caff_initialize
 * user_data: an optional pointer passed blindly to the callbacks
 * started_callback: called when broadcast successfully starts
 * failed_callback: called when broadcast fails to start
 *
 * returns a handle to the broadcast. If an error occurs before starting
 * the asynchronous operation, the handle will be NULL and the
 * failed_callback will NOT be called
 */
CAFFEINE_API
caff_broadcast_handle caff_start_broadcast(
    caff_interface_handle interface_handle,
    void* user_data,
    caff_offer_generated offer_generated_callback,
    caff_ice_gathered ice_gathered_callback,
    caff_broadcast_started started_callback,
    caff_broadcast_failed failed_callback);

/* TODO */
/* caff_update_screenshot(caff_broadcast_handle, some_data...) */
/* caff_update_broadcast(caff_broadcast_handle, some_data...) */
/* caff_send_video(caff_broadcast_handle, some_data...) */
/* caff_send_audio(caff_broadcast_handle, some_data...) */

/* TODO pass format, channels, etc */
CAFFEINE_API
void caff_send_audio(caff_broadcast_handle broadcast_handle,
                     uint8_t* samples,
                     size_t samples_per_channel);

/* End a Caffeine broadcast
 *
 * This signals the server to end the broadcast and closes the RTC connection.
 *
 * broadcast_handle: the broadcast handle received from caff_start_broadcast.
 *     This handle will no longer be valid after the function returns.
 */
CAFFEINE_API
void caff_end_broadcast(caff_broadcast_handle broadcast_handle);

/* Deinitialize Caffeine library
 *
 * This destroys the internal factory objects, shuts down worker threads, etc.
 *
 * interface_handle: the interface handle received from caff_initialize. This
 *     handle will no longer be valid after the function returns.
 */
CAFFEINE_API
void caff_deinitialize(caff_interface_handle interface_handle);

#ifdef __cplusplus
}
#endif

#endif /* CAFFEINE_RTC_CAFFEINE_H */
