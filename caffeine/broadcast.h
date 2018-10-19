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
}

namespace caff {
class BroadcastAudioDevice;

// TODO: maybe don't need this layer of indirection
class Broadcast {
 public:
  Broadcast(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream,
            rtc::scoped_refptr<BroadcastAudioDevice> audioDevice);
  virtual ~Broadcast();

  void SendAudio(uint8_t* samples, size_t samples_per_channel);

 private:
  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
  rtc::scoped_refptr<BroadcastAudioDevice> audioDevice;
};

}  // namespace caff
