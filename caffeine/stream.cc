/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "stream.h"
#include "audiodevice.h"
#include "videocapturer.h"

#include "api/mediastreaminterface.h"
#include "api/peerconnectioninterface.h"

namespace caff {

Stream::Stream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream,
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peerConnection,
    rtc::scoped_refptr<AudioDevice> audioDevice,
    rtc::scoped_refptr<VideoCapturer> videoCapturer)
    : stream(stream),
      peerConnection(peerConnection),
      audioDevice(audioDevice),
      videoCapturer(videoCapturer) {}

Stream::~Stream() {}

void Stream::SendAudio(uint8_t const* samples, size_t samples_per_channel) {
  audioDevice->SendAudio(samples, samples_per_channel);
}

void Stream::SendVideo(uint8_t const* frameData,
                       size_t frameBytes,
                       uint32_t width,
                       uint32_t height) {
  videoCapturer->SendVideo(frameData, frameBytes, width, height);
}

}  // namespace caff
