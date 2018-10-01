/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "broadcastaudiodevice.h"

namespace caff {

BroadcastAudioDevice::BroadcastAudioDevice() {}

int32_t BroadcastAudioDevice::RegisterAudioCallback(
    webrtc::AudioTransport* audioCallback) {
  // TODO
  return -1;
}

int32_t BroadcastAudioDevice::Init() {
  // TODO
  return 0;
}

int32_t BroadcastAudioDevice::Terminate() {
  // TODO
  return -1;
}

bool BroadcastAudioDevice::Initialized() const {
  // TODO
  return false;
}

int16_t BroadcastAudioDevice::RecordingDevices() {
  return 1;
}

int32_t BroadcastAudioDevice::InitRecording() {
  // TODO
  return -1;
}

bool BroadcastAudioDevice::RecordingIsInitialized() const {
  // TODO
  return false;
}

int32_t BroadcastAudioDevice::StartRecording() {
  // TODO
  return -1;
}

int32_t BroadcastAudioDevice::StopRecording() {
  // TODO
  return -1;
}

bool BroadcastAudioDevice::Recording() const {
  // TODO
  return false;
}

int32_t BroadcastAudioDevice::SetStereoRecording(bool enable) {
  // TODO
  return -1;
}

int32_t BroadcastAudioDevice::StereoRecording(bool* enabled) const {
  // TODO
  return -1;
}

}  // namespace caff
