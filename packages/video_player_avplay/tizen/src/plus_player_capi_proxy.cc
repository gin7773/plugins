// Copyright 2025 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "plus_player_capi_proxy.h"

#include <dlfcn.h>

#include <utility>

#include "log.h"

// Template function for dynamic library function calls with custom error values
template <typename FuncType, typename... Args>
int CallPlusplayerFunction(void* handle, const char* func_name,
                           Args&&... args) {
  if (!handle) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return PLUSPLAYER_ERROR_TYPE_UNKNOWN;
  }

  FuncType func = reinterpret_cast<FuncType>(dlsym(handle, func_name));
  if (!func) {
    LOG_ERROR("Failed to find %s function.", func_name);
    return PLUSPLAYER_ERROR_TYPE_UNKNOWN;
  }

  return func(std::forward<Args>(args)...);
}

PlusPlayerCapiProxy::PlusPlayerCapiProxy() {
  plusplayer_capi_handle_ = dlopen("libplusplayer_tvplus.so", RTLD_LAZY);
  if (plusplayer_capi_handle_ == nullptr) {
    LOG_ERROR("Failed to open libplusplayer_tvplus.so");
  }
}

PlusPlayerCapiProxy::~PlusPlayerCapiProxy() {
  if (plusplayer_capi_handle_) {
    dlclose(plusplayer_capi_handle_);
    plusplayer_capi_handle_ = nullptr;
  }
}

plusplayer_h PlusPlayerCapiProxy::plusplayer_capi_create(void) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return nullptr;
  }

  FunPlusplayerCapiCreate plusplayer_capi_create =
      reinterpret_cast<FunPlusplayerCapiCreate>(
          dlsym(plusplayer_capi_handle_, "plusplayer_create"));

  if (!plusplayer_capi_create) {
    LOG_ERROR("Failed to find plusplayer_create function.");
    return nullptr;
  }

  return plusplayer_capi_create();
}

int PlusPlayerCapiProxy::plusplayer_capi_open(plusplayer_h handle,
                                              const char* uri) {
  return CallPlusplayerFunction<FunPlusplayerCapiOpen>(
      plusplayer_capi_handle_, "plusplayer_open", handle, uri);
  // if (!plusplayer_capi_handle_) {
  //   LOG_ERROR("power_state_handle_ is invalid.");
  //   return PLUSPLAYER_ERROR_TYPE_UNKNOWN;
  // }

  // FunPlusplayerCapiOpen plusplayer_capi_open =
  //     reinterpret_cast<FunPlusplayerCapiOpen>(
  //         dlsym(plusplayer_capi_handle_, "plusplayer_open"));

  // if (!plusplayer_capi_open) {
  //   LOG_ERROR("Failed to find plusplayer_open function.");
  //   return PLUSPLAYER_ERROR_TYPE_UNKNOWN;
  // }

  // return plusplayer_capi_open(handle, uri);
}

plusplayer_state_e PlusPlayerCapiProxy::plusplayer_capi_get_state(
    plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return static_cast<plusplayer_state_e>(-1);
  }

  FunPlusplayerCapiGetState plusplayer_capi_get_state =
      reinterpret_cast<FunPlusplayerCapiGetState>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_state"));

  if (!plusplayer_capi_get_state) {
    LOG_ERROR("Failed to find plusplayer_get_state function.");
    return static_cast<plusplayer_state_e>(-1);
  }

  return plusplayer_capi_get_state(handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_property(
    plusplayer_h handle, plusplayer_property_e property, const char* value) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetProperty>(
      plusplayer_capi_handle_, "plusplayer_set_property", handle, property,
      value);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_app_id(plusplayer_h handle,
                                                    const char* app_id) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetAppId>(
      plusplayer_capi_handle_, "plusplayer_set_app_id", handle, app_id);
}

void PlusPlayerCapiProxy::plusplayer_capi_set_prebuffer_mode(
    plusplayer_h handle, bool prebuffer_mode) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return;
  }

  FunPlusplayerCapiSetPrebufferMode plusplayer_capi_set_prebuffer_mode =
      reinterpret_cast<FunPlusplayerCapiSetPrebufferMode>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_prebuffer_mode"));

  if (!plusplayer_capi_set_prebuffer_mode) {
    LOG_ERROR("Failed to find plusplayer_set_prebuffer_mode function.");
    return;
  }

  return plusplayer_capi_set_prebuffer_mode(handle, prebuffer_mode);
}

int PlusPlayerCapiProxy::plusplayer_capi_prepare(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiPrepare>(
      plusplayer_capi_handle_, "plusplayer_prepare", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_start(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiStart>(
      plusplayer_capi_handle_, "plusplayer_start", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_stop(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiStop>(
      plusplayer_capi_handle_, "plusplayer_stop", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_close(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiClose>(
      plusplayer_capi_handle_, "plusplayer_close", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_destroy(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiDestroy>(
      plusplayer_capi_handle_, "plusplayer_destroy", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_display(
    plusplayer_h handle, plusplayer_display_type_e type, void* window) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDisplay>(
      plusplayer_capi_handle_, "plusplayer_set_display", handle, type, window);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_display_subsurface(
    plusplayer_h handle, plusplayer_display_type_e type, uint32_t surface_id,
    plusplayer_geometry_s roi) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDisplaySubsurface>(
      plusplayer_capi_handle_, "plusplayer_set_display_subsurface", handle,
      type, surface_id, roi);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_prepare_async_done_cb(
    plusplayer_h handle, plusplayer_prepare_async_done_cb prepare_async_done_cb,
    void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetPrepareAsyncDoneCb>(
      plusplayer_capi_handle_, "plusplayer_set_prepare_async_done_cb", handle,
      prepare_async_done_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_resource_conflicted_cb(
    plusplayer_h handle,
    plusplayer_resource_conflicted_cb resource_conflicted_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetResourceConflictedCb>(
      plusplayer_capi_handle_, "plusplayer_set_resource_conflicted_cb", handle,
      resource_conflicted_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_eos_cb(plusplayer_h handle,
                                                    plusplayer_eos_cb eos_cb,
                                                    void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetEosCb>(
      plusplayer_capi_handle_, "plusplayer_set_eos_cb", handle, eos_cb,
      userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_buffer_status_cb(
    plusplayer_h handle, plusplayer_buffer_status_cb buffer_status_cb,
    void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetBufferStatusCb>(
      plusplayer_capi_handle_, "plusplayer_set_buffer_status_cb", handle,
      buffer_status_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_error_cb(
    plusplayer_h handle, plusplayer_error_cb error_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetErrorCb>(
      plusplayer_capi_handle_, "plusplayer_set_error_cb", handle, error_cb,
      userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_error_msg_cb(
    plusplayer_h handle, plusplayer_error_msg_cb error_msg_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetErrorMsgCb>(
      plusplayer_capi_handle_, "plusplayer_set_error_msg_cb", handle,
      error_msg_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_seek_done_cb(
    plusplayer_h handle, plusplayer_seek_done_cb seek_done_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetSeekDoneCb>(
      plusplayer_capi_handle_, "plusplayer_set_seek_done_cb", handle,
      seek_done_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_subtitle_updated_cb(
    plusplayer_h handle, plusplayer_subtitle_updated_cb subtitle_updated_cb,
    void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetSubtitleUpdatedCb>(
      plusplayer_capi_handle_, "plusplayer_set_subtitle_updated_cb", handle,
      subtitle_updated_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_ad_event_cb(
    plusplayer_h handle, plusplayer_ad_event_cb ad_event_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetAdEventCb>(
      plusplayer_capi_handle_, "plusplayer_set_ad_event_cb", handle,
      ad_event_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_prepare_async(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiPrepareAsync>(
      plusplayer_capi_handle_, "plusplayer_prepare_async", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_pause(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiPause>(
      plusplayer_capi_handle_, "plusplayer_pause", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_resume(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiResume>(
      plusplayer_capi_handle_, "plusplayer_resume", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_seek(plusplayer_h handle,
                                              uint64_t time) {
  return CallPlusplayerFunction<FunPlusplayerCapiSeek>(
      plusplayer_capi_handle_, "plusplayer_seek", handle, time);
}

int PlusPlayerCapiProxy::plusplayer_capi_suspend(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiSuspend>(
      plusplayer_capi_handle_, "plusplayer_suspend", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_restore(
    plusplayer_h handle, plusplayer_state_e target_state) {
  return CallPlusplayerFunction<FunPlusplayerCapiRestore>(
      plusplayer_capi_handle_, "plusplayer_restore", handle, target_state);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_playing_time(
    plusplayer_h handle, uint64_t* cur_time_ms) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetPlayingTime>(
      plusplayer_capi_handle_, "plusplayer_get_playing_time", handle,
      cur_time_ms);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_display_mode(
    plusplayer_h handle, plusplayer_display_mode_e mode) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDisplayMode>(
      plusplayer_capi_handle_, "plusplayer_set_display_mode", handle, mode);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_display_roi(
    plusplayer_h handle, plusplayer_geometry_s roi) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDisplayRoi>(
      plusplayer_capi_handle_, "plusplayer_set_display_roi", handle, roi);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_display_rotation(
    plusplayer_h handle, plusplayer_display_rotation_type_e rotation) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDisplayRotation>(
      plusplayer_capi_handle_, "plusplayer_set_display_rotation", handle,
      rotation);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_buffer_config(plusplayer_h handle,
                                                           const char* config,
                                                           int amount) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetBufferConfig>(
      plusplayer_capi_handle_, "plusplayer_set_buffer_config", handle, config,
      amount);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_duration(plusplayer_h handle,
                                                      int64_t* duration_ms) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetDuration>(
      plusplayer_capi_handle_, "plusplayer_get_duration", handle, duration_ms);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_playback_rate(
    plusplayer_h handle, const double playback_rate) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetPlaybackRate>(
      plusplayer_capi_handle_, "plusplayer_set_playback_rate", handle,
      playback_rate);
}

int PlusPlayerCapiProxy::plusplayer_capi_deactivate_audio(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiDeactivateAudio>(
      plusplayer_capi_handle_, "plusplayer_deactivate_audio", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_activate_audio(plusplayer_h handle) {
  return CallPlusplayerFunction<FunPlusplayerCapiActivateAudio>(
      plusplayer_capi_handle_, "plusplayer_activate_audio", handle);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_property(
    plusplayer_h handle, plusplayer_property_e property, char** value) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetProperty>(
      plusplayer_capi_handle_, "plusplayer_get_property", handle, property,
      value);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_count(
    plusplayer_h handle, plusplayer_track_type_e track_type, int* count) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackCount>(
      plusplayer_capi_handle_, "plusplayer_get_track_count", handle, track_type,
      count);
}

int PlusPlayerCapiProxy::plusplayer_capi_select_track(
    plusplayer_h handle, plusplayer_track_type_e type, int index) {
  return CallPlusplayerFunction<FunPlusplayerCapiSelectTrack>(
      plusplayer_capi_handle_, "plusplayer_select_track", handle, type, index);
}

const char* PlusPlayerCapiProxy::plusplayer_capi_get_track_language_code(
    plusplayer_h handle, plusplayer_track_type_e type, int index) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return nullptr;
  }

  FunPlusplayerCapiGetTrackLanguageCode
      plusplayer_capi_get_track_language_code =
          reinterpret_cast<FunPlusplayerCapiGetTrackLanguageCode>(dlsym(
              plusplayer_capi_handle_, "plusplayer_get_track_language_code"));

  if (!plusplayer_capi_get_track_language_code) {
    LOG_ERROR("Failed to find plusplayer_get_track_language_code function.");
    return nullptr;
  }

  return plusplayer_capi_get_track_language_code(handle, type, index);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_app_info(
    plusplayer_h handle, const plusplayer_app_info_s* app_info) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetAppInfo>(
      plusplayer_capi_handle_, "plusplayer_set_app_info", handle, app_info);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_drm(
    plusplayer_h handle, plusplayer_drm_property_s drm_property) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDrm>(
      plusplayer_capi_handle_, "plusplayer_set_drm", handle, drm_property);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_drm_init_data_cb(
    plusplayer_h handle, plusplayer_drm_init_data_cb drm_init_data_callback,
    void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetDrmInitDataCb>(
      plusplayer_capi_handle_, "plusplayer_set_drm_init_data_cb", handle,
      drm_init_data_callback, userdata);
}

int PlusPlayerCapiProxy::
    plusplayer_capi_set_adaptive_streaming_control_event_cb(
        plusplayer_h handle,
        plusplayer_adaptive_streaming_control_event_cb
            adaptive_streaming_control_event_cb,
        void* userdata) {
  return CallPlusplayerFunction<
      FunPlusplayerCapiSetAdaptiveStreamingControlEventCb>(
      plusplayer_capi_handle_,
      "plusplayer_set_adaptive_streaming_control_event_cb", handle,
      adaptive_streaming_control_event_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_drm_license_acquired_done(
    plusplayer_h handle, plusplayer_track_type_e track_type) {
  return CallPlusplayerFunction<FunPlusplayerCapiDrmLicenseAcquiredDone>(
      plusplayer_capi_handle_, "plusplayer_drm_license_acquired_done", handle,
      track_type);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_subtitle_path(plusplayer_h handle,
                                                           const char* uri) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetSubtitlePath>(
      plusplayer_capi_handle_, "plusplayer_set_subtitle_path", handle, uri);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_video_stillmode(
    plusplayer_h handle, plusplayer_still_mode_e stillmode) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetVideoStillmode>(
      plusplayer_capi_handle_, "plusplayer_set_video_stillmode", handle,
      stillmode);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_alternative_video_resource(
    plusplayer_h handle, unsigned int rsc_type) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetAlternativeVideoResource>(
      plusplayer_capi_handle_, "plusplayer_set_alternative_video_resource",
      handle, rsc_type);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_foreach_track(
    plusplayer_h handle, plusplayer_track_cb track_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetForeachTrack>(
      plusplayer_capi_handle_, "plusplayer_get_foreach_track", handle, track_cb,
      userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_foreach_active_track(
    plusplayer_h handle, plusplayer_track_cb track_cb, void* userdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetForeachActiveTrack>(
      plusplayer_capi_handle_, "plusplayer_get_foreach_active_track", handle,
      track_cb, userdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_cookie(plusplayer_h handle,
                                                    const char* cookie) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetCookie>(
      plusplayer_capi_handle_, "plusplayer_set_cookie", handle, cookie);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_user_agent(
    plusplayer_h handle, const char* user_agent) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetUserAgent>(
      plusplayer_capi_handle_, "plusplayer_set_user_agent", handle, user_agent);
}

int PlusPlayerCapiProxy::plusplayer_capi_set_resume_time(
    plusplayer_h handle, uint64_t resume_time_ms) {
  return CallPlusplayerFunction<FunPlusplayerCapiSetResumeTime>(
      plusplayer_capi_handle_, "plusplayer_set_resume_time", handle,
      resume_time_ms);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_index(
    plusplayer_track_h track, int* track_index) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackIndex>(
      plusplayer_capi_handle_, "plusplayer_get_track_index", track,
      track_index);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_id(plusplayer_track_h track,
                                                      int* track_id) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackId>(
      plusplayer_capi_handle_, "plusplayer_get_track_id", track, track_id);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_mimetype(
    plusplayer_track_h track, const char** track_mimetype) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackMimetype>(
      plusplayer_capi_handle_, "plusplayer_get_track_mimetype", track,
      track_mimetype);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_streamtype(
    plusplayer_track_h track, const char** track_streamtype) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackStreamtype>(
      plusplayer_capi_handle_, "plusplayer_get_track_streamtype", track,
      track_streamtype);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_container_type(
    plusplayer_track_h track, const char** track_containertype) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackContainerType>(
      plusplayer_capi_handle_, "plusplayer_get_track_container_type", track,
      track_containertype);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_type(
    plusplayer_track_h track, plusplayer_track_type_e* track_type) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackType>(
      plusplayer_capi_handle_, "plusplayer_get_track_type", track, track_type);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_codec_data(
    plusplayer_track_h track, const char** track_codecdata) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackCodecData>(
      plusplayer_capi_handle_, "plusplayer_get_track_codec_data", track,
      track_codecdata);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_codec_tag(
    plusplayer_track_h track, unsigned int* track_codectag) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackCodecTag>(
      plusplayer_capi_handle_, "plusplayer_get_track_codec_tag", track,
      track_codectag);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_codec_data_len(
    plusplayer_track_h track, int* track_codecdatalen) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackCodecDataLen>(
      plusplayer_capi_handle_, "plusplayer_get_track_codec_data_len", track,
      track_codecdatalen);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_width(
    plusplayer_track_h track, int* track_width) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackWidth>(
      plusplayer_capi_handle_, "plusplayer_get_track_width", track,
      track_width);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_height(
    plusplayer_track_h track, int* track_height) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackHeight>(
      plusplayer_capi_handle_, "plusplayer_get_track_height", track,
      track_height);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_maxwidth(
    plusplayer_track_h track, int* track_maxwidth) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackMaxwidth>(
      plusplayer_capi_handle_, "plusplayer_get_track_maxwidth", track,
      track_maxwidth);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_maxheight(
    plusplayer_track_h track, int* track_maxheight) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackMaxheight>(
      plusplayer_capi_handle_, "plusplayer_get_track_maxheight", track,
      track_maxheight);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_framerate_num(
    plusplayer_track_h track, int* track_framerate_num) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackFramerateNum>(
      plusplayer_capi_handle_, "plusplayer_get_track_framerate_num", track,
      track_framerate_num);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_framerate_den(
    plusplayer_track_h track, int* track_framerate_den) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackFramerateDen>(
      plusplayer_capi_handle_, "plusplayer_get_track_framerate_den", track,
      track_framerate_den);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_sample_rate(
    plusplayer_track_h track, int* track_sample_rate) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackSampleRate>(
      plusplayer_capi_handle_, "plusplayer_get_track_sample_rate", track,
      track_sample_rate);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_sample_format(
    plusplayer_track_h track, int* track_sample_format) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackSampleFormat>(
      plusplayer_capi_handle_, "plusplayer_get_track_sample_format", track,
      track_sample_format);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_channels(
    plusplayer_track_h track, int* track_channels) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackChannels>(
      plusplayer_capi_handle_, "plusplayer_get_track_channels", track,
      track_channels);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_version(
    plusplayer_track_h track, int* track_version) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackVersion>(
      plusplayer_capi_handle_, "plusplayer_get_track_version", track,
      track_version);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_layer(
    plusplayer_track_h track, int* track_layer) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackLayer>(
      plusplayer_capi_handle_, "plusplayer_get_track_layer", track,
      track_layer);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_bits_per_sample(
    plusplayer_track_h track, int* track_bits_per_sample) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackBitsPerSample>(
      plusplayer_capi_handle_, "plusplayer_get_track_bits_per_sample", track,
      track_bits_per_sample);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_block_align(
    plusplayer_track_h track, int* track_block_align) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackBlockAlign>(
      plusplayer_capi_handle_, "plusplayer_get_track_block_align", track,
      track_block_align);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_bitrate(
    plusplayer_track_h track, int* track_bitrate) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackBitrate>(
      plusplayer_capi_handle_, "plusplayer_get_track_bitrate", track,
      track_bitrate);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_endianness(
    plusplayer_track_h track, int* track_endianness) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackEndianness>(
      plusplayer_capi_handle_, "plusplayer_get_track_endianness", track,
      track_endianness);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_is_signed(
    plusplayer_track_h track, bool* track_is_signed) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackIsSigned>(
      plusplayer_capi_handle_, "plusplayer_get_track_is_signed", track,
      track_is_signed);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_active(
    plusplayer_track_h track, bool* track_active) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackActive>(
      plusplayer_capi_handle_, "plusplayer_get_track_active", track,
      track_active);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_lang_code(
    plusplayer_track_h track, const char** track_lang_code) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackLangCode>(
      plusplayer_capi_handle_, "plusplayer_get_track_lang_code", track,
      track_lang_code);
}

int PlusPlayerCapiProxy::plusplayer_capi_get_track_subtitle_format(
    plusplayer_track_h track, const char** track_subtitle_format) {
  return CallPlusplayerFunction<FunPlusplayerCapiGetTrackSubtitleFormat>(
      plusplayer_capi_handle_, "plusplayer_get_track_subtitle_format", track,
      track_subtitle_format);
}
