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

#include "rtc_base/scoped_ref_ptr.h"

namespace webrtc {
class MediaStreamInterface;
class PeerConnectionInterface;
}

namespace caff {
class BroadcastAudioDevice;
class BroadcastVideoCapturer;

// TODO: maybe don't need this layer of indirection
class Broadcast {
 public:
  Broadcast(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream,
            rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection,
            rtc::scoped_refptr<BroadcastAudioDevice> audioDevice,
            rtc::scoped_refptr<BroadcastVideoCapturer> videoCapturer);
  virtual ~Broadcast();

  void SendAudio(uint8_t const * samples, size_t samples_per_channel);
  void SendVideo(uint8_t const* frameData,
                 size_t frameBytes,
                 uint32_t width,
                 uint32_t height);

 private:
  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection;
  rtc::scoped_refptr<BroadcastAudioDevice> audioDevice;
  rtc::scoped_refptr<BroadcastVideoCapturer> videoCapturer;
};

}  // namespace caff
