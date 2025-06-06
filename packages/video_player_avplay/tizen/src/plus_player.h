// Copyright 2023 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_PLUGIN_PLUS_PLAYER_H_
#define FLUTTER_PLUGIN_PLUS_PLAYER_H_

#include <flutter/plugin_registrar.h>

#include <memory>
#include <string>

#include "device_proxy.h"
#include "drm_manager.h"
#include "messages.h"
#include "plusplayer/plusplayer_wrapper.h"
#include "video_player.h"

class PlusPlayer : public VideoPlayer {
 public:
  explicit PlusPlayer(flutter::BinaryMessenger *messenger,
                      FlutterDesktopViewRef flutter_view);
  ~PlusPlayer();

  int64_t Create(const std::string &uri,
                 const CreateMessage &create_message) override;
  void Dispose() override;

  void SetDisplayRoi(int32_t x, int32_t y, int32_t width,
                     int32_t height) override;
  bool Play() override;
  bool Deactivate() override;
  bool Activate() override;
  bool Pause() override;
  bool SetLooping(bool is_looping) override;
  bool SetVolume(double volume) override;
  bool SetPlaybackSpeed(double speed) override;
  bool SeekTo(int64_t position, SeekCompletedCallback callback) override;
  int64_t GetPosition() override;
  std::pair<int64_t, int64_t> GetDuration() override;
  void GetVideoSize(int32_t *width, int32_t *height) override;
  bool IsReady() override;
  flutter::EncodableList GetTrackInfo(std::string track_type) override;
  bool SetTrackSelection(int32_t track_id, std::string track_type) override;
  std::string GetStreamingProperty(
      const std::string &streaming_property_type) override;
  bool SetBufferConfig(const std::string &key, int64_t value) override;
  void SetStreamingProperty(const std::string &type,
                            const std::string &value) override;
  bool SetDisplayRotate(int64_t rotation) override;
  bool SetDisplayMode(int64_t display_mode) override;
  bool Suspend() override;
  bool Restore(const CreateMessage *restore_message,
               int64_t resume_time) override;
  bool SetData(const flutter::EncodableMap &data) override;
  flutter::EncodableMap GetData(const flutter::EncodableList &data) override;
  flutter::EncodableList GetActiveTrackInfo() override;

 private:
  bool IsLive();
  std::pair<int64_t, int64_t> GetLiveDuration();
  bool SetDisplay();
  bool SetDrm(const std::string &uri, int drm_type,
              const std::string &license_server_url);
  flutter::EncodableValue ParseVideoTrack(plusplayer::Track video_track);
  flutter::EncodableValue ParseAudioTrack(plusplayer::Track audio_track);
  flutter::EncodableValue ParseSubtitleTrack(plusplayer::Track subtitle_track);
  void RegisterListener();
  bool StopAndClose();
  bool RestorePlayer(const CreateMessage *restore_message, int64_t resume_time);

  static bool OnLicenseAcquired(int *drm_handle, unsigned int length,
                                unsigned char *pssh_data, void *user_data);
  static void OnPrepareDone(bool ret, void *user_data);
  static void OnBufferStatus(const int percent, void *user_data);
  static void OnSeekDone(void *user_data);
  static void OnEos(void *user_data);
  static void OnSubtitleData(char *data, const int size,
                             const plusplayer::SubtitleType &type,
                             const uint64_t duration,
                             plusplayer::SubtitleAttributeListPtr attr_list,
                             void *user_data);
  static void OnResourceConflicted(void *user_data);
  static void OnError(const plusplayer::ErrorType &error_code, void *user_data);
  static void OnErrorMsg(const plusplayer::ErrorType &error_code,
                         const char *error_msg, void *user_data);
  static void OnDrmInitData(int *drm_andle, unsigned int len,
                            unsigned char *pssh_data,
                            plusplayer::TrackType type, void *user_data);
  static void OnAdaptiveStreamingControlEvent(
      const plusplayer::StreamingMessageType &type,
      const plusplayer::MessageParam &msg, void *user_data);
  static void OnClosedCaptionData(std::unique_ptr<char[]> data, const int size,
                                  void *user_data);
  static void OnCueEvent(const char *cue_data, void *user_data);
  static void OnDateRangeEvent(const char *date_range_data, void *user_data);
  static void OnStopReachEvent(bool stop_reach, void *user_data);
  static void OnCueOutContEvent(const char *cue_out_cont_data, void *user_data);
  static void OnChangeSourceDone(bool ret, void *user_data);
  static void OnStateChangedToPlaying(void *user_data);

  PlusplayerRef player_ = nullptr;
  PlusplayerListener listener_;
  std::unique_ptr<DrmManager> drm_manager_;
  bool is_buffering_ = false;
  bool is_prebuffer_mode_ = false;
  SeekCompletedCallback on_seek_completed_;
  std::unique_ptr<plusplayer::PlayerMemento> memento_ = nullptr;
  std::string url_;
  std::unique_ptr<DeviceProxy> device_proxy_ = nullptr;
  CreateMessage create_message_;
};

#endif  // FLUTTER_PLUGIN_PLUS_PLAYER_H_
