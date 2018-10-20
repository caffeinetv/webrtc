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
#include "broadcastvideocapturer.h"

#include "api/mediastreaminterface.h"
#include "api/peerconnectioninterface.h"

namespace caff {

Broadcast::Broadcast(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream,
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection,
    rtc::scoped_refptr<BroadcastAudioDevice> audioDevice,
    rtc::scoped_refptr<BroadcastVideoCapturer> videoCapturer)
    : stream(stream),
      peerConnection(peerConnection),
      audioDevice(audioDevice),
      videoCapturer(videoCapturer) {}

Broadcast::~Broadcast() { }

void Broadcast::SendAudio(uint8_t const* samples, size_t samples_per_channel) {
  audioDevice->SendAudio(samples, samples_per_channel);
}

void Broadcast::SendVideo(uint8_t const* frameData,
	size_t frameBytes,
	uint32_t width,
	uint32_t height) {
  videoCapturer->SendVideo(frameData, frameBytes, width, height);
}

}  // namespace caff
