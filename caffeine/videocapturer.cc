/*
 *  Copyright 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "videocapturer.h"

#include "api/video/i420_buffer.h"
#include "common_video/libyuv/include/webrtc_libyuv.h"
#include "rtc_base/logging.h"
#include "libyuv.h"

namespace caff {

// Copied from old version of libwebrtc
libyuv::RotationMode ConvertRotationMode(webrtc::VideoRotation rotation) {
  switch (rotation) {
    case webrtc::kVideoRotation_0:
      return libyuv::kRotate0;
    case webrtc::kVideoRotation_90:
      return libyuv::kRotate90;
    case webrtc::kVideoRotation_180:
      return libyuv::kRotate180;
    case webrtc::kVideoRotation_270:
      return libyuv::kRotate270;
  }
  RTC_NOTREACHED();
  return libyuv::kRotate0;
}

// Copied from old version of libwebrtc
static int ConvertToI420(webrtc::VideoType src_video_type,
                         const uint8_t* src_frame,
                         int crop_x,
                         int crop_y,
                         int src_width,
                         int src_height,
                         size_t sample_size,
                         webrtc::VideoRotation rotation,
                         webrtc::I420Buffer* dst_buffer) {
  int dst_width = dst_buffer->width();
  int dst_height = dst_buffer->height();
  // LibYuv expects pre-rotation values for dst.
  // Stride values should correspond to the destination values.
  if (rotation == webrtc::kVideoRotation_90 ||
      rotation == webrtc::kVideoRotation_270) {
    std::swap(dst_width, dst_height);
  }
  return libyuv::ConvertToI420(
      src_frame, sample_size, dst_buffer->MutableDataY(), dst_buffer->StrideY(),
      dst_buffer->MutableDataU(), dst_buffer->StrideU(),
      dst_buffer->MutableDataV(), dst_buffer->StrideV(), crop_x, crop_y,
      src_width, src_height, dst_width, dst_height,
      ConvertRotationMode(rotation), webrtc::ConvertVideoType(src_video_type));
}

cricket::CaptureState VideoCapturer::Start(cricket::VideoFormat const& format) {
  SetCaptureFormat(&format);
  SetCaptureState(cricket::CS_RUNNING);
  return cricket::CS_STARTING;
}

static uint32_t const enforcedFrameHeight = 720;

void VideoCapturer::SendVideo(uint8_t const* frameData,
                              size_t frameByteCount,
                              uint32_t width,
                              uint32_t height,
                              webrtc::VideoType format) {
  auto const now = rtc::TimeMicros();
  auto span = now - lastFrameMicros;
  if (span < minFrameMicros) {
    RTC_LOG(LS_INFO) << "Dropping frame";
    return;
  }
  lastFrameMicros = now;

  // TODO: scaling, check formats, etc
  rtc::scoped_refptr<webrtc::I420Buffer> unscaledBuffer =
      webrtc::I420Buffer::Create(width, height);

  ConvertToI420(format, frameData, 0, 0, width, height, frameByteCount,
                webrtc::kVideoRotation_0, unscaledBuffer.get());

  rtc::scoped_refptr<webrtc::I420Buffer> scaledBuffer = unscaledBuffer;
  uint32_t scaledWidth = width;
  uint32_t scaledHeight = enforcedFrameHeight;
  if (height != enforcedFrameHeight) {
    scaledWidth = width * enforcedFrameHeight / height;
    scaledBuffer = webrtc::I420Buffer::Create(scaledWidth, scaledHeight);
    scaledBuffer->ScaleFrom(*unscaledBuffer);
  }

  webrtc::VideoFrame frame(scaledBuffer, webrtc::kVideoRotation_0,
                           rtc::TimeMicros());

  OnFrame(frame, scaledWidth, scaledHeight);
}

void VideoCapturer::Stop() {}

bool VideoCapturer::IsRunning() {
  return true;
}

bool VideoCapturer::IsScreencast() const {
  return false;
}

bool VideoCapturer::GetPreferredFourccs(std::vector<uint32_t>* fourccs) {
  // ignore preferred formats
  if (fourccs == nullptr)
    return false;

  fourccs->clear();
  return true;
}

}  // namespace caff