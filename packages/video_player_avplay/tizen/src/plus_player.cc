// Copyright 2023 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "plus_player.h"

#include <app_manager.h>
#include <system_info.h>

#include <sstream>

#include "log.h"
#include "plusplayer_capi/track.h"
#include "plus_player_capi_proxy.h"
#include "plus_player_util.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace video_player_avplay_tizen {

static std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

PlusPlayer::PlusPlayer(flutter::BinaryMessenger *messenger,
                       FlutterDesktopViewRef flutter_view)
    : VideoPlayer(messenger, flutter_view) {
  memento_ = std::make_unique<PlayerMemento>();
  device_proxy_ = std::make_unique<DeviceProxy>();
  plusplayer_capi_proxy_ = std::make_unique<PlusPlayerCapiProxy>();
}

PlusPlayer::~PlusPlayer() {
  if (player_) {
    plusplayer_capi_proxy_->plusplayer_capi_stop(player_);
    plusplayer_capi_proxy_->plusplayer_capi_close(player_);
    UnRegisterCallback();
    plusplayer_capi_proxy_->plusplayer_capi_destroy(player_);
    player_ = nullptr;
  }

  if (drm_manager_) {
    drm_manager_->ReleaseDrmSession();
  }
}

void PlusPlayer::RegisterCallback() {
  plusplayer_capi_proxy_->plusplayer_capi_set_prepare_async_done_cb(
      player_, OnPrepareDone, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_error_cb(player_, OnError, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_error_msg_cb(player_, OnErrorMsg,
                                                           this);
  plusplayer_capi_proxy_->plusplayer_capi_set_resource_conflicted_cb(
      player_, OnResourceConflicted, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_eos_cb(player_, OnEos, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_seek_done_cb(player_, OnSeekDone,
                                                           this);
  plusplayer_capi_proxy_->plusplayer_capi_set_buffer_status_cb(
      player_, OnBufferStatus, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_drm_init_data_cb(
      player_, OnDrmInitData, this);
  plusplayer_capi_proxy_
      ->plusplayer_capi_set_adaptive_streaming_control_event_cb(
          player_, OnAdaptiveStreamingControlEvent, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_ad_event_cb(
      player_, OnADEventFromDash, this);
  // plusplayer_capi_proxy_->plusplayer_capi_set_subtitle_updated_cb(player_,
  // OnSubtitleData, this);
}

void PlusPlayer::UnRegisterCallback() {
  plusplayer_capi_proxy_->plusplayer_capi_set_buffer_status_cb(player_, nullptr,
                                                               this);
  plusplayer_capi_proxy_
      ->plusplayer_capi_set_adaptive_streaming_control_event_cb(player_,
                                                                nullptr, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_eos_cb(player_, nullptr, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_drm_init_data_cb(player_, nullptr,
                                                               this);
  plusplayer_capi_proxy_->plusplayer_capi_set_error_cb(player_, nullptr, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_error_msg_cb(player_, nullptr,
                                                           this);
  plusplayer_capi_proxy_->plusplayer_capi_set_prepare_async_done_cb(
      player_, nullptr, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_seek_done_cb(player_, nullptr,
                                                           this);
  plusplayer_capi_proxy_->plusplayer_capi_set_subtitle_updated_cb(
      player_, nullptr, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_resource_conflicted_cb(
      player_, nullptr, this);
  plusplayer_capi_proxy_->plusplayer_capi_set_ad_event_cb(player_, nullptr,
                                                          this);
}

bool PlusPlayer::GetMemento(PlayerMemento *memento) {
  uint64_t playing_time;
  if (plusplayer_capi_proxy_->plusplayer_capi_get_playing_time(player_,
                                                               &playing_time) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    return false;
  }
  memento->playing_time = playing_time;
  memento->is_live = IsLive();
  memento->state = plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  return true;
}

int64_t PlusPlayer::Create(const std::string &uri,
                           const CreateMessage &create_message) {
  LOG_INFO("[PlusPlayer] Create player.");

  player_ = plusplayer_capi_proxy_->plusplayer_capi_create();

  if (!player_) {
    LOG_ERROR("[PlusPlayer] Fail to create player.");
    return -1;
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_open(player_, uri.c_str()) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Fail to open uri :  %s.", uri.c_str());
    return -1;
  }
  url_ = uri;
  create_message_ = create_message;
  LOG_INFO("[PlusPlayer] Uri: %s", uri.c_str());

  if (create_message.streaming_property() != nullptr &&
      !create_message.streaming_property()->empty()) {
    for (const auto &[key, value] : *create_message.streaming_property()) {
      SetStreamingProperty(std::get<std::string>(key),
                           std::get<std::string>(value));
    }
  }

  char *appId = nullptr;
  int ret = app_manager_get_app_id(getpid(), &appId);
  if (ret != APP_MANAGER_ERROR_NONE) {
    LOG_ERROR("[PlusPlayer] Fail to get app id: %s.", get_error_message(ret));
    return -1;
  }
  plusplayer_capi_proxy_->plusplayer_capi_set_app_id(player_, appId);
  free(appId);

  RegisterCallback();

  int64_t drm_type = flutter_common::GetValue(create_message.drm_configs(),
                                              "drmType", (int64_t)0);
  std::string license_server_url = flutter_common::GetValue(
      create_message.drm_configs(), "licenseServerUrl", std::string());
  if (drm_type != 0) {
    if (!SetDrm(uri, drm_type, license_server_url)) {
      LOG_ERROR("[PlusPlayer] Fail to set drm.");
      return -1;
    }
  }

  if (!SetDisplay()) {
    LOG_ERROR("[PlusPlayer] Fail to set display.");
    return -1;
  }

  SetDisplayRoi(0, 0, 1, 1);

  bool is_prebuffer_mode = flutter_common::GetValue(
      create_message.player_options(), "prebufferMode", false);
  if (is_prebuffer_mode) {
    plusplayer_capi_proxy_->plusplayer_capi_set_prebuffer_mode(player_, true);
    is_prebuffer_mode_ = true;
  }

  int64_t start_position = flutter_common::GetValue(
      create_message.player_options(), "startPosition", (int64_t)0);
  if (start_position > 0) {
    LOG_INFO("[PlusPlayer] Start position: %lld", start_position);
    if (plusplayer_capi_proxy_->plusplayer_capi_seek(player_, start_position) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_INFO("[PlusPlayer] Fail to seek, it's a non-seekable content");
    }
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_prepare_async(player_) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to prepare.");
    return -1;
  }
  return SetUpEventChannel();
}

void PlusPlayer::Dispose() {
  LOG_INFO("[PlusPlayer] Player disposing.");
  ClearUpEventChannel();
}

void PlusPlayer::SetDisplayRoi(int32_t x, int32_t y, int32_t width,
                               int32_t height) {
  plusplayer_geometry_s roi;
  roi.x = x;
  roi.y = y;
  roi.width = width;
  roi.height = height;
  if (int ret = plusplayer_capi_proxy_->plusplayer_capi_set_display_roi(player_,
                                                                        roi) !=
                plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to set display roi.");
  }
}

bool PlusPlayer::Play() {
  LOG_INFO("[PlusPlayer] Player starting.");

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state < plusplayer_state_e::PLUSPLAYER_STATE_TRACK_SOURCE_READY) {
    LOG_ERROR("[PlusPlayer] Player is not ready.");
    return false;
  }

  if (state <= plusplayer_state_e::PLUSPLAYER_STATE_READY) {
    if (plusplayer_capi_proxy_->plusplayer_capi_start(player_) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR("[PlusPlayer] Player fail to start.");
      return false;
    }
    return true;
  } else if (state == plusplayer_state_e::PLUSPLAYER_STATE_PAUSED) {
    if (plusplayer_capi_proxy_->plusplayer_capi_resume(player_) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR("[PlusPlayer] Player fail to resume.");
      return false;
    }
    return true;
  }
  return false;
}

bool PlusPlayer::Activate() {
  // if (!::Activate(player_, plusplayer::kTrackTypeVideo)) {
  //   LOG_ERROR("[PlusPlayer] Fail to activate video.");
  //   return false;
  // }
  if (plusplayer_capi_proxy_->plusplayer_capi_activate_audio(player_) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Fail to activate audio.");
    return false;
  }
  // if (!::Activate(player_, plusplayer::kTrackTypeSubtitle)) {
  //   LOG_ERROR("[PlusPlayer] Fail to activate subtitle.");
  // }

  return true;
}

bool PlusPlayer::Deactivate() {
  if (is_prebuffer_mode_) {
    plusplayer_capi_proxy_->plusplayer_capi_stop(player_);
    return true;
  }

  // if (!::Deactivate(player_, plusplayer::kTrackTypeVideo)) {
  //   LOG_ERROR("[PlusPlayer] Fail to deactivate video.");
  //   return false;
  // }
  if (plusplayer_capi_proxy_->plusplayer_capi_deactivate_audio(player_) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Fail to deactivate audio.");
    return false;
  }
  // if (!::Deactivate(player_, plusplayer::kTrackTypeSubtitle)) {
  //   LOG_ERROR("[PlusPlayer] Fail to deactivate subtitle.");
  // }

  return true;
}

bool PlusPlayer::Pause() {
  LOG_INFO("[PlusPlayer] Player pausing.");

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state < plusplayer_state_e::PLUSPLAYER_STATE_READY) {
    LOG_ERROR("[PlusPlayer] Player is not ready.");
    return false;
  }

  if (state != plusplayer_state_e::PLUSPLAYER_STATE_PLAYING) {
    LOG_INFO("[PlusPlayer] Player not playing.");
    return false;
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_pause(player_) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to pause.");
    return false;
  }

  SendIsPlayingState(false);
  return true;
}

bool PlusPlayer::SetLooping(bool is_looping) {
  LOG_ERROR("[PlusPlayer] Not support to set looping.");
  return true;
}

bool PlusPlayer::SetVolume(double volume) {
  if (plusplayer_capi_proxy_->plusplayer_capi_get_state(player_) <
      plusplayer_state_e::PLUSPLAYER_STATE_PLAYING) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state");
    return false;
  }
  // dart api volume range[0,1], plusplaer volume range[0,100]
  int new_volume = volume * 100;
  LOG_INFO("[PlusPlayer] Volume: %d", new_volume);
  // if (!::SetVolume(player_, new_volume)) {
  //   LOG_ERROR("[PlusPlayer] Fail to set volume.");
  //   return false;
  // }
  return true;
}

bool PlusPlayer::SetPlaybackSpeed(double speed) {
  LOG_INFO("[PlusPlayer] Speed: %f", speed);

  if (plusplayer_capi_proxy_->plusplayer_capi_get_state(player_) <=
      plusplayer_state_e::PLUSPLAYER_STATE_IDLE) {
    LOG_ERROR("[PlusPlayer] Player is not prepared.");
    return false;
  }
  if (plusplayer_capi_proxy_->plusplayer_capi_set_playback_rate(player_,
                                                                speed) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to set playback rate.");
    return false;
  }
  return true;
}

bool PlusPlayer::SeekTo(int64_t position, SeekCompletedCallback callback) {
  LOG_INFO("[PlusPlayer] Seek to position: %lld", position);

  if (plusplayer_capi_proxy_->plusplayer_capi_get_state(player_) <
      plusplayer_state_e::PLUSPLAYER_STATE_READY) {
    LOG_ERROR("[PlusPlayer] Player is not ready.");
    return false;
  }

  if (on_seek_completed_) {
    LOG_ERROR("[PlusPlayer] Player is already seeking.");
    return false;
  }

  on_seek_completed_ = std::move(callback);
  if (plusplayer_capi_proxy_->plusplayer_capi_seek(player_, position) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    on_seek_completed_ = nullptr;
    LOG_ERROR("[PlusPlayer] Player fail to seek.");
    return false;
  }

  return true;
}

int64_t PlusPlayer::GetPosition() {
  uint64_t position = 0;
  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state == plusplayer_state_e::PLUSPLAYER_STATE_PLAYING ||
      state == plusplayer_state_e::PLUSPLAYER_STATE_PAUSED) {
    if (plusplayer_capi_proxy_->plusplayer_capi_get_playing_time(player_,
                                                                 &position) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR("[PlusPlayer] Player fail to get the current playing time.");
    }
  }
  return static_cast<int64_t>(position);
}

bool PlusPlayer::IsLive() {
  char *is_live;
  if (plusplayer_capi_proxy_->plusplayer_capi_get_property(
          player_, plusplayer_property_e::PLUSPLAYER_PROPERTY_IS_LIVE,
          &is_live) != plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to get property[IS_LIVE].");
    return false;
  }

  std::string is_live_str(is_live);
  free(is_live);
  return is_live_str == "TRUE";
}

std::pair<int64_t, int64_t> PlusPlayer::GetLiveDuration() {
  char *live_duration;
  if (plusplayer_capi_proxy_->plusplayer_capi_get_property(
          player_, plusplayer_property_e::PLUSPLAYER_PROPERTY_LIVE_DURATION,
          &live_duration) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to get property[LIVE_DURATION].");
    return std::make_pair(0, 0);
  }

  std::string live_duration_str(live_duration);
  free(live_duration);
  std::vector<std::string> time_vec = split(live_duration_str, '|');
  return std::make_pair(std::stoll(time_vec[0]), std::stoll(time_vec[1]));
}

std::pair<int64_t, int64_t> PlusPlayer::GetDuration() {
  if (IsLive()) {
    return GetLiveDuration();
  } else {
    int64_t duration = 0;
    if (plusplayer_capi_proxy_->plusplayer_capi_get_duration(player_,
                                                             &duration) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR("[PlusPlayer] Player fail to get the duration.");
      return std::make_pair(0, 0);
    }
    return std::make_pair(0, duration);
  }
}

void PlusPlayer::GetVideoSize(int32_t *width, int32_t *height) {
  if (plusplayer_capi_proxy_->plusplayer_capi_get_state(player_) >=
      plusplayer_state_e::PLUSPLAYER_STATE_TRACK_SOURCE_READY) {
    plusplayer_track_h video_track = nullptr;
    if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
            player_, GetTrackVideo, &video_track) !=
            plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE ||
        !video_track) {
      LOG_ERROR("[PlusPlayer] Player fail to get video track.");
    }

    if (plusplayer_capi_proxy_->plusplayer_capi_get_track_width(video_track,
                                                                width) !=
            plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE ||
        plusplayer_capi_proxy_->plusplayer_capi_get_track_height(video_track,
                                                                 height) !=
            plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR("[PlusPlayer] Player fail to get video size.");
    } else {
      LOG_INFO("[PlusPlayer] Video width: %d, height: %d.", *width, *height);
    }
  }
}

bool PlusPlayer::IsReady() {
  return plusplayer_state_e::PLUSPLAYER_STATE_READY ==
         plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
}

bool PlusPlayer::SetDisplay() {
  void *native_window = GetWindowHandle();
  if (!native_window) {
    LOG_ERROR("[PlusPlayer] Could not get a native window handle.");
    return false;
  }
  int x = 0, y = 0, width = 0, height = 0;
  ecore_wl2_window_proxy_->ecore_wl2_window_geometry_get(native_window, &x, &y,
                                                         &width, &height);
  uint32_t resource_id = FlutterDesktopViewGetResourceId(flutter_view_);
  if (resource_id == 0) {
    LOG_ERROR("[PlusPlayer] Fail to get resource id.");
    return false;
  }

  plusplayer_geometry_s roi;
  roi.x = x;
  roi.y = y;
  roi.width = width;
  roi.height = height;
  int ret = plusplayer_capi_proxy_->plusplayer_capi_set_display_subsurface(
      player_, plusplayer_display_type_e::PLUSPLAYER_DISPLAY_TYPE_OVERLAY,
      resource_id, roi);
  if (ret != plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to set display.");
    return false;
  }

  ret = plusplayer_capi_proxy_->plusplayer_capi_set_display_mode(
      player_, plusplayer_display_mode_e::PLUSPLAYER_DISPLAY_MODE_DST_ROI);
  if (ret != plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to set display mode.");
    return false;
  }

  return true;
}

flutter::EncodableValue PlusPlayer::ParseVideoTrack(
    plusplayer_track_h video_track) {
  flutter::EncodableMap video_track_result = {};
  video_track_result.insert_or_assign(flutter::EncodableValue("trackType"),
                                      flutter::EncodableValue("video"));
  int track_index;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_index(video_track,
                                                          &track_index);
  video_track_result.insert_or_assign(flutter::EncodableValue("trackId"),
                                      flutter::EncodableValue(track_index));

  const char *track_mimetype;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_mimetype(video_track,
                                                             &track_mimetype);
  video_track_result.insert_or_assign(flutter::EncodableValue("mimetype"),
                                      flutter::EncodableValue(track_mimetype));

  int track_width;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_width(video_track,
                                                          &track_width);
  video_track_result.insert_or_assign(flutter::EncodableValue("width"),
                                      flutter::EncodableValue(track_width));

  int track_height;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_height(video_track,
                                                           &track_height);
  video_track_result.insert_or_assign(flutter::EncodableValue("height"),
                                      flutter::EncodableValue(track_height));

  int track_bitrate;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_bitrate(video_track,
                                                            &track_bitrate);
  video_track_result.insert_or_assign(flutter::EncodableValue("bitrate"),
                                      flutter::EncodableValue(track_bitrate));
  LOG_DEBUG(
      "[PlusPlayer] video track info : trackId : %d, mimetype : %s, width : "
      "%d, height : %d, birate : %d",
      track_index, track_mimetype, track_width, track_height, track_bitrate);
  return flutter::EncodableValue(video_track_result);
}

flutter::EncodableValue PlusPlayer::ParseAudioTrack(
    plusplayer_track_h audio_track) {
  flutter::EncodableMap audio_track_result = {};
  audio_track_result.insert_or_assign(flutter::EncodableValue("trackType"),
                                      flutter::EncodableValue("audio"));
  int track_index;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_index(audio_track,
                                                          &track_index);
  audio_track_result.insert_or_assign(flutter::EncodableValue("trackId"),
                                      flutter::EncodableValue(track_index));

  const char *track_mimetype;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_mimetype(audio_track,
                                                             &track_mimetype);
  audio_track_result.insert_or_assign(flutter::EncodableValue("mimetype"),
                                      flutter::EncodableValue(track_mimetype));

  const char *track_lang_code;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_lang_code(audio_track,
                                                              &track_lang_code);
  audio_track_result.insert_or_assign(flutter::EncodableValue("language"),
                                      flutter::EncodableValue(track_lang_code));

  int track_channels;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_channels(audio_track,
                                                             &track_channels);
  audio_track_result.insert_or_assign(flutter::EncodableValue("channel"),
                                      flutter::EncodableValue(track_channels));

  int track_bitrate;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_bitrate(audio_track,
                                                            &track_bitrate);
  audio_track_result.insert_or_assign(flutter::EncodableValue("bitrate"),
                                      flutter::EncodableValue(track_bitrate));
  LOG_DEBUG(
      "[PlusPlayer] audio track info : trackId : %d, mimetype : %s, "
      "language_code : %s, channel : %d, bitrate : %d",
      track_index, track_mimetype, track_lang_code, track_channels,
      track_bitrate);
  return flutter::EncodableValue(audio_track_result);
}

flutter::EncodableValue PlusPlayer::ParseSubtitleTrack(
    plusplayer_track_h subtitle_track) {
  flutter::EncodableMap subtitle_track_result = {};
  subtitle_track_result.insert_or_assign(flutter::EncodableValue("trackType"),
                                         flutter::EncodableValue("text"));
  int track_index;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_index(subtitle_track,
                                                          &track_index);
  subtitle_track_result.insert_or_assign(flutter::EncodableValue("trackId"),
                                         flutter::EncodableValue(track_index));

  const char *track_mimetype;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_mimetype(subtitle_track,
                                                             &track_mimetype);
  subtitle_track_result.insert_or_assign(
      flutter::EncodableValue("mimetype"),
      flutter::EncodableValue(track_mimetype));

  const char *track_lang_code;
  plusplayer_capi_proxy_->plusplayer_capi_get_track_lang_code(subtitle_track,
                                                              &track_lang_code);
  subtitle_track_result.insert_or_assign(
      flutter::EncodableValue("language"),
      flutter::EncodableValue(track_lang_code));

  LOG_DEBUG(
      "[PlusPlayer] subtitle track info : trackId : %d, mimetype : %s, "
      "language_code : %s",
      track_index, track_mimetype, track_lang_code);
  return flutter::EncodableValue(subtitle_track_result);
}

flutter::EncodableList PlusPlayer::GetTrackInfo(std::string track_type) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return {};
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state < plusplayer_state_e::PLUSPLAYER_STATE_TRACK_SOURCE_READY) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state.");
    return {};
  }

  plusplayer_track_type_e type = ConvertTrackType(track_type);

  int track_count = 0;
  if (plusplayer_capi_proxy_->plusplayer_capi_get_track_count(player_, type,
                                                              &track_count) !=
          plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE ||
      track_count <= 0) {
    LOG_ERROR("[PlusPlayer] Fail to get track count.");
    return {};
  }

  flutter::EncodableList trackSelections = {};
  if (type == plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_VIDEO) {
    LOG_INFO("[PlusPlayer] Video track count: %d", track_count);
    for (size_t i = 0; i < track_count; i++) {
      plusplayer_track_h video_track = nullptr;
      if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
              player_, GetTrackVideo, &video_track) ==
              plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE &&
          video_track) {
        trackSelections.push_back(ParseVideoTrack(video_track));
      }
    }
  } else if (type == plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_AUDIO) {
    LOG_INFO("[PlusPlayer] Audio track count: %d", track_count);
    for (size_t i = 0; i < track_count; i++) {
      plusplayer_track_h audio_track = nullptr;
      if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
              player_, GetTrackAudio, &audio_track) ==
              plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE &&
          audio_track) {
        trackSelections.push_back(ParseVideoTrack(audio_track));
      }
    }
  } else if (type == plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_SUBTITLE) {
    LOG_INFO("[PlusPlayer] Subtitle track count: %d", track_count);
    for (size_t i = 0; i < track_count; i++) {
      plusplayer_track_h subtitle_track = nullptr;
      if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
              player_, GetTrackSubtitle, &subtitle_track) ==
              plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE &&
          subtitle_track) {
        trackSelections.push_back(ParseVideoTrack(subtitle_track));
      }
    }
  }

  return trackSelections;
}

bool PlusPlayer::GetTrackVideo(const plusplayer_track_h track,
                               void *user_data) {
  plusplayer_track_h *found_track =
      static_cast<plusplayer_track_h *>(user_data);
  plusplayer_track_type_e track_type;
  if (plusplayer_get_track_type(track, &track_type) ==
      PLUSPLAYER_ERROR_TYPE_NONE) {
    if (track_type == plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_VIDEO) {
      *found_track = track;
      return true;
    }
  }
  return false;
}

bool PlusPlayer::GetTrackAudio(const plusplayer_track_h track,
                               void *user_data) {
  plusplayer_track_h *found_track =
      static_cast<plusplayer_track_h *>(user_data);
  plusplayer_track_type_e track_type;
  if (plusplayer_get_track_type(track, &track_type) ==
      PLUSPLAYER_ERROR_TYPE_NONE) {
    if (track_type == plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_AUDIO) {
      *found_track = track;
      return true;
    }
  }
  return false;
}

bool PlusPlayer::GetTrackSubtitle(const plusplayer_track_h track,
                                  void *user_data) {
  plusplayer_track_h *found_track =
      static_cast<plusplayer_track_h *>(user_data);
  plusplayer_track_type_e track_type;
  if (plusplayer_get_track_type(track, &track_type) ==
      PLUSPLAYER_ERROR_TYPE_NONE) {
    if (track_type == plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_SUBTITLE) {
      *found_track = track;
      return true;
    }
  }
  return false;
}

flutter::EncodableList PlusPlayer::GetActiveTrackInfo() {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return {};
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state < plusplayer_state_e::PLUSPLAYER_STATE_TRACK_SOURCE_READY) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state.");
    return {};
  }

  flutter::EncodableList active_tracks = {};
  plusplayer_track_h video_track = nullptr, audio_track = nullptr,
                     subtitle_track = nullptr;
  if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
          player_, GetTrackVideo, &video_track) ==
          plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE &&
      video_track) {
    active_tracks.push_back(ParseVideoTrack(video_track));
  }
  if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
          player_, GetTrackAudio, &audio_track) ==
          plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE &&
      audio_track) {
    active_tracks.push_back(ParseAudioTrack(audio_track));
  }
  if (plusplayer_capi_proxy_->plusplayer_capi_get_foreach_active_track(
          player_, GetTrackSubtitle, &subtitle_track) ==
          plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE &&
      subtitle_track) {
    active_tracks.push_back(ParseSubtitleTrack(subtitle_track));
  }
  return active_tracks;
}

bool PlusPlayer::SetTrackSelection(int32_t track_id, std::string track_type) {
  LOG_INFO("[PlusPlayer] Track id is: %d,track type is: %s", track_id,
           track_type.c_str());

  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state < plusplayer_state_e::PLUSPLAYER_STATE_TRACK_SOURCE_READY) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state.");
    return false;
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_select_track(
          player_, ConvertTrackType(track_type), track_id) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to select track.");
    return false;
  }
  return true;
}

bool PlusPlayer::SetDrm(const std::string &uri, int drm_type,
                        const std::string &license_server_url) {
  drm_manager_ = std::make_unique<DrmManager>();
  if (!drm_manager_->CreateDrmSession(drm_type, true)) {
    LOG_ERROR("[PlusPlayer] Fail to create drm session.");
    return false;
  }

  int drm_handle = 0;
  if (!drm_manager_->GetDrmHandle(&drm_handle)) {
    LOG_ERROR("[PlusPlayer] Fail to get drm handle.");
    return false;
  }

  plusplayer_drm_type_e type;
  switch (drm_type) {
    case DrmManager::DrmType::DRM_TYPE_PLAYREADAY:
      type = plusplayer_drm_type_e::PLUSPLAYER_DRM_TYPE_PLAYREADY;
      break;
    case DrmManager::DrmType::DRM_TYPE_WIDEVINECDM:
      type = plusplayer_drm_type_e::PLUSPLAYER_DRM_TYPE_WIDEVINE_CDM;
      break;
    default:
      type = plusplayer_drm_type_e::PLUSPLAYER_DRM_TYPE_NONE;
      break;
  }

  plusplayer_drm_property_s property;
  property.handle = drm_handle;
  property.type = type;
  property.license_acquired_cb = reinterpret_cast<void *>(OnLicenseAcquired);
  property.license_acquired_userdata = reinterpret_cast<void *>(this);
  property.external_decryption = false;
  plusplayer_capi_proxy_->plusplayer_capi_set_drm(player_, property);

  if (license_server_url.empty()) {
    bool success = drm_manager_->SetChallenge(uri, binary_messenger_);
    if (!success) {
      LOG_ERROR("[PlusPlayer]Fail to set challenge.");
      return false;
    }
  } else {
    if (!drm_manager_->SetChallenge(uri, license_server_url)) {
      LOG_ERROR("[PlusPlayer]Fail to set challenge.");
      return false;
    }
  }
  return true;
}

std::string PlusPlayer::GetStreamingProperty(
    const std::string &streaming_property_type) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return "";
  }
  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state == plusplayer_state_e::PLUSPLAYER_STATE_NONE ||
      state == plusplayer_state_e::PLUSPLAYER_STATE_IDLE) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state[%d]", state);
    return "";
  }

  char *streaming_property_value;
  if (plusplayer_capi_proxy_->plusplayer_capi_get_property(
          player_, ConvertPropertyType(streaming_property_type),
          &streaming_property_value) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Fail to get streaming property[%s].",
              streaming_property_type.c_str());
  }
  std::string streaming_property_result(streaming_property_value);
  return streaming_property_result;
}

bool PlusPlayer::SetBufferConfig(const std::string &key, int64_t value) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state == plusplayer_state_e::PLUSPLAYER_STATE_NONE) {
    LOG_ERROR("[PlusPlayer]:Player is in invalid state[%d]", state);
    return false;
  }

  return plusplayer_capi_proxy_->plusplayer_capi_set_buffer_config(
             player_, key.c_str(), value) ==
         plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE;
}

void PlusPlayer::SetStreamingProperty(const std::string &type,
                                      const std::string &value) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return;
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state == plusplayer_state_e::PLUSPLAYER_STATE_NONE) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state[%d]", state);
    return;
  }

  if ((!create_message_.format_hint() ||
       create_message_.format_hint()->empty() ||
       *create_message_.format_hint() != "dash") &&
      (type == "OPEN_HTTP_HEADER" || type == "TOKEN" ||
       type == "UNWANTED_FRAMERATE" || type == "UNWANTED_RESOLUTION" ||
       type == "UPDATE_SAME_LANGUAGE_CODE")) {
    LOG_ERROR("[PlusPlayer] Only support streaming property type: %s for DASH!",
              type.c_str());
    return;
  }

  LOG_INFO("[PlusPlayer] SetStreamingProp: type[%s], value[%s]", type.c_str(),
           value.c_str());
  plusplayer_capi_proxy_->plusplayer_capi_set_property(
      player_, ConvertPropertyType(type), value.c_str());
}

bool PlusPlayer::SetDisplayRotate(int64_t rotation) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state == plusplayer_state_e::PLUSPLAYER_STATE_NONE) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state[%d]", state);
    return false;
  }

  LOG_INFO("[PlusPlayer] rotation: %lld", rotation);
  return plusplayer_capi_proxy_->plusplayer_capi_set_display_rotation(
             player_, ConvertDisplayRotationType(rotation)) ==
         plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE;
}

bool PlusPlayer::SetDisplayMode(int64_t display_mode) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  plusplayer_state_e state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (state == plusplayer_state_e::PLUSPLAYER_STATE_NONE) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state[%d]", state);
    return false;
  }
  LOG_INFO("[PlusPlayer] display_mode: %lld", display_mode);
  return plusplayer_capi_proxy_->plusplayer_capi_set_display_mode(
      player_, ConvertDisplayMode(display_mode));
}

bool PlusPlayer::StopAndClose() {
  LOG_INFO("[PlusPlayer] StopAndClose is called.");
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  is_buffering_ = false;
  plusplayer_state_e player_state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (player_state < plusplayer_state_e::PLUSPLAYER_STATE_READY) {
    LOG_INFO("[PlusPlayer] Player already stop, nothing to do.");
    return true;
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_stop(player_) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to stop.");
    return false;
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_close(player_) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Player fail to close.");
    return false;
  }

  if (drm_manager_) {
    drm_manager_->ReleaseDrmSession();
    drm_manager_.reset();
  }

  return true;
}

bool PlusPlayer::Suspend() {
  LOG_INFO("[PlusPlayer] Suspend is called.");

  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  if (is_prebuffer_mode_) {
    LOG_ERROR("[PlusPlayer] Player is in prebuffer mode, do nothing.");
    return true;
  }

  memento_.reset(new PlayerMemento());
  if (!GetMemento(memento_.get())) {
    LOG_ERROR("[PlusPlayer] Player fail to get memento.");
    return false;
  }
  LOG_INFO(
      "[PlusPlayer] Memento saved current player state: %d, position: %llu ms, "
      "is_live: %d",
      (int)memento_->state, memento_->playing_time, memento_->is_live);

  if (memento_->is_live) {
    memento_->playing_time = 0;
    if (!StopAndClose()) {
      LOG_ERROR("[PlusPlayer] Player is live, StopAndClose fail.");
      return false;
    }
    LOG_INFO("[PlusPlayer] Player is live: close done successfully.");
    return true;
  }

  int power_state = device_proxy_->device_power_get_state();
  if (power_state == POWER_STATE_STANDBY) {
    LOG_INFO("[PlusPlayer] Power state is standby.");
    if (!StopAndClose()) {
      LOG_ERROR("[PlusPlayer] Player need to stop and close, but failed.");
      return false;
    }
    LOG_INFO("[PlusPlayer] Standby state: close done successfully.");
    return true;
  } else {
    LOG_INFO("[PlusPlayer] Player state is not standby: %d, do nothing.",
             power_state);
  }

  plusplayer_state_e player_state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (player_state <= plusplayer_state_e::PLUSPLAYER_STATE_TRACK_SOURCE_READY) {
    if (plusplayer_capi_proxy_->plusplayer_capi_close(player_) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR("[PlusPlayer] Player close fail.");
      return false;
    }
    LOG_INFO("[PlusPlayer] Player is in invalid state[%d], just close.",
             player_state);
    return true;
  } else if (player_state != plusplayer_state_e::PLUSPLAYER_STATE_PAUSED) {
    LOG_INFO("[PlusPlayer] Player calling pause from suspend.");
    if (plusplayer_capi_proxy_->plusplayer_capi_suspend(player_) !=
        plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
      LOG_ERROR(
          "[PlusPlayer] Suspend fail, in restore player instance would be "
          "created newly.");
      if (!StopAndClose())
        LOG_ERROR("[PlusPlayer] Suspend error, player stop and close fail.");
      return false;
    }
    SendIsPlayingState(false);
  }
  return true;
}

bool PlusPlayer::Restore(const CreateMessage *restore_message,
                         int64_t resume_time) {
  LOG_INFO("[PlusPlayer] Restore is called.");
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player is not initialized.");
    return false;
  }

  plusplayer_state_e player_state =
      plusplayer_capi_proxy_->plusplayer_capi_get_state(player_);
  if (player_state != plusplayer_state_e::PLUSPLAYER_STATE_NONE &&
      player_state != plusplayer_state_e::PLUSPLAYER_STATE_PAUSED &&
      player_state != plusplayer_state_e::PLUSPLAYER_STATE_PLAYING) {
    LOG_ERROR("[PlusPlayer] Player is in invalid state[%d].", player_state);
    return false;
  }

  if (!memento_) {
    LOG_ERROR(
        "[PlusPlayer] No memento to restore. Player is in invalid state[%d]",
        player_state);
    return false;
  }

  if (is_prebuffer_mode_) {
    LOG_ERROR("[PlusPlayer] Player is in prebuffer mode, do nothing.");
    return true;
  }

  if (restore_message->uri()) {
    LOG_INFO(
        "[PlusPlayer] Restore URL is not emptpy, close the existing instance.");
    if (!StopAndClose()) {
      LOG_ERROR("[PlusPlayer] Player need to stop and close, but failed.");
      return false;
    }
    return RestorePlayer(restore_message, resume_time);
  }

  switch (player_state) {
    case plusplayer_state_e::PLUSPLAYER_STATE_NONE:
      return RestorePlayer(restore_message, resume_time);
      break;
    case plusplayer_state_e::PLUSPLAYER_STATE_PAUSED:
      if (plusplayer_capi_proxy_->plusplayer_capi_restore(player_,
                                                          memento_->state) !=
          plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
        if (!StopAndClose()) {
          LOG_ERROR("[PlusPlayer] Player need to stop and close, but failed.");
          return false;
        }
        return RestorePlayer(restore_message, resume_time);
      }
      break;
    case plusplayer_state_e::PLUSPLAYER_STATE_PLAYING:
      // might be the case that widget has called
      // restore more than once, just ignore.
      break;
    default:
      LOG_INFO(
          "[PlusPlayer] Unhandled state, dont know how to process, just return "
          "false.");
      return false;
  }
  return true;
}

bool PlusPlayer::RestorePlayer(const CreateMessage *restore_message,
                               int64_t resume_time) {
  LOG_INFO("[PlusPlayer] RestorePlayer is called.");
  LOG_INFO("[PlusPlayer] is_live: %d", memento_->is_live);

  if (restore_message->uri()) {
    LOG_INFO("[PlusPlayer] Player previous url: %s", url_.c_str());
    LOG_INFO("[PlusPlayer] Player new url: %s",
             restore_message->uri()->c_str());
    url_ = *restore_message->uri();
    create_message_ = *restore_message;
  }

  LOG_INFO("[PlusPlayer] Player previous playing time: %llu ms",
           memento_->playing_time);
  LOG_INFO("[PlusPlayer] Player new resume time: %lld ms", resume_time);
  // resume_time < 0  ==> use previous playing time
  // resume_time == 0 ==> play from beginning
  // resume_time > 0  ==> play from resume_time(Third-party settings)
  if (resume_time >= 0)
    memento_->playing_time = static_cast<uint64_t>(resume_time);

  is_restored_ = true;
  if (Create(url_, create_message_) < 0) {
    LOG_ERROR("[PlusPlayer] Fail to create player.");
    is_restored_ = false;
    return false;
  }
  if (memento_->playing_time > 0 &&
      plusplayer_capi_proxy_->plusplayer_capi_seek(player_,
                                                   memento_->playing_time) !=
          plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    LOG_ERROR("[PlusPlayer] Fail to seek.");
  }
  SetDisplayRoi(memento_->display_area.x, memento_->display_area.y,
                memento_->display_area.width, memento_->display_area.height);

  return true;
}

std::string BuildJsonString(const flutter::EncodableMap &data) {
  rapidjson::Document doc;
  doc.SetObject();
  rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

  for (const auto &pair : data) {
    std::string key_str = std::get<std::string>(pair.first);
    rapidjson::Value key(key_str.c_str(), allocator);
    if (key_str == "max-bandwidth") {
      doc.AddMember(key, rapidjson::Value(std::get<int64_t>(pair.second)),
                    allocator);
    } else {
      doc.AddMember(key,
                    rapidjson::Value(std::get<std::string>(pair.second).c_str(),
                                     allocator),
                    allocator);
    }
  }
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  return buffer.GetString();
}

std::string BuildJsonString(const flutter::EncodableList &encodable_keys) {
  rapidjson::Document doc;
  doc.SetObject();
  rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

  for (const auto &encodable_key : encodable_keys) {
    std::string key_str = std::get<std::string>(encodable_key);
    rapidjson::Value key(key_str.c_str(), allocator);
    if (key_str == "max-bandwidth") {
      doc.AddMember(key, 0, allocator);
    } else {
      doc.AddMember(key, "", allocator);
    }
  }
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  doc.Accept(writer);
  return buffer.GetString();
}

void ParseJsonString(std::string json_str,
                     const flutter::EncodableList &encodable_keys,
                     flutter::EncodableMap &output) {
  rapidjson::Document doc;
  doc.Parse(json_str.c_str());
  if (doc.HasParseError()) {
    LOG_ERROR("[PlusPlayer] Fail to parse json string.");
    return;
  }
  for (const auto &encodable_key : encodable_keys) {
    std::string key_str = std::get<std::string>(encodable_key);
    if (doc.HasMember(key_str.c_str())) {
      if (key_str == "max-bandwidth") {
        output.insert_or_assign(
            encodable_key,
            flutter::EncodableValue(doc[key_str.c_str()].GetInt64()));
      } else {
        output.insert_or_assign(
            encodable_key,
            flutter::EncodableValue(doc[key_str.c_str()].GetString()));
      }
    }
  }
}

bool PlusPlayer::SetData(const flutter::EncodableMap &data) {
  // if (!player_) {
  //   LOG_ERROR("[PlusPlayer] Player not created.");
  //   return false;
  // }
  // std::string json_data = BuildJsonString(data);
  // if (json_data.empty()) {
  //   LOG_ERROR("[PlusPlayer] json_data is empty.");
  //   return false;
  // }
  // return ::SetData(player_, json_data);
  return true;
}

flutter::EncodableMap PlusPlayer::GetData(const flutter::EncodableList &data) {
  // flutter::EncodableMap result;
  // if (!player_) {
  //   LOG_ERROR("[PlusPlayer] Player not created.");
  //   return result;
  // }
  // std::string json_data = BuildJsonString(data);
  // if (json_data.empty()) {
  //   LOG_ERROR("[PlusPlayer] json_data is empty.");
  //   return result;
  // }
  // if (!::GetData(player_, json_data)) {
  //   LOG_ERROR("[PlusPlayer] Fail to get data from player");
  //   return result;
  // }
  // ParseJsonString(json_data, data, result);
  // return result;
  return {};
}

bool PlusPlayer::UpdateDashToken(const std::string &dashToken) {
  if (!player_) {
    LOG_ERROR("[PlusPlayer] Player not created.");
    return false;
  }

  if (plusplayer_capi_proxy_->plusplayer_capi_set_property(
          player_, plusplayer_property_e::PLUSPLAYER_PROPERTY_TOKEN,
          dashToken.c_str()) !=
      plusplayer_error_type_e::PLUSPLAYER_ERROR_TYPE_NONE) {
    return false;
  }
  return true;
}

bool PlusPlayer::OnLicenseAcquired(int *drm_handle, unsigned int length,
                                   unsigned char *pssh_data, void *user_data) {
  LOG_INFO("[PlusPlayer] License acquired.");
  PlusPlayer *self = static_cast<PlusPlayer *>(user_data);

  if (self->drm_manager_) {
    return self->drm_manager_->SecurityInitCompleteCB(drm_handle, length,
                                                      pssh_data, nullptr);
  }
  return false;
}

void PlusPlayer::OnPrepareDone(bool ret, void *user_data) {
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  // if (!SetDisplayVisible(self->player_, true)) {
  //   LOG_ERROR("[PlusPlayer] Fail to set display visible.");
  // }

  if (!self->is_initialized_ && ret) {
    self->SendInitialized();
  }

  if (self->is_restored_ && ret) {
    self->SendRestored();
  }
}

void PlusPlayer::OnBufferStatus(int percent, void *user_data) {
  LOG_INFO("[PlusPlayer] Buffering percent: %d.", percent);
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  if (percent == 100) {
    self->SendBufferingEnd();
    self->is_buffering_ = false;
  } else if (!self->is_buffering_ && percent <= 5) {
    self->SendBufferingStart();
    self->is_buffering_ = true;
  } else {
    self->SendBufferingUpdate(percent);
  }
}

void PlusPlayer::OnSeekDone(void *user_data) {
  LOG_INFO("[PlusPlayer] Seek completed.");
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  if (self->on_seek_completed_) {
    self->on_seek_completed_();
    self->on_seek_completed_ = nullptr;
  }
}

void PlusPlayer::OnEos(void *user_data) {
  LOG_INFO("[PlusPlayer] Play completed.");
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  self->SendPlayCompleted();
}

// void PlusPlayer::OnSubtitleData(char *data, const int size,
//                                 const plusplayer::SubtitleType &type,
//                                 const uint64_t duration,
//                                 plusplayer::SubtitleAttributeListPtr
//                                 attr_list, void *user_data) {
//   LOG_INFO("[PlusPlayer] Subtitle updated, duration: %llu, text: %s",
//   duration,
//            data);
//   PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

//   // plusplayer::SubtitleAttributeList *attrs = attr_list.get();
//   // flutter::EncodableList attributes_list;
//   // for (auto attr = attrs->begin(); attr != attrs->end(); attr++) {
//   //   LOG_INFO("[PlusPlayer] Subtitle update: type: %d, start: %u, end: %u",
//   //            attr->type, attr->start_time, attr->stop_time);
//   //   flutter::EncodableMap attributes = {
//   //       {flutter::EncodableValue("attrType"),
//   //        flutter::EncodableValue(attr->type)},
//   //       {flutter::EncodableValue("startTime"),
//   //        flutter::EncodableValue((int64_t)attr->start_time)},
//   //       {flutter::EncodableValue("stopTime"),
//   //        flutter::EncodableValue((int64_t)attr->stop_time)},
//   //   };

//   //   switch (attr->type) {
//   //     case plusplayer::kSubAttrRegionXPos:
//   //     case plusplayer::kSubAttrRegionYPos:
//   //     case plusplayer::kSubAttrRegionWidth:
//   //     case plusplayer::kSubAttrRegionHeight:
//   //     case plusplayer::kSubAttrWindowXPadding:
//   //     case plusplayer::kSubAttrWindowYPadding:
//   //     case plusplayer::kSubAttrWindowOpacity:
//   //     case plusplayer::kSubAttrFontSize:
//   //     case plusplayer::kSubAttrFontOpacity:
//   //     case plusplayer::kSubAttrFontBgOpacity:
//   //     case plusplayer::kSubAttrWebvttCueLine:
//   //     case plusplayer::kSubAttrWebvttCueSize:
//   //     case plusplayer::kSubAttrWebvttCuePosition: {
//   //       intptr_t value_temp = reinterpret_cast<intptr_t>(attr->value);
//   //       float value_float;
//   //       std::memcpy(&value_float, &value_temp, sizeof(float));
//   //       LOG_INFO("[PlusPlayer] Subtitle update: value<float>: %f",
//   //       value_float); attributes[flutter::EncodableValue("attrValue")] =
//   //           flutter::EncodableValue((double)value_float);
//   //     } break;
//   //     case plusplayer::kSubAttrWindowLeftMargin:
//   //     case plusplayer::kSubAttrWindowRightMargin:
//   //     case plusplayer::kSubAttrWindowTopMargin:
//   //     case plusplayer::kSubAttrWindowBottomMargin:
//   //     case plusplayer::kSubAttrWindowBgColor:
//   //     case plusplayer::kSubAttrFontWeight:
//   //     case plusplayer::kSubAttrFontStyle:
//   //     case plusplayer::kSubAttrFontColor:
//   //     case plusplayer::kSubAttrFontBgColor:
//   //     case plusplayer::kSubAttrFontTextOutlineColor:
//   //     case plusplayer::kSubAttrFontTextOutlineThickness:
//   //     case plusplayer::kSubAttrFontTextOutlineBlurRadius:
//   //     case plusplayer::kSubAttrFontVerticalAlign:
//   //     case plusplayer::kSubAttrFontHorizontalAlign:
//   //     case plusplayer::kSubAttrWebvttCueLineNum:
//   //     case plusplayer::kSubAttrWebvttCueLineAlign:
//   //     case plusplayer::kSubAttrWebvttCueAlign:
//   //     case plusplayer::kSubAttrWebvttCuePositionAlign:
//   //     case plusplayer::kSubAttrWebvttCueVertical:
//   //     case plusplayer::kSubAttrTimestamp: {
//   //       int value_int = reinterpret_cast<int>(attr->value);
//   //       LOG_INFO("[PlusPlayer] Subtitle update: value<int>: %d",
//   value_int);
//   //       attributes[flutter::EncodableValue("attrValue")] =
//   //           flutter::EncodableValue(value_int);
//   //     } break;
//   //     case plusplayer::kSubAttrFontFamily:
//   //     case plusplayer::kSubAttrRawSubtitle: {
//   //       const char *value_chars = reinterpret_cast<const char
//   //       *>(attr->value); LOG_INFO("[PlusPlayer] Subtitle update:
//   value<char
//   //       *>: %s",
//   //                value_chars);
//   //       std::string value_string(value_chars);
//   //       attributes[flutter::EncodableValue("attrValue")] =
//   //           flutter::EncodableValue(value_string);
//   //     } break;
//   //     case plusplayer::kSubAttrWindowShowBg: {
//   //       uint32_t value_uint32 = reinterpret_cast<uint32_t>(attr->value);
//   //       LOG_INFO("[PlusPlayer] Subtitle update: value<uint32_t>: %u",
//   //                value_uint32);
//   //       attributes[flutter::EncodableValue("attrValue")] =
//   //           flutter::EncodableValue((int64_t)value_uint32);
//   //     } break;
//   //     default:
//   //       LOG_ERROR("[PlusPlayer] Unknown Subtitle type: %d", attr->type);
//   //       break;
//   //   }
//   //   attributes_list.push_back(flutter::EncodableValue(attributes));
//   // }
//   // self->SendSubtitleUpdate(duration, data, attributes_list);
//   self->SendSubtitleUpdate(duration, data);
// }

void PlusPlayer::OnResourceConflicted(void *user_data) {
  LOG_ERROR("[PlusPlayer] Resource conflicted.");
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  self->SendIsPlayingState(false);
}

void PlusPlayer::OnError(plusplayer_error_type_e error_code, void *user_data) {
  LOG_ERROR("[PlusPlayer] Error code: %d", error_code);
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  self->SendError("[PlusPlayer] error",
                  std::string("Error: ") + GetErrorMessage(error_code));
}

void PlusPlayer::OnErrorMsg(plusplayer_error_type_e error_code,
                            const char *error_msg, void *user_data) {
  LOG_ERROR("[PlusPlayer] Error code: %d, message: %s.", error_code, error_msg);
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  self->SendError("PlusPlayer error", std::string("Error: ") + error_msg);
}

void PlusPlayer::OnDrmInitData(int *drm_handle, unsigned int len,
                               unsigned char *pssh_data,
                               plusplayer_track_type_e type, void *user_data) {
  LOG_INFO("[PlusPlayer] Drm init completed.");
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  if (self->drm_manager_) {
    if (self->drm_manager_->SecurityInitCompleteCB(drm_handle, len, pssh_data,
                                                   nullptr)) {
      self->plusplayer_capi_proxy_->plusplayer_capi_drm_license_acquired_done(
          self->player_, type);
    }
  }
}

void PlusPlayer::OnAdaptiveStreamingControlEvent(
    plusplayer_streaming_message_type_e type, plusplayer_message_param_s *msg,
    void *user_data) {
  LOG_INFO("[PlusPlayer] Message type: %d, is DrmInitData (%d)", type,
           type == plusplayer_streaming_message_type_e::
                       PLUSPLAYER_STREAMING_MESSAGE_TYPE_DRMINITDATA);
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);

  if (type == plusplayer_streaming_message_type_e::
                  PLUSPLAYER_STREAMING_MESSAGE_TYPE_DRMINITDATA) {
    if (msg->data == nullptr || 0 == msg->size) {
      LOG_ERROR("[PlusPlayer] Empty message.");
      return;
    }

    if (self->drm_manager_) {
      self->drm_manager_->UpdatePsshData(msg->data, msg->size);
    }
  }
}

void PlusPlayer::OnClosedCaptionData(std::unique_ptr<char[]> data,
                                     const int size, void *user_data) {}

void PlusPlayer::OnCueEvent(const char *cue_data, void *user_data) {}

void PlusPlayer::OnDateRangeEvent(const char *date_range_data,
                                  void *user_data) {}

void PlusPlayer::OnStopReachEvent(bool stop_reach, void *user_data) {}

void PlusPlayer::OnCueOutContEvent(const char *cue_out_cont_data,
                                   void *user_data) {}

void PlusPlayer::OnChangeSourceDone(bool ret, void *user_data) {}

void PlusPlayer::OnStateChangedToPlaying(void *user_data) {
  PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);
  self->SendIsPlayingState(true);
}

void PlusPlayer::OnADEventFromDash(const char *ad_data, void *user_data) {
  if (!ad_data) {
    LOG_ERROR("[PlusPlayer] No ad_data.");
    return;
  }

  const char *prefix = "AD_INFO: ";
  const char *data = strstr(ad_data, prefix);
  if (!data) {
    LOG_ERROR("[PlusPlayer] Invalid ad_data.");
    return;
  }
  data += strlen(prefix);
  const_cast<char *>(data)[strlen(data) - 1] = '\0';
  LOG_INFO("[PlusPlayer] AD info: %s", data);

  rapidjson::Document doc;
  doc.Parse(data);
  if (doc.HasParseError()) {
    LOG_ERROR("[PlusPlayer] Fail to parse ad_data: %d.", doc.GetParseError());
    return;
  }

  flutter::EncodableMap ad_info = {};
  if (doc.HasMember("event") && doc["event"].IsObject()) {
    const rapidjson::Value &event = doc["event"];

    if (event.HasMember("data") && event["data"].IsObject()) {
      const rapidjson::Value &data = event["data"];

      if (data.HasMember("id") && data["id"].IsInt64()) {
        ad_info.insert_or_assign(
            flutter::EncodableValue("id"),
            flutter::EncodableValue(data["id"].GetInt64()));
      }

      if (data.HasMember("cancel_indicator") &&
          data["cancel_indicator"].IsBool()) {
        ad_info.insert_or_assign(
            flutter::EncodableValue("cancel_indicator"),
            flutter::EncodableValue(data["cancel_indicator"].GetBool()));
      }

      if (data.HasMember("start_ms") && data["start_ms"].IsInt64()) {
        ad_info.insert_or_assign(
            flutter::EncodableValue("start_ms"),
            flutter::EncodableValue(data["start_ms"].GetInt64()));
      }

      if (data.HasMember("duration_ms") && data["duration_ms"].IsInt64()) {
        ad_info.insert_or_assign(
            flutter::EncodableValue("duration_ms"),
            flutter::EncodableValue(data["duration_ms"].GetInt64()));
      }

      if (data.HasMember("end_ms") && data["end_ms"].IsInt64()) {
        ad_info.insert_or_assign(
            flutter::EncodableValue("end_ms"),
            flutter::EncodableValue(data["end_ms"].GetInt64()));
      }

      if (data.HasMember("out_of_network_indicator") &&
          data["out_of_network_indicator"].IsInt64()) {
        ad_info.insert_or_assign(
            flutter::EncodableValue("out_of_network_indicator"),
            flutter::EncodableValue(
                data["out_of_network_indicator"].GetInt64()));
      }
    }

    PlusPlayer *self = reinterpret_cast<PlusPlayer *>(user_data);
    self->SendADFromDash(ad_info);
  }
}

}  // namespace video_player_avplay_tizen
