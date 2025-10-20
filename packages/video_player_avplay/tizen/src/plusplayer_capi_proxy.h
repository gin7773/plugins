// Copyright 2025 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_PLUGIN_PLUSPLAYER_CAPI_PROXY_H_
#define FLUTTER_PLUGIN_PLUSPLAYER_CAPI_DEVICE_PROXY_H_

#include "plusplayer_capi/plusplayer_capi.h"

class PlusplayerCapiProxy {
 public:
  PlusplayerCapiProxy();
  ~PlusplayerCapiProxy();
  plusplayer_h plusplayer_capi_create(void);
  int plusplayer_capi_open(plusplayer_h handle, const char* uri);
  int plusplayer_capi_prepare(plusplayer_h handle);
  int plusplayer_capi_start(plusplayer_h handle);
  int plusplayer_capi_stop(plusplayer_h handle);
  int plusplayer_capi_close(plusplayer_h handle);
  int plusplayer_capi_destroy(plusplayer_h handle);
  plusplayer_state_e plusplayer_capi_get_state(plusplayer_h handle);
  int plusplayer_capi_set_display(plusplayer_h handle,
                                  plusplayer_display_type_e type, void* window);
  int plusplayer_capi_set_display_subsurface(plusplayer_h handle,
                                             plusplayer_display_type_e type,
                                             uint32_t surface_id,
                                             plusplayer_geometry_s roi);
  int plusplayer_capi_set_prepare_async_done_cb(
      plusplayer_h handle,
      plusplayer_prepare_async_done_cb prepare_async_done_cb, void* userdata);
  int plusplayer_capi_set_resource_conflicted_cb(
      plusplayer_h handle,
      plusplayer_resource_conflicted_cb resource_conflicted_cb, void* userdata);
  int plusplayer_capi_set_eos_cb(plusplayer_h handle, plusplayer_eos_cb eos_cb,
                                 void* userdata);
  int plusplayer_capi_set_buffer_status_cb(
      plusplayer_h handle, plusplayer_buffer_status_cb buffer_status_cb,
      void* userdata);
  int plusplayer_capi_set_error_cb(plusplayer_h handle,
                                   plusplayer_error_cb error_cb,
                                   void* userdata);
  int plusplayer_capi_set_error_msg_cb(plusplayer_h handle,
                                       plusplayer_error_msg_cb error_msg_cb,
                                       void* userdata);
  int plusplayer_capi_set_seek_done_cb(plusplayer_h handle,
                                       plusplayer_seek_done_cb seek_done_cb,
                                       void* userdata);

  int plusplayer_capi_set_subtitle_updated_cb(
      plusplayer_h handle, plusplayer_subtitle_updated_cb subtitle_updated_cb,
      void* userdata);
  int plusplayer_capi_set_ad_event_cb(plusplayer_h handle,
                                      plusplayer_ad_event_cb ad_event_cb,
                                      void* userdata);
  int plusplayer_capi_prepare_async(plusplayer_h handle);
  int plusplayer_capi_pause(plusplayer_h handle);
  int plusplayer_capi_resume(plusplayer_h handle);
  int plusplayer_capi_seek(plusplayer_h handle, uint64_t time);
  void plusplayer_capi_set_prebuffer_mode(plusplayer_h handle,
                                          bool prebuffer_mode);
  int plusplayer_capi_set_app_id(plusplayer_h handle, const char* app_id);
  int plusplayer_capi_suspend(plusplayer_h handle);
  int plusplayer_capi_restore(plusplayer_h handle,
                              plusplayer_state_e target_state);
  int plusplayer_capi_get_playing_time(plusplayer_h handle,
                                       uint64_t* cur_time_ms);
  int plusplayer_capi_set_display_mode(plusplayer_h handle,
                                       plusplayer_display_mode_e mode);
  int plusplayer_capi_set_display_roi(plusplayer_h handle,
                                      plusplayer_geometry_s roi);
  int plusplayer_capi_set_display_rotation(
      plusplayer_h handle, plusplayer_display_rotation_type_e rotation);
  int plusplayer_capi_set_buffer_config(plusplayer_h handle, const char* config,
                                        int amount);
  int plusplayer_capi_get_duration(plusplayer_h handle, int64_t* duration_ms);
  int plusplayer_capi_set_playback_rate(plusplayer_h handle,
                                        const double playback_rate);
  int plusplayer_capi_deactivate_audio(plusplayer_h handle);
  int plusplayer_capi_activate_audio(plusplayer_h handle);
  int plusplayer_capi_set_property(plusplayer_h handle,
                                   plusplayer_property_e property,
                                   const char* value);
  int plusplayer_capi_get_property(plusplayer_h handle,
                                   plusplayer_property_e property,
                                   char** value);
  int plusplayer_capi_get_track_count(plusplayer_h handle,
                                      plusplayer_track_type_e track_type,
                                      int* count);
  int plusplayer_capi_select_track(plusplayer_h handle,
                                   plusplayer_track_type_e type, int index);
  const char* plusplayer_capi_get_track_language_code(
      plusplayer_h handle, plusplayer_track_type_e type, int index);
  int plusplayer_capi_set_app_info(plusplayer_h handle,
                                   const plusplayer_app_info_s* app_info);
  int plusplayer_capi_set_drm(plusplayer_h handle,
                              plusplayer_drm_property_s drm_property);
  int plusplayer_capi_set_drm_init_data_cb(
      plusplayer_h handle, plusplayer_drm_init_data_cb drm_init_data_callback,
      void* userdata);
  int plusplayer_capi_set_adaptive_streaming_control_event_cb(
      plusplayer_h handle,
      plusplayer_adaptive_streaming_control_event_cb
          adaptive_streaming_control_event_cb,
      void* userdata);
  int plusplayer_capi_drm_license_acquired_done(
      plusplayer_h handle, plusplayer_track_type_e track_type);
  int plusplayer_capi_set_subtitle_path(plusplayer_h handle, const char* uri);
  int plusplayer_capi_set_video_stillmode(plusplayer_h handle,
                                          plusplayer_still_mode_e stillmode);
  int plusplayer_capi_set_alternative_video_resource(plusplayer_h handle,
                                                     unsigned int rsc_type);
  int plusplayer_capi_get_foreach_track(plusplayer_h handle,
                                        plusplayer_track_cb track_cb,
                                        void* userdata);
  int plusplayer_capi_get_foreach_active_track(plusplayer_h handle,
                                               plusplayer_track_cb track_cb,
                                               void* userdata);
  int plusplayer_capi_set_cookie(plusplayer_h handle, const char* cookie);
  int plusplayer_capi_set_user_agent(plusplayer_h handle,
                                     const char* user_agent);
  int plusplayer_capi_set_resume_time(plusplayer_h handle,
                                      uint64_t resume_time_ms);

 private:
  void* plusplayer_capi_handle_ = nullptr;
};

#endif