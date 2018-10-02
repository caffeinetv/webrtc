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

#include "caffeine.h"

#include <functional>

#include "rtc_base/scoped_ref_ptr.h"

namespace webrtc {
class PeerConnectionFactoryInterface;
}

namespace rtc {
class Thread;
}

namespace caff {
class Broadcast;

class Interface {
 public:
  Interface();

  virtual ~Interface();

  Broadcast* StartBroadcast(std::function<void()> startedCallback,
                            std::function<void(caff_error)> failedCallback);

 private:
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory;
  std::unique_ptr<rtc::Thread> networkThread;
  std::unique_ptr<rtc::Thread> workerThread;
  std::unique_ptr<rtc::Thread> signalingThread;
};

}
