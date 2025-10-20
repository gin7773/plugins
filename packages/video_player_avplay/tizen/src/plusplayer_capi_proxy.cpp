// Copyright 2025 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "plusplayer_capi_proxy.h"

#include <dlfcn.h>

#include "log.h"

// Template function for dynamic library function calls
// template<typename FuncType, typename... Args>
// auto CallPlusplayerFunction(void* handle, const char* func_name, Args&&... args) 
//     -> decltype(reinterpret_cast<FuncType>(nullptr)(std::forward<Args>(args)...)) {
//   if (!handle) {
//     LOG_ERROR("plusplayer_capi_handle_ is invalid.");
//     if constexpr (std::is_pointer_v<decltype(reinterpret_cast<FuncType>(nullptr)(std::forward<Args>(args)...))>) {
//       return nullptr;
//     } else {
//       return 0;
//     }
//   }

//   FuncType func = reinterpret_cast<FuncType>(dlsym(handle, func_name));
//   if (!func) {
//     LOG_ERROR("Failed to find %s function.", func_name);
//     if constexpr (std::is_pointer_v<decltype(reinterpret_cast<FuncType>(nullptr)(std::forward<Args>(args)...))>) {
//       return nullptr;
//     } else {
//       return 0;
//     }
//   }

//   return func(std::forward<Args>(args)...);
// }

typedef plusplayer_h (*FunPlusplayerCapiCreate)(void);
typedef int (*FunPlusplayerCapiOpen)(plusplayer_h handle, const char* uri);
typedef int (*FunPlusplayerCapiPrepare)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiStart)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiStop)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiClose)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiDestroy)(plusplayer_h handle);
typedef plusplayer_state_e (*FunPlusplayerCapiGetState)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiSetDisplay)(plusplayer_h handle,
                                           plusplayer_display_type_e type, void* window);
typedef int (*FunPlusplayerCapiSetDisplaySubsurface)(plusplayer_h handle,
                                                    plusplayer_display_type_e type,
                                                    uint32_t surface_id,
                                                    plusplayer_geometry_s roi);
typedef int (*FunPlusplayerCapiSetPrepareAsyncDoneCb)(plusplayer_h handle,
                                                      plusplayer_prepare_async_done_cb prepare_async_done_cb, void* userdata);
typedef int (*FunPlusplayerCapiSetResourceConflictedCb)(plusplayer_h handle,
                                                        plusplayer_resource_conflicted_cb resource_conflicted_cb, void* userdata);
typedef int (*FunPlusplayerCapiSetEosCb)(plusplayer_h handle, plusplayer_eos_cb eos_cb,
                                         void* userdata);
typedef int (*FunPlusplayerCapiSetBufferStatusCb)(plusplayer_h handle, plusplayer_buffer_status_cb buffer_status_cb,
                                                  void* userdata);
typedef int (*FunPlusplayerCapiSetErrorCb)(plusplayer_h handle,
                                           plusplayer_error_cb error_cb,
                                           void* userdata);
typedef int (*FunPlusplayerCapiSetErrorMsgCb)(plusplayer_h handle,
                                              plusplayer_error_msg_cb error_msg_cb,
                                              void* userdata);
typedef int (*FunPlusplayerCapiSetSeekDoneCb)(plusplayer_h handle,
                                              plusplayer_seek_done_cb seek_done_cb,
                                              void* userdata);
typedef int (*FunPlusplayerCapiSetSubtitleUpdatedCb)(plusplayer_h handle,
                                                     plusplayer_subtitle_updated_cb subtitle_updated_cb,
                                                     void* userdata);
typedef int (*FunPlusplayerCapiSetAdEventCb)(plusplayer_h handle,
                                             plusplayer_ad_event_cb ad_event_cb,
                                             void* userdata);
typedef int (*FunPlusplayerCapiPrepareAsync)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiPause)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiResume)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiSeek)(plusplayer_h handle, uint64_t time);
typedef void (*FunPlusplayerCapiSetPrebufferMode)(plusplayer_h handle, bool prebuffer_mode);
typedef int (*FunPlusplayerCapiSetAppId)(plusplayer_h handle, const char* app_id);
typedef int (*FunPlusplayerCapiSuspend)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiRestore)(plusplayer_h handle,
                                        plusplayer_state_e target_state);
typedef int (*FunPlusplayerCapiGetPlayingTime)(plusplayer_h handle,
                                               uint64_t* cur_time_ms);
typedef int (*FunPlusplayerCapiSetDisplayMode)(plusplayer_h handle,
                                               plusplayer_display_mode_e mode);
typedef int (*FunPlusplayerCapiSetDisplayRoi)(plusplayer_h handle,
                                              plusplayer_geometry_s roi);
typedef int (*FunPlusplayerCapiSetDisplayRotation)(plusplayer_h handle,
                                                   plusplayer_display_rotation_type_e rotation);
typedef int (*FunPlusplayerCapiSetBufferConfig)(plusplayer_h handle, const char* config,
                                                int amount);
typedef int (*FunPlusplayerCapiGetDuration)(plusplayer_h handle, int64_t* duration_ms);
typedef int (*FunPlusplayerCapiSetPlaybackRate)(plusplayer_h handle,
                                                const double playback_rate);
typedef int (*FunPlusplayerCapiDeactivateAudio)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiActivateAudio)(plusplayer_h handle);
typedef int (*FunPlusplayerCapiSetProperty)(plusplayer_h handle,
                                            plusplayer_property_e property,
                                            const char* value);
typedef int (*FunPlusplayerCapiGetProperty)(plusplayer_h handle,
                                            plusplayer_property_e property,
                                            char** value);
typedef int (*FunPlusplayerCapiGetTrackCount)(plusplayer_h handle,
                                              plusplayer_track_type_e track_type,
                                              int* count);
typedef int (*FunPlusplayerCapiSelectTrack)(plusplayer_h handle,
                                            plusplayer_track_type_e type, int index);
typedef const char* (*FunPlusplayerCapiGetTrackLanguageCode)(plusplayer_h handle, plusplayer_track_type_e type, int index);
typedef int (*FunPlusplayerCapiSetAppInfo)(plusplayer_h handle,
                                           const plusplayer_app_info_s* app_info);
typedef int (*FunPlusplayerCapiSetDrm)(plusplayer_h handle,
                                       plusplayer_drm_property_s drm_property);
typedef int (*FunPlusplayerCapiSetDrmInitDataCb)(plusplayer_h handle, plusplayer_drm_init_data_cb drm_init_data_callback,
                                                 void* userdata);
typedef int (*FunPlusplayerCapiSetAdaptiveStreamingControlEventCb)(plusplayer_h handle,
                                                                   plusplayer_adaptive_streaming_control_event_cb adaptive_streaming_control_event_cb,
                                                                   void* userdata);
typedef int (*FunPlusplayerCapiDrmLicenseAcquiredDone)(plusplayer_h handle, plusplayer_track_type_e track_type);
typedef int (*FunPlusplayerCapiSetSubtitlePath)(plusplayer_h handle, const char* uri);
typedef int (*FunPlusplayerCapiSetVideoStillmode)(plusplayer_h handle,
                                                  plusplayer_still_mode_e stillmode);
typedef int (*FunPlusplayerCapiSetAlternativeVideoResource)(plusplayer_h handle,
                                                           unsigned int rsc_type);
typedef int (*FunPlusplayerCapiGetForeachTrack)(plusplayer_h handle,
                                                plusplayer_track_cb track_cb,
                                                void* userdata);
typedef int (*FunPlusplayerCapiGetForeachActiveTrack)(plusplayer_h handle,
                                                     plusplayer_track_cb track_cb,
                                                     void* userdata);
typedef int (*FunPlusplayerCapiSetCookie)(plusplayer_h handle, const char* cookie);
typedef int (*FunPlusplayerCapiSetUserAgent)(plusplayer_h handle,
                                             const char* user_agent);
typedef int (*FunPlusplayerCapiSetResumeTime)(plusplayer_h handle,
                                              uint64_t resume_time_ms);

PlusplayerCapiProxy::PlusplayerCapiProxy() {
  plusplayer_capi_handle_ = dlopen("xxxx.so", RTLD_LAZY);
  if (plusplayer_capi_handle_ == nullptr) {
    LOG_ERROR("Failed to open xxx.so");
  }
}

PlusplayerCapiProxy::~PlusplayerCapiProxy() {
  if (plusplayer_capi_handle_) {
    dlclose(plusplayer_capi_handle_);
    plusplayer_capi_handle_ = nullptr;
  }
}

plusplayer_h PlusplayerCapiProxy::plusplayer_capi_create(void) {
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

int PlusplayerCapiProxy::plusplayer_capi_open(plusplayer_h handle,
                                              const char* uri) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiOpen plusplayer_capi_open =
      reinterpret_cast<FunPlusplayerCapiOpen>(
          dlsym(plusplayer_capi_handle_, "plusplayer_open"));

  if (!plusplayer_capi_open) {
    LOG_ERROR("Failed to find plusplayer_open function.");
    return 0;
  }

  return plusplayer_capi_open(handle, uri);
}

plusplayer_state_e PlusplayerCapiProxy::plusplayer_capi_get_state(
    plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetState plusplayer_capi_get_state =
      reinterpret_cast<FunPlusplayerCapiGetState>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_state"));

  if (!plusplayer_capi_get_state) {
    LOG_ERROR("Failed to find plusplayer_get_state function.");
    return 0;
  }

  return plusplayer_capi_get_state(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_set_property(
    plusplayer_h handle, plusplayer_property_e property, const char* value) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetProperty plusplayer_capi_set_property =
      reinterpret_cast<FunPlusplayerCapiSetProperty>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_property"));

  if (!plusplayer_capi_set_property) {
    LOG_ERROR("Failed to find plusplayer_set_property function.");
    return 0;
  }

  return plusplayer_capi_set_property(handle, property, value);
}

int PlusplayerCapiProxy::plusplayer_capi_set_app_id(plusplayer_h handle, const char* app_id) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetAppId plusplayer_capi_set_app_id = 
    reinterpret_cast<FunPlusplayerCapiSetAppId>(
      dlsym(plusplayer_capi_handle_, "plusplayer_set_app_id")
    );
  
  if (!plusplayer_capi_set_app_id) {
    LOG_ERROR("Failed to find plusplayer_set_app_id function.");
    return 0;
  }

  return plusplayer_capi_set_app_id(handle, app_id);
}

void PlusplayerCapiProxy::plusplayer_capi_set_prebuffer_mode(plusplayer_h handle, bool prebuffer_mode) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("power_state_handle_ is invalid.");
    return;
  }

  FunPlusplayerCapiSetPrebufferMode plusplayer_capi_set_prebuffer_mode = 
    reinterpret_cast<FunPlusplayerCapiSetPrebufferMode>(
      dlsym(plusplayer_capi_handle_, "plusplayer_set_prebuffer_mode")
    );

  if (!plusplayer_capi_set_prebuffer_mode) {
    LOG_ERROR("Failed to find plusplayer_set_prebuffer_mode function.");
    return;
  }

  return plusplayer_capi_set_prebuffer_mode(handle, prebuffer_mode);
}

int PlusplayerCapiProxy::plusplayer_capi_prepare(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiPrepare plusplayer_capi_prepare =
      reinterpret_cast<FunPlusplayerCapiPrepare>(
          dlsym(plusplayer_capi_handle_, "plusplayer_prepare"));

  if (!plusplayer_capi_prepare) {
    LOG_ERROR("Failed to find plusplayer_prepare function.");
    return 0;
  }

  return plusplayer_capi_prepare(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_start(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiStart plusplayer_capi_start =
      reinterpret_cast<FunPlusplayerCapiStart>(
          dlsym(plusplayer_capi_handle_, "plusplayer_start"));

  if (!plusplayer_capi_start) {
    LOG_ERROR("Failed to find plusplayer_start function.");
    return 0;
  }

  return plusplayer_capi_start(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_stop(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiStop plusplayer_capi_stop =
      reinterpret_cast<FunPlusplayerCapiStop>(
          dlsym(plusplayer_capi_handle_, "plusplayer_stop"));

  if (!plusplayer_capi_stop) {
    LOG_ERROR("Failed to find plusplayer_stop function.");
    return 0;
  }

  return plusplayer_capi_stop(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_close(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiClose plusplayer_capi_close =
      reinterpret_cast<FunPlusplayerCapiClose>(
          dlsym(plusplayer_capi_handle_, "plusplayer_close"));

  if (!plusplayer_capi_close) {
    LOG_ERROR("Failed to find plusplayer_close function.");
    return 0;
  }

  return plusplayer_capi_close(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_destroy(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiDestroy plusplayer_capi_destroy =
      reinterpret_cast<FunPlusplayerCapiDestroy>(
          dlsym(plusplayer_capi_handle_, "plusplayer_destroy"));

  if (!plusplayer_capi_destroy) {
    LOG_ERROR("Failed to find plusplayer_destroy function.");
    return 0;
  }

  return plusplayer_capi_destroy(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_set_display(plusplayer_h handle,
                                                     plusplayer_display_type_e type, void* window) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDisplay plusplayer_capi_set_display =
      reinterpret_cast<FunPlusplayerCapiSetDisplay>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_display"));

  if (!plusplayer_capi_set_display) {
    LOG_ERROR("Failed to find plusplayer_set_display function.");
    return 0;
  }

  return plusplayer_capi_set_display(handle, type, window);
}

int PlusplayerCapiProxy::plusplayer_capi_set_display_subsurface(plusplayer_h handle,
                                                                plusplayer_display_type_e type,
                                                                uint32_t surface_id,
                                                                plusplayer_geometry_s roi) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDisplaySubsurface plusplayer_capi_set_display_subsurface =
      reinterpret_cast<FunPlusplayerCapiSetDisplaySubsurface>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_display_subsurface"));

  if (!plusplayer_capi_set_display_subsurface) {
    LOG_ERROR("Failed to find plusplayer_set_display_subsurface function.");
    return 0;
  }

  return plusplayer_capi_set_display_subsurface(handle, type, surface_id, roi);
}

int PlusplayerCapiProxy::plusplayer_capi_set_prepare_async_done_cb(
    plusplayer_h handle,
    plusplayer_prepare_async_done_cb prepare_async_done_cb, void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetPrepareAsyncDoneCb plusplayer_capi_set_prepare_async_done_cb =
      reinterpret_cast<FunPlusplayerCapiSetPrepareAsyncDoneCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_prepare_async_done_cb"));

  if (!plusplayer_capi_set_prepare_async_done_cb) {
    LOG_ERROR("Failed to find plusplayer_set_prepare_async_done_cb function.");
    return 0;
  }

  return plusplayer_capi_set_prepare_async_done_cb(handle, prepare_async_done_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_resource_conflicted_cb(
    plusplayer_h handle,
    plusplayer_resource_conflicted_cb resource_conflicted_cb, void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetResourceConflictedCb plusplayer_capi_set_resource_conflicted_cb =
      reinterpret_cast<FunPlusplayerCapiSetResourceConflictedCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_resource_conflicted_cb"));

  if (!plusplayer_capi_set_resource_conflicted_cb) {
    LOG_ERROR("Failed to find plusplayer_set_resource_conflicted_cb function.");
    return 0;
  }

  return plusplayer_capi_set_resource_conflicted_cb(handle, resource_conflicted_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_eos_cb(plusplayer_h handle, plusplayer_eos_cb eos_cb,
                                                     void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetEosCb plusplayer_capi_set_eos_cb =
      reinterpret_cast<FunPlusplayerCapiSetEosCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_eos_cb"));

  if (!plusplayer_capi_set_eos_cb) {
    LOG_ERROR("Failed to find plusplayer_set_eos_cb function.");
    return 0;
  }

  return plusplayer_capi_set_eos_cb(handle, eos_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_buffer_status_cb(
    plusplayer_h handle, plusplayer_buffer_status_cb buffer_status_cb,
    void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetBufferStatusCb plusplayer_capi_set_buffer_status_cb =
      reinterpret_cast<FunPlusplayerCapiSetBufferStatusCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_buffer_status_cb"));

  if (!plusplayer_capi_set_buffer_status_cb) {
    LOG_ERROR("Failed to find plusplayer_set_buffer_status_cb function.");
    return 0;
  }

  return plusplayer_capi_set_buffer_status_cb(handle, buffer_status_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_error_cb(plusplayer_h handle,
                                                       plusplayer_error_cb error_cb,
                                                       void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetErrorCb plusplayer_capi_set_error_cb =
      reinterpret_cast<FunPlusplayerCapiSetErrorCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_error_cb"));

  if (!plusplayer_capi_set_error_cb) {
    LOG_ERROR("Failed to find plusplayer_set_error_cb function.");
    return 0;
  }

  return plusplayer_capi_set_error_cb(handle, error_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_error_msg_cb(plusplayer_h handle,
                                                           plusplayer_error_msg_cb error_msg_cb,
                                                           void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetErrorMsgCb plusplayer_capi_set_error_msg_cb =
      reinterpret_cast<FunPlusplayerCapiSetErrorMsgCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_error_msg_cb"));

  if (!plusplayer_capi_set_error_msg_cb) {
    LOG_ERROR("Failed to find plusplayer_set_error_msg_cb function.");
    return 0;
  }

  return plusplayer_capi_set_error_msg_cb(handle, error_msg_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_seek_done_cb(plusplayer_h handle,
                                                           plusplayer_seek_done_cb seek_done_cb,
                                                           void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetSeekDoneCb plusplayer_capi_set_seek_done_cb =
      reinterpret_cast<FunPlusplayerCapiSetSeekDoneCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_seek_done_cb"));

  if (!plusplayer_capi_set_seek_done_cb) {
    LOG_ERROR("Failed to find plusplayer_set_seek_done_cb function.");
    return 0;
  }

  return plusplayer_capi_set_seek_done_cb(handle, seek_done_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_subtitle_updated_cb(
    plusplayer_h handle, plusplayer_subtitle_updated_cb subtitle_updated_cb,
    void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetSubtitleUpdatedCb plusplayer_capi_set_subtitle_updated_cb =
      reinterpret_cast<FunPlusplayerCapiSetSubtitleUpdatedCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_subtitle_updated_cb"));

  if (!plusplayer_capi_set_subtitle_updated_cb) {
    LOG_ERROR("Failed to find plusplayer_set_subtitle_updated_cb function.");
    return 0;
  }

  return plusplayer_capi_set_subtitle_updated_cb(handle, subtitle_updated_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_ad_event_cb(plusplayer_h handle,
                                                          plusplayer_ad_event_cb ad_event_cb,
                                                          void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetAdEventCb plusplayer_capi_set_ad_event_cb =
      reinterpret_cast<FunPlusplayerCapiSetAdEventCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_ad_event_cb"));

  if (!plusplayer_capi_set_ad_event_cb) {
    LOG_ERROR("Failed to find plusplayer_set_ad_event_cb function.");
    return 0;
  }

  return plusplayer_capi_set_ad_event_cb(handle, ad_event_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_prepare_async(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiPrepareAsync plusplayer_capi_prepare_async =
      reinterpret_cast<FunPlusplayerCapiPrepareAsync>(
          dlsym(plusplayer_capi_handle_, "plusplayer_prepare_async"));

  if (!plusplayer_capi_prepare_async) {
    LOG_ERROR("Failed to find plusplayer_prepare_async function.");
    return 0;
  }

  return plusplayer_capi_prepare_async(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_pause(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiPause plusplayer_capi_pause =
      reinterpret_cast<FunPlusplayerCapiPause>(
          dlsym(plusplayer_capi_handle_, "plusplayer_pause"));

  if (!plusplayer_capi_pause) {
    LOG_ERROR("Failed to find plusplayer_pause function.");
    return 0;
  }

  return plusplayer_capi_pause(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_resume(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiResume plusplayer_capi_resume =
      reinterpret_cast<FunPlusplayerCapiResume>(
          dlsym(plusplayer_capi_handle_, "plusplayer_resume"));

  if (!plusplayer_capi_resume) {
    LOG_ERROR("Failed to find plusplayer_resume function.");
    return 0;
  }

  return plusplayer_capi_resume(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_seek(plusplayer_h handle, uint64_t time) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSeek plusplayer_capi_seek =
      reinterpret_cast<FunPlusplayerCapiSeek>(
          dlsym(plusplayer_capi_handle_, "plusplayer_seek"));

  if (!plusplayer_capi_seek) {
    LOG_ERROR("Failed to find plusplayer_seek function.");
    return 0;
  }

  return plusplayer_capi_seek(handle, time);
}

int PlusplayerCapiProxy::plusplayer_capi_suspend(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSuspend plusplayer_capi_suspend =
      reinterpret_cast<FunPlusplayerCapiSuspend>(
          dlsym(plusplayer_capi_handle_, "plusplayer_suspend"));

  if (!plusplayer_capi_suspend) {
    LOG_ERROR("Failed to find plusplayer_suspend function.");
    return 0;
  }

  return plusplayer_capi_suspend(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_restore(plusplayer_h handle,
                                                  plusplayer_state_e target_state) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiRestore plusplayer_capi_restore =
      reinterpret_cast<FunPlusplayerCapiRestore>(
          dlsym(plusplayer_capi_handle_, "plusplayer_restore"));

  if (!plusplayer_capi_restore) {
    LOG_ERROR("Failed to find plusplayer_restore function.");
    return 0;
  }

  return plusplayer_capi_restore(handle, target_state);
}

int PlusplayerCapiProxy::plusplayer_capi_get_playing_time(plusplayer_h handle,
                                                          uint64_t* cur_time_ms) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetPlayingTime plusplayer_capi_get_playing_time =
      reinterpret_cast<FunPlusplayerCapiGetPlayingTime>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_playing_time"));

  if (!plusplayer_capi_get_playing_time) {
    LOG_ERROR("Failed to find plusplayer_get_playing_time function.");
    return 0;
  }

  return plusplayer_capi_get_playing_time(handle, cur_time_ms);
}

int PlusplayerCapiProxy::plusplayer_capi_set_display_mode(plusplayer_h handle,
                                                          plusplayer_display_mode_e mode) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDisplayMode plusplayer_capi_set_display_mode =
      reinterpret_cast<FunPlusplayerCapiSetDisplayMode>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_display_mode"));

  if (!plusplayer_capi_set_display_mode) {
    LOG_ERROR("Failed to find plusplayer_set_display_mode function.");
    return 0;
  }

  return plusplayer_capi_set_display_mode(handle, mode);
}

int PlusplayerCapiProxy::plusplayer_capi_set_display_roi(plusplayer_h handle,
                                                         plusplayer_geometry_s roi) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDisplayRoi plusplayer_capi_set_display_roi =
      reinterpret_cast<FunPlusplayerCapiSetDisplayRoi>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_display_roi"));

  if (!plusplayer_capi_set_display_roi) {
    LOG_ERROR("Failed to find plusplayer_set_display_roi function.");
    return 0;
  }

  return plusplayer_capi_set_display_roi(handle, roi);
}

int PlusplayerCapiProxy::plusplayer_capi_set_display_rotation(
    plusplayer_h handle, plusplayer_display_rotation_type_e rotation) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDisplayRotation plusplayer_capi_set_display_rotation =
      reinterpret_cast<FunPlusplayerCapiSetDisplayRotation>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_display_rotation"));

  if (!plusplayer_capi_set_display_rotation) {
    LOG_ERROR("Failed to find plusplayer_set_display_rotation function.");
    return 0;
  }

  return plusplayer_capi_set_display_rotation(handle, rotation);
}

int PlusplayerCapiProxy::plusplayer_capi_set_buffer_config(plusplayer_h handle, const char* config,
                                                           int amount) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetBufferConfig plusplayer_capi_set_buffer_config =
      reinterpret_cast<FunPlusplayerCapiSetBufferConfig>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_buffer_config"));

  if (!plusplayer_capi_set_buffer_config) {
    LOG_ERROR("Failed to find plusplayer_set_buffer_config function.");
    return 0;
  }

  return plusplayer_capi_set_buffer_config(handle, config, amount);
}

int PlusplayerCapiProxy::plusplayer_capi_get_duration(plusplayer_h handle, int64_t* duration_ms) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetDuration plusplayer_capi_get_duration =
      reinterpret_cast<FunPlusplayerCapiGetDuration>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_duration"));

  if (!plusplayer_capi_get_duration) {
    LOG_ERROR("Failed to find plusplayer_get_duration function.");
    return 0;
  }

  return plusplayer_capi_get_duration(handle, duration_ms);
}

int PlusplayerCapiProxy::plusplayer_capi_set_playback_rate(plusplayer_h handle,
                                                           const double playback_rate) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetPlaybackRate plusplayer_capi_set_playback_rate =
      reinterpret_cast<FunPlusplayerCapiSetPlaybackRate>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_playback_rate"));

  if (!plusplayer_capi_set_playback_rate) {
    LOG_ERROR("Failed to find plusplayer_set_playback_rate function.");
    return 0;
  }

  return plusplayer_capi_set_playback_rate(handle, playback_rate);
}

int PlusplayerCapiProxy::plusplayer_capi_deactivate_audio(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiDeactivateAudio plusplayer_capi_deactivate_audio =
      reinterpret_cast<FunPlusplayerCapiDeactivateAudio>(
          dlsym(plusplayer_capi_handle_, "plusplayer_deactivate_audio"));

  if (!plusplayer_capi_deactivate_audio) {
    LOG_ERROR("Failed to find plusplayer_deactivate_audio function.");
    return 0;
  }

  return plusplayer_capi_deactivate_audio(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_activate_audio(plusplayer_h handle) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiActivateAudio plusplayer_capi_activate_audio =
      reinterpret_cast<FunPlusplayerCapiActivateAudio>(
          dlsym(plusplayer_capi_handle_, "plusplayer_activate_audio"));

  if (!plusplayer_capi_activate_audio) {
    LOG_ERROR("Failed to find plusplayer_activate_audio function.");
    return 0;
  }

  return plusplayer_capi_activate_audio(handle);
}

int PlusplayerCapiProxy::plusplayer_capi_get_property(plusplayer_h handle,
                                                      plusplayer_property_e property,
                                                      char** value) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetProperty plusplayer_capi_get_property =
      reinterpret_cast<FunPlusplayerCapiGetProperty>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_property"));

  if (!plusplayer_capi_get_property) {
    LOG_ERROR("Failed to find plusplayer_get_property function.");
    return 0;
  }

  return plusplayer_capi_get_property(handle, property, value);
}

int PlusplayerCapiProxy::plusplayer_capi_get_track_count(plusplayer_h handle,
                                                         plusplayer_track_type_e track_type,
                                                         int* count) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetTrackCount plusplayer_capi_get_track_count =
      reinterpret_cast<FunPlusplayerCapiGetTrackCount>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_track_count"));

  if (!plusplayer_capi_get_track_count) {
    LOG_ERROR("Failed to find plusplayer_get_track_count function.");
    return 0;
  }

  return plusplayer_capi_get_track_count(handle, track_type, count);
}

int PlusplayerCapiProxy::plusplayer_capi_select_track(plusplayer_h handle,
                                                      plusplayer_track_type_e type, int index) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSelectTrack plusplayer_capi_select_track =
      reinterpret_cast<FunPlusplayerCapiSelectTrack>(
          dlsym(plusplayer_capi_handle_, "plusplayer_select_track"));

  if (!plusplayer_capi_select_track) {
    LOG_ERROR("Failed to find plusplayer_select_track function.");
    return 0;
  }

  return plusplayer_capi_select_track(handle, type, index);
}

const char* PlusplayerCapiProxy::plusplayer_capi_get_track_language_code(
    plusplayer_h handle, plusplayer_track_type_e type, int index) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return nullptr;
  }

  FunPlusplayerCapiGetTrackLanguageCode plusplayer_capi_get_track_language_code =
      reinterpret_cast<FunPlusplayerCapiGetTrackLanguageCode>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_track_language_code"));

  if (!plusplayer_capi_get_track_language_code) {
    LOG_ERROR("Failed to find plusplayer_get_track_language_code function.");
    return nullptr;
  }

  return plusplayer_capi_get_track_language_code(handle, type, index);
}

int PlusplayerCapiProxy::plusplayer_capi_set_app_info(plusplayer_h handle,
                                                       const plusplayer_app_info_s* app_info) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetAppInfo plusplayer_capi_set_app_info =
      reinterpret_cast<FunPlusplayerCapiSetAppInfo>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_app_info"));

  if (!plusplayer_capi_set_app_info) {
    LOG_ERROR("Failed to find plusplayer_set_app_info function.");
    return 0;
  }

  return plusplayer_capi_set_app_info(handle, app_info);
}

int PlusplayerCapiProxy::plusplayer_capi_set_drm(plusplayer_h handle,
                                                  plusplayer_drm_property_s drm_property) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDrm plusplayer_capi_set_drm =
      reinterpret_cast<FunPlusplayerCapiSetDrm>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_drm"));

  if (!plusplayer_capi_set_drm) {
    LOG_ERROR("Failed to find plusplayer_set_drm function.");
    return 0;
  }

  return plusplayer_capi_set_drm(handle, drm_property);
}

int PlusplayerCapiProxy::plusplayer_capi_set_drm_init_data_cb(
    plusplayer_h handle, plusplayer_drm_init_data_cb drm_init_data_callback,
    void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetDrmInitDataCb plusplayer_capi_set_drm_init_data_cb =
      reinterpret_cast<FunPlusplayerCapiSetDrmInitDataCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_drm_init_data_cb"));

  if (!plusplayer_capi_set_drm_init_data_cb) {
    LOG_ERROR("Failed to find plusplayer_set_drm_init_data_cb function.");
    return 0;
  }

  return plusplayer_capi_set_drm_init_data_cb(handle, drm_init_data_callback, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_adaptive_streaming_control_event_cb(
    plusplayer_h handle,
    plusplayer_adaptive_streaming_control_event_cb adaptive_streaming_control_event_cb,
    void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetAdaptiveStreamingControlEventCb plusplayer_capi_set_adaptive_streaming_control_event_cb =
      reinterpret_cast<FunPlusplayerCapiSetAdaptiveStreamingControlEventCb>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_adaptive_streaming_control_event_cb"));

  if (!plusplayer_capi_set_adaptive_streaming_control_event_cb) {
    LOG_ERROR("Failed to find plusplayer_set_adaptive_streaming_control_event_cb function.");
    return 0;
  }

  return plusplayer_capi_set_adaptive_streaming_control_event_cb(handle, adaptive_streaming_control_event_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_drm_license_acquired_done(
    plusplayer_h handle, plusplayer_track_type_e track_type) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiDrmLicenseAcquiredDone plusplayer_capi_drm_license_acquired_done =
      reinterpret_cast<FunPlusplayerCapiDrmLicenseAcquiredDone>(
          dlsym(plusplayer_capi_handle_, "plusplayer_drm_license_acquired_done"));

  if (!plusplayer_capi_drm_license_acquired_done) {
    LOG_ERROR("Failed to find plusplayer_drm_license_acquired_done function.");
    return 0;
  }

  return plusplayer_capi_drm_license_acquired_done(handle, track_type);
}

int PlusplayerCapiProxy::plusplayer_capi_set_subtitle_path(plusplayer_h handle, const char* uri) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetSubtitlePath plusplayer_capi_set_subtitle_path =
      reinterpret_cast<FunPlusplayerCapiSetSubtitlePath>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_subtitle_path"));

  if (!plusplayer_capi_set_subtitle_path) {
    LOG_ERROR("Failed to find plusplayer_set_subtitle_path function.");
    return 0;
  }

  return plusplayer_capi_set_subtitle_path(handle, uri);
}

int PlusplayerCapiProxy::plusplayer_capi_set_video_stillmode(plusplayer_h handle,
                                                             plusplayer_still_mode_e stillmode) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetVideoStillmode plusplayer_capi_set_video_stillmode =
      reinterpret_cast<FunPlusplayerCapiSetVideoStillmode>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_video_stillmode"));

  if (!plusplayer_capi_set_video_stillmode) {
    LOG_ERROR("Failed to find plusplayer_set_video_stillmode function.");
    return 0;
  }

  return plusplayer_capi_set_video_stillmode(handle, stillmode);
}

int PlusplayerCapiProxy::plusplayer_capi_set_alternative_video_resource(plusplayer_h handle,
                                                                        unsigned int rsc_type) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetAlternativeVideoResource plusplayer_capi_set_alternative_video_resource =
      reinterpret_cast<FunPlusplayerCapiSetAlternativeVideoResource>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_alternative_video_resource"));

  if (!plusplayer_capi_set_alternative_video_resource) {
    LOG_ERROR("Failed to find plusplayer_set_alternative_video_resource function.");
    return 0;
  }

  return plusplayer_capi_set_alternative_video_resource(handle, rsc_type);
}

int PlusplayerCapiProxy::plusplayer_capi_get_foreach_track(plusplayer_h handle,
                                                           plusplayer_track_cb track_cb,
                                                           void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetForeachTrack plusplayer_capi_get_foreach_track =
      reinterpret_cast<FunPlusplayerCapiGetForeachTrack>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_foreach_track"));

  if (!plusplayer_capi_get_foreach_track) {
    LOG_ERROR("Failed to find plusplayer_get_foreach_track function.");
    return 0;
  }

  return plusplayer_capi_get_foreach_track(handle, track_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_get_foreach_active_track(plusplayer_h handle,
                                                                  plusplayer_track_cb track_cb,
                                                                  void* userdata) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiGetForeachActiveTrack plusplayer_capi_get_foreach_active_track =
      reinterpret_cast<FunPlusplayerCapiGetForeachActiveTrack>(
          dlsym(plusplayer_capi_handle_, "plusplayer_get_foreach_active_track"));

  if (!plusplayer_capi_get_foreach_active_track) {
    LOG_ERROR("Failed to find plusplayer_get_foreach_active_track function.");
    return 0;
  }

  return plusplayer_capi_get_foreach_active_track(handle, track_cb, userdata);
}

int PlusplayerCapiProxy::plusplayer_capi_set_cookie(plusplayer_h handle, const char* cookie) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetCookie plusplayer_capi_set_cookie =
      reinterpret_cast<FunPlusplayerCapiSetCookie>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_cookie"));

  if (!plusplayer_capi_set_cookie) {
    LOG_ERROR("Failed to find plusplayer_set_cookie function.");
    return 0;
  }

  return plusplayer_capi_set_cookie(handle, cookie);
}

int PlusplayerCapiProxy::plusplayer_capi_set_user_agent(plusplayer_h handle,
                                                        const char* user_agent) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetUserAgent plusplayer_capi_set_user_agent =
      reinterpret_cast<FunPlusplayerCapiSetUserAgent>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_user_agent"));

  if (!plusplayer_capi_set_user_agent) {
    LOG_ERROR("Failed to find plusplayer_set_user_agent function.");
    return 0;
  }

  return plusplayer_capi_set_user_agent(handle, user_agent);
}

int PlusplayerCapiProxy::plusplayer_capi_set_resume_time(plusplayer_h handle,
                                                         uint64_t resume_time_ms) {
  if (!plusplayer_capi_handle_) {
    LOG_ERROR("plusplayer_capi_handle_ is invalid.");
    return 0;
  }

  FunPlusplayerCapiSetResumeTime plusplayer_capi_set_resume_time =
      reinterpret_cast<FunPlusplayerCapiSetResumeTime>(
          dlsym(plusplayer_capi_handle_, "plusplayer_set_resume_time"));

  if (!plusplayer_capi_set_resume_time) {
    LOG_ERROR("Failed to find plusplayer_set_resume_time function.");
    return 0;
  }

  return plusplayer_capi_set_resume_time(handle, resume_time_ms);
}
