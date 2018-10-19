/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "broadcast.h"
#include "broadcastaudiodevice.h"

#include "api/mediastreaminterface.h"

namespace caff {

Broadcast::Broadcast(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream,
                     rtc::scoped_refptr<BroadcastAudioDevice> audioDevice)
    : stream(stream), audioDevice(audioDevice) {}

Broadcast::~Broadcast() {}

void Broadcast::SendAudio(uint8_t* samples, size_t samples_per_channel) {
  audioDevice->SendAudio(samples, samples_per_channel);
}

}  // namespace caff
