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

#include "api/mediastreaminterface.h"

namespace caff {

Broadcast::Broadcast(
    rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
    : stream(stream) {}

Broadcast::~Broadcast() {}

}  // namespace caff
