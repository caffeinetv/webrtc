/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "interface.h"

#include "audiodevice.h"
#include "stream.h"

#include "api/peerconnectioninterface.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"

#include "modules/audio_processing/include/audio_processing.h"

#include "rtc_base/thread.h"

namespace caff {

Interface::Interface() {
  networkThread = rtc::Thread::CreateWithSocketServer();
  networkThread->SetName("caffeine-network", nullptr);
  networkThread->Start();

  workerThread = rtc::Thread::Create();
  workerThread->SetName("caffeine-worker", nullptr);
  workerThread->Start();

  signalingThread = rtc::Thread::Create();
  signalingThread->SetName("caffeine-signaling", nullptr);
  signalingThread->Start();

  audioDevice = workerThread->Invoke<rtc::scoped_refptr<AudioDevice>>(
      RTC_FROM_HERE, [] { return new AudioDevice(); });

  factory = webrtc::CreatePeerConnectionFactory(
      networkThread.get(), workerThread.get(), signalingThread.get(),
      audioDevice, webrtc::CreateBuiltinAudioEncoderFactory(),
      webrtc::CreateBuiltinAudioDecoderFactory(),
      webrtc::CreateBuiltinVideoEncoderFactory(),
      webrtc::CreateBuiltinVideoDecoderFactory(), nullptr, nullptr);
}

Interface::~Interface() {
  factory = nullptr;
}

Stream* Interface::StartStream(
    std::function<std::string(std::string const&)> offerGeneratedCallback,
    std::function<bool(std::vector<IceInfo> const&)> iceGatheredCallback,
    std::function<void()> startedCallback,
    std::function<void(caff_error)> failedCallback) {
  auto stream = new Stream(audioDevice, factory);
  stream->Start(offerGeneratedCallback, iceGatheredCallback, startedCallback,
                failedCallback);
  return stream;
}

}  // namespace caff
