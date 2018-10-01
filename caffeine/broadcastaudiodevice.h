/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#pragma once

#include "audiodevicedefaultimpl.h"

namespace caff {

class BroadcastAudioDevice : public AudioDeviceDefaultImpl {
 public:
  BroadcastAudioDevice();

  int32_t RegisterAudioCallback(webrtc::AudioTransport* audioCallback) override;
  int32_t Init() override;
  int32_t Terminate() override;
  bool Initialized() const override;
  int16_t RecordingDevices() override;
  int32_t InitRecording() override;
  bool RecordingIsInitialized() const override;
  int32_t StartRecording() override;
  int32_t StopRecording() override;
  bool Recording() const override;
  int32_t SetStereoRecording(bool enable) override;
  int32_t StereoRecording(bool* enabled) const override;
};

}  // namespace caff
