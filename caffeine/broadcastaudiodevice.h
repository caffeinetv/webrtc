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

#include <vector>

namespace caff {

class BroadcastAudioDevice : public AudioDeviceDefaultImpl {
 public:
  BroadcastAudioDevice();

  void SendAudio(uint8_t const* data, size_t samples_per_channel);

  virtual int32_t RegisterAudioCallback(
      webrtc::AudioTransport* audioTransport) override;
  virtual int32_t Init() override;
  virtual int32_t Terminate() override;
  virtual bool Initialized() const override;
  virtual int16_t RecordingDevices() override;
  virtual int32_t InitRecording() override;
  virtual bool RecordingIsInitialized() const override;
  virtual int32_t StartRecording() override;
  virtual int32_t StopRecording() override;
  virtual bool Recording() const override;
  virtual int32_t SetStereoRecording(bool enable) override;
  virtual int32_t StereoRecording(bool* enabled) const override;

 private:
  webrtc::AudioTransport* audioTransport{nullptr};
  std::vector<uint8_t> buffer;
  size_t bufferIndex{0};
};

}  // namespace caff
