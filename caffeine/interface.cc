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
#include "peerconnectionobserver.h"
#include "sessiondescriptionobserver.h"
#include "stream.h"
#include "videocapturer.h"

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
  webrtc::PeerConnectionInterface::IceServer server;
  server.urls.push_back("stun:stun.l.google.com:19302");

  webrtc::PeerConnectionInterface::RTCConfiguration config;
  config.servers.push_back(server);

  auto observer = new PeerConnectionObserver;

  auto peerConnection = factory->CreatePeerConnection(
      config, webrtc::PeerConnectionDependencies(observer));

  auto videoCapturer = rtc::scoped_refptr<VideoCapturer>(new VideoCapturer);
  auto videoSource = factory->CreateVideoSource(videoCapturer);
  auto videoTrack = factory->CreateVideoTrack("external_video", videoSource);

  auto audioSource = factory->CreateAudioSource(cricket::AudioOptions());
  auto audioTrack = factory->CreateAudioTrack("external_audio", audioSource);

  auto stream = factory->CreateLocalMediaStream("caffeine_stream");
  stream->AddTrack(videoTrack);
  stream->AddTrack(audioTrack);

  rtc::scoped_refptr<CreateSessionDescriptionObserver> createObserver =
      new rtc::RefCountedObject<CreateSessionDescriptionObserver>;

  webrtc::PeerConnectionInterface::RTCOfferAnswerOptions options;

  peerConnection->AddStream(stream);

  // TODO: do this stuff asynchronously
  peerConnection->CreateOffer(createObserver, options);
  auto offer = createObserver->GetFuture().get();
  if (!offer) {
    // Logged by the observer
    return nullptr;
  }

  if (offer->type() != webrtc::SessionDescriptionInterface::kOffer) {
    RTC_LOG(LS_ERROR) << "Expected "
                      << webrtc::SessionDescriptionInterface::kOffer
                      << " but got " << offer->type();
    return nullptr;
  }

  std::string offerSdp;
  if (!offer->ToString(&offerSdp)) {
    RTC_LOG(LS_ERROR) << "Error serializing SDP offer";
    return nullptr;
  }

  webrtc::SdpParseError offerError;
  auto localDesc = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer,
                                                    offerSdp, &offerError);
  if (!localDesc) {
    RTC_LOG(LS_ERROR) << "Error parsing SDP offer: " << offerError.description;
    return nullptr;
  }

  rtc::scoped_refptr<SetSessionDescriptionObserver> setLocalObserver =
      new rtc::RefCountedObject<SetSessionDescriptionObserver>;

  peerConnection->SetLocalDescription(setLocalObserver, localDesc.release());
  auto setLocalSuccess = setLocalObserver->GetFuture().get();
  if (!setLocalSuccess) {
    // Logged by the observer
    return nullptr;
  }

  /* TODO: this is probably the first point where async will split off */

  // offerGenerated must be called before iceGathered so that a signed_payload
  // is available
  std::string answerSdp = offerGeneratedCallback(offerSdp);

  webrtc::SdpParseError answerError;
  auto remoteDesc = webrtc::CreateSessionDescription(webrtc::SdpType::kAnswer,
                                                     answerSdp, &answerError);
  if (!remoteDesc) {
    RTC_LOG(LS_ERROR) << "Error parsing SDP answer: "
                      << answerError.description;
    return nullptr;
  }

  if (!iceGatheredCallback(observer->GetFuture().get())) {
    RTC_LOG(LS_ERROR) << "Failed to negotiate ICE";
    return nullptr;
  }

  rtc::scoped_refptr<SetSessionDescriptionObserver> setRemoteObserver =
      new rtc::RefCountedObject<SetSessionDescriptionObserver>;

  peerConnection->SetRemoteDescription(setRemoteObserver, remoteDesc.release());
  auto setRemoteSuccess = setRemoteObserver->GetFuture().get();
  if (!setRemoteSuccess) {
    // Logged by the observer
    return nullptr;
  }

  startedCallback();

  return new Stream(stream, peerConnection, audioDevice, videoCapturer);
}

}  // namespace caff
