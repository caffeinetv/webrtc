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

#include "broadcast.h"
#include "broadcastaudiodevice.h"
#include "peerconnectionobserver.h"

#include "api/peerconnectioninterface.h"

#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"

#include "modules/audio_processing/include/audio_processing.h"

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

    auto adm =
        workerThread->Invoke<rtc::scoped_refptr<webrtc::AudioDeviceModule>>(
            RTC_FROM_HERE, [] { return new BroadcastAudioDevice(); });

    adm->AddRef();

    factory = webrtc::CreatePeerConnectionFactory(
        networkThread.get(), workerThread.get(), signalingThread.get(), adm,
        webrtc::CreateBuiltinAudioEncoderFactory(),
        webrtc::CreateBuiltinAudioDecoderFactory(),
        webrtc::CreateBuiltinVideoEncoderFactory(),
        webrtc::CreateBuiltinVideoDecoderFactory(), nullptr, nullptr);
  }

  Interface::~Interface() {
    factory = nullptr;
  }

  Broadcast* Interface::StartBroadcast(
      std::function<void()> startedCallback,
      std::function<void(caff_error)> failedCallback) {
    webrtc::PeerConnectionInterface::IceServer server;
    server.urls.push_back("stun:stun.l.google.com:19302");

    webrtc::PeerConnectionInterface::RTCConfiguration config;
    config.servers.push_back(server);

    auto observer = new PeerConnectionObserver();

    auto peerConnection = factory->CreatePeerConnection(
        config, webrtc::PeerConnectionDependencies(observer));

    // TODO: signaling
    startedCallback();

    return new Broadcast;
  }

  }
