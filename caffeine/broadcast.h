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

#include "rtc_base/scoped_ref_ptr.h"

namespace webrtc {
class MediaStreamInterface;
}

namespace caff {

class Broadcast {
 public:
  Broadcast(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream);
  virtual ~Broadcast();

 private:
  rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
};

}  // namespace caff
