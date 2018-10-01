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

#include "api/refcountedbase.h"
#include "modules/audio_device/include/audio_device.h"

namespace caff {

// Contains default implementations of AudioDeviceModule methods that
// are not supported by caffeine. This is mostly to make the actual impl
// more readable and maintainable.
class AudioDeviceDefaultImpl : public webrtc::AudioDeviceModule,
                               public rtc::RefCountedBase {
 public:
  void AddRef() const override { rtc::RefCountedBase::AddRef(); }
  rtc::RefCountReleaseStatus Release() const override {
    return rtc::RefCountedBase::Release();
  }

  // Retrieve the currently utilized audio layer
  int32_t ActiveAudioLayer(AudioLayer* audioLayer) const override { return -1; }

  // Full-duplex transportation of PCM audio
  int32_t RegisterAudioCallback(
      webrtc::AudioTransport* audioCallback) override = 0;

  // Main initialization and termination
  int32_t Init() override = 0;
  int32_t Terminate() override = 0;
  bool Initialized() const override = 0;

  // Device enumeration
  int16_t PlayoutDevices() override { return 0; }
  int16_t RecordingDevices() override = 0;
  int32_t PlayoutDeviceName(uint16_t index,
                            char name[webrtc::kAdmMaxDeviceNameSize],
                            char guid[webrtc::kAdmMaxGuidSize]) override {
    return -1;
  }
  int32_t RecordingDeviceName(uint16_t index,
                              char name[webrtc::kAdmMaxDeviceNameSize],
                              char guid[webrtc::kAdmMaxGuidSize]) override {
    return -1;
  }

  // Device selection
  int32_t SetPlayoutDevice(uint16_t index) override { return -1; }
  int32_t SetPlayoutDevice(WindowsDeviceType device) override { return -1; }
  int32_t SetRecordingDevice(uint16_t index) override { return -1; }
  int32_t SetRecordingDevice(WindowsDeviceType device) override { return -1; }

  // Audio transport initialization
  int32_t PlayoutIsAvailable(bool* available) override {
    *available = false;
    return 0;
  }
  int32_t InitPlayout() override { return -1; }
  bool PlayoutIsInitialized() const override { return false; }
  int32_t RecordingIsAvailable(bool* available) override {
    *available = true;
    return 0;
  }
  int32_t InitRecording() override = 0;
  bool RecordingIsInitialized() const override = 0;

  // Audio transport control
  int32_t StartPlayout() override { return -1; }
  int32_t StopPlayout() override { return -1; }
  bool Playing() const override { return -1; }
  int32_t StartRecording() override = 0;
  int32_t StopRecording() override = 0;
  bool Recording() const override = 0;

  // Audio mixer initialization
  int32_t InitSpeaker() override { return -1; }
  bool SpeakerIsInitialized() const override { return false; }
  int32_t InitMicrophone() override { return -1; }
  bool MicrophoneIsInitialized() const override { return false; }

  // Speaker volume controls
  int32_t SpeakerVolumeIsAvailable(bool* available) override {
    *available = false;
    return 0;
  }
  int32_t SetSpeakerVolume(uint32_t volume) override { return -1; }
  int32_t SpeakerVolume(uint32_t* volume) const override { return -1; }
  int32_t MaxSpeakerVolume(uint32_t* maxVolume) const override { return -1; }
  int32_t MinSpeakerVolume(uint32_t* minVolume) const override { return -1; }

  // Microphone volume controls
  int32_t MicrophoneVolumeIsAvailable(bool* available) override {
    *available = false;
    return 0;
  }
  int32_t SetMicrophoneVolume(uint32_t volume) override { return -1; }
  int32_t MicrophoneVolume(uint32_t* volume) const override { return -1; }
  int32_t MaxMicrophoneVolume(uint32_t* maxVolume) const override { return -1; }
  int32_t MinMicrophoneVolume(uint32_t* minVolume) const override { return -1; }

  // Speaker mute control
  int32_t SpeakerMuteIsAvailable(bool* available) override {
    *available = false;
    return 0;
  }
  int32_t SetSpeakerMute(bool enable) override { return -1; }
  int32_t SpeakerMute(bool* enabled) const override { return -1; }

  // Microphone mute control
  int32_t MicrophoneMuteIsAvailable(bool* available) override {
    *available = false;
    return 0;
  }
  int32_t SetMicrophoneMute(bool enable) override { return -1; }
  int32_t MicrophoneMute(bool* enabled) const override { return -1; }

  // Stereo support
  int32_t StereoPlayoutIsAvailable(bool* available) const override {
    *available = false;
    return 0;
  }
  int32_t SetStereoPlayout(bool enable) override { return -1; }
  int32_t StereoPlayout(bool* enabled) const override { return -1; }
  int32_t StereoRecordingIsAvailable(bool* available) const override {
    *available = true;
    return 0;
  }
  int32_t SetStereoRecording(bool enable) override = 0;
  int32_t StereoRecording(bool* enabled) const override = 0;

  // Playout delay
  int32_t PlayoutDelay(uint16_t* delayMS) const override { return -1; }

  // Only supported on Android.
  bool BuiltInAECIsAvailable() const override { return false; }
  bool BuiltInAGCIsAvailable() const override { return false; }
  bool BuiltInNSIsAvailable() const override { return false; }

  // Enables the built-in audio effects. Only supported on Android.
  int32_t EnableBuiltInAEC(bool enable) override { return -1; }
  int32_t EnableBuiltInAGC(bool enable) override { return -1; }
  int32_t EnableBuiltInNS(bool enable) override { return -1; }

 protected:
  ~AudioDeviceDefaultImpl() override {}
};

}  // namespace caff
