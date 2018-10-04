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

#include "api/peerconnectioninterface.h"

#include <future>

namespace caff {

struct IceCandidateInfo {
  std::string const sdp;
  std::string const sdp_mid;
  int const sdp_mline_index;
};

class PeerConnectionObserver : public webrtc::PeerConnectionObserver {
  RTC_DISALLOW_COPY_AND_ASSIGN(PeerConnectionObserver);

 public:
  PeerConnectionObserver();

  std::future<std::vector<IceCandidateInfo>> GetFuture();

  void OnSignalingChange(
      webrtc::PeerConnectionInterface::SignalingState new_state) override;
  void OnRenegotiationNeeded() override;
  void OnIceConnectionChange(
      webrtc::PeerConnectionInterface::IceConnectionState new_state) override;
  void OnAddStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnRemoveStream(
      rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
  void OnDataChannel(
      rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
  void OnIceGatheringChange(
      webrtc::PeerConnectionInterface::IceGatheringState new_state) override;
  void OnIceCandidate(webrtc::IceCandidateInterface const* candidate) override;

 private:
  std::promise<std::vector<IceCandidateInfo>> promise;
  std::vector<IceCandidateInfo> gatheredCandidates;
};

}  // namespace caff
