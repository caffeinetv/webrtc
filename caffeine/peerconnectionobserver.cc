/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "peerconnectionobserver.h"

namespace caff {

PeerConnectionObserver::PeerConnectionObserver(
    std::function<void(std::vector<caff_ice_info> const&)>
        iceGatheredCallback)
    : iceGatheredCallback(iceGatheredCallback) {}

void PeerConnectionObserver::OnSignalingChange(
    webrtc::PeerConnectionInterface::SignalingState new_state) {}

void PeerConnectionObserver::OnRenegotiationNeeded() {}

void PeerConnectionObserver::OnIceConnectionChange(
    webrtc::PeerConnectionInterface::IceConnectionState new_state) {}

void PeerConnectionObserver::OnAddStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {}

void PeerConnectionObserver::OnRemoveStream(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) {}

void PeerConnectionObserver::OnDataChannel(
    rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {}

void PeerConnectionObserver::OnIceGatheringChange(
    webrtc::PeerConnectionInterface::IceGatheringState new_state) {
  using State = webrtc::PeerConnectionInterface::IceGatheringState;
  switch (new_state) {
    case State::kIceGatheringNew:
      RTC_LOG(LS_INFO) << "ICE gathering: new";
      break;
    case State::kIceGatheringGathering:
      RTC_LOG(LS_INFO) << "ICE gathering: gathering";
      break;
    case State::kIceGatheringComplete:
      RTC_LOG(LS_INFO) << "ICE gathering: complete";
      iceGatheredCallback(gatheredCandidates);
      break;
    default:
      RTC_LOG(LS_WARNING) << "ICE gathering: unrecognized state [" << new_state
                          << "]";
      break;
  }
}

void PeerConnectionObserver::OnIceCandidate(
    webrtc::IceCandidateInterface const* candidate) {
  RTC_DCHECK(candidate);

  stringHolders.emplace_back();
  auto & sdpString = stringHolders.back();
  if (!candidate->ToString(&sdpString)) {
    RTC_LOG(LS_ERROR) << "Failed parsing ICE candidate";
    stringHolders.pop_back();
    return;
  }

  stringHolders.emplace_back(candidate->sdp_mid());
  auto& sdpMidString = stringHolders.back();

  gatheredCandidates.push_back(
      {sdpString.c_str(), sdpMidString.c_str(), candidate->sdp_mline_index()});
  RTC_LOG(LS_INFO) << "ICE candidate added";
}

}  // namespace caff
