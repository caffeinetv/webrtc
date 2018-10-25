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

#include <atomic>
#include <functional>
#include <vector>

#include "iceinfo.h"

#include "rtc_base/scoped_ref_ptr.h"

namespace webrtc {
class PeerConnectionFactoryInterface;
class MediaStreamInterface;
class PeerConnectionInterface;
}  // namespace webrtc

namespace caff {
class AudioDevice;
class VideoCapturer;

class Stream {
 public:
  Stream(AudioDevice* audioDevice,
         webrtc::PeerConnectionFactoryInterface* factory);
  virtual ~Stream();

  void Start(
      std::function<std::string(std::string const&)> offerGeneratedCallback,
      std::function<bool(std::vector<IceInfo> const&)> iceGatheredCallback,
      std::function<void()> startedCallback,
      std::function<void(caff_error)> failedCallback);

  void SendAudio(uint8_t const* samples, size_t samples_per_channel);
  void SendVideo(uint8_t const* frameData,
                 size_t frameBytes,
                 uint32_t width,
                 uint32_t height);

 private:
  std::atomic_bool started{false};
  AudioDevice* audioDevice;
  VideoCapturer* videoCapturer;
  webrtc::PeerConnectionFactoryInterface* factory;
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;
};

}  // namespace caff
