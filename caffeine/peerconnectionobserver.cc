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

PeerConnectionObserver::PeerConnectionObserver() {}

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
    webrtc::PeerConnectionInterface::IceGatheringState new_state) {}

void PeerConnectionObserver::OnIceCandidate(
    webrtc::IceCandidateInterface const* candidate) {}

}  // namespace caff
