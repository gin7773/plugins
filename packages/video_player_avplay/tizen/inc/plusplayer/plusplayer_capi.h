// Copyright 2022 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_PLUGIN_PLUSPLAYER_CAPI_H
#define FLUTTER_PLUGIN_PLUSPLAYER_CAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <tizen_error.h>

#define PLUSPLAYER_ERROR_CLASS TIZEN_ERROR_PLAYER | 0x20

/* This is for custom defined plusplayer error. */
#define PLUSPLAYER_CUSTOM_ERROR_CLASS TIZEN_ERROR_PLAYER | 0x1000

/**
 * @brief  Enumerations for the error type
 */
typedef enum plusplayer_error_type {
  PLUSPLAYER_ERROR_TYPE_NONE = TIZEN_ERROR_NONE, /**< Successful */
  PLUSPLAYER_ERROR_TYPE_OUT_OF_MEMORY =
      TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
  PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER =
      TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
  PLUSPLAYER_ERROR_TYPE_NO_SUCH_FILE =
      TIZEN_ERROR_NO_SUCH_FILE, /**< No such file or directory */
  PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION =
      TIZEN_ERROR_INVALID_OPERATION, /**< Invalid operation */
  PLUSPLAYER_ERROR_TYPE_FILE_NO_SPACE_ON_DEVICE =
      TIZEN_ERROR_FILE_NO_SPACE_ON_DEVICE, /**< No space left on the device */
  PLUSPLAYER_ERROR_TYPE_FEATURE_NOT_SUPPORTED_ON_DEVICE =
      TIZEN_ERROR_NOT_SUPPORTED, /**< Not supported */
  PLUSPLAYER_ERROR_TYPE_SEEK_FAILED =
      PLUSPLAYER_ERROR_CLASS | 0x01, /**< Seek operation failure */
  PLUSPLAYER_ERROR_TYPE_INVALID_STATE =
      PLUSPLAYER_ERROR_CLASS | 0x02, /**< Invalid state */
  PLUSPLAYER_ERROR_TYPE_NOT_SUPPORTED_FILE =
      PLUSPLAYER_ERROR_CLASS | 0x03, /**< File format not supported */
  PLUSPLAYER_ERROR_TYPE_INVALID_URI =
      PLUSPLAYER_ERROR_CLASS | 0x04, /**< Invalid URI */
  PLUSPLAYER_ERROR_TYPE_SOUND_POLICY =
      PLUSPLAYER_ERROR_CLASS | 0x05, /**< Sound policy error */
  PLUSPLAYER_ERROR_TYPE_CONNECTION_FAILED =
      PLUSPLAYER_ERROR_CLASS | 0x06, /**< Streaming connection failed */
  PLUSPLAYER_ERROR_TYPE_VIDEO_CAPTURE_FAILED =
      PLUSPLAYER_ERROR_CLASS | 0x07, /**< Video capture failed */
  PLUSPLAYER_ERROR_TYPE_DRM_EXPIRED =
      PLUSPLAYER_ERROR_CLASS | 0x08, /**< Expired license */
  PLUSPLAYER_ERROR_TYPE_DRM_NO_LICENSE =
      PLUSPLAYER_ERROR_CLASS | 0x09, /**< No license */
  PLUSPLAYER_ERROR_TYPE_DRM_FUTURE_USE =
      PLUSPLAYER_ERROR_CLASS | 0x0a, /**< License for future use */
  PLUSPLAYER_ERROR_TYPE_NOT_PERMITTED =
      PLUSPLAYER_ERROR_CLASS | 0x0b, /**< Format not permitted */
  PLUSPLAYER_ERROR_TYPE_RESOURCE_LIMIT =
      PLUSPLAYER_ERROR_CLASS | 0x0c, /**< Resource limit */
  PLUSPLAYER_ERROR_TYPE_PERMISSION_DENIED =
      TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
  PLUSPLAYER_ERROR_TYPE_SERVICE_DISCONNECTED =
      PLUSPLAYER_ERROR_CLASS | 0x0d, /**< Socket connection lost (Since 3.0) */
  PLUSPLAYER_ERROR_TYPE_NO_BUFFER_SPACE =
      TIZEN_ERROR_BUFFER_SPACE, /**< No buffer space available (Since 3.0) */
  PLUSPLAYER_ERROR_TYPE_NOT_SUPPORTED_AUDIO_CODEC =
      PLUSPLAYER_ERROR_CLASS | 0x0e, /**< Not supported audio codec but video
                                        can be played (Since 4.0) */
  PLUSPLAYER_ERROR_TYPE_NOT_SUPPORTED_VIDEO_CODEC =
      PLUSPLAYER_ERROR_CLASS | 0x0f, /**< Not supported video codec but video
                                        can be played (Since 4.0) */
  PLUSPLAYER_ERROR_TYPE_NOT_SUPPORTED_SUBTITLE =
      PLUSPLAYER_ERROR_CLASS |
      0x10, /**< Not supported subtitle format (Since 4.0) */
  PLUSPLAYER_ERROR_TYPE_DRM_DECRYPTION_FAILED =
      PLUSPLAYER_CUSTOM_ERROR_CLASS | 0x05, /**< playready drm error info */
  PLUSPLAYER_ERROR_TYPE_NOT_SUPPORTED_FORMAT =
      PLUSPLAYER_CUSTOM_ERROR_CLASS | 0x08, /**< format not supported */
  PLUSPLAYER_ERROR_TYPE_STREAMING_PLAYER =
      PLUSPLAYER_CUSTOM_ERROR_CLASS | 0x09,  // confirm it later
  PLUSPLAYER_ERROR_TYPE_DTCPFSK = PLUSPLAYER_CUSTOM_ERROR_CLASS | 0x0a,
  PLUSPLAYER_ERROR_TYPE_PRELOADING_TIMEOUT =
      PLUSPLAYER_CUSTOM_ERROR_CLASS |
      0x0b, /**< can't finish preloading in time*/
  PLUSPLAYER_ERROR_TYPE_NETWORK_ERROR =
      PLUSPLAYER_CUSTOM_ERROR_CLASS | 0x0c, /**< for network error*/
  PLUSPLAYER_ERROR_TYPE_NOT_CHANNEL_SURFING_ERROR =
      PLUSPLAYER_CUSTOM_ERROR_CLASS | 0x0d, /**< for channel surfing error*/
  PLUSPLAYER_ERROR_TYPE_UNKNOWN
} plusplayer_error_type;

/**
 * @brief  Enumerations for plusplayer state.
 */
typedef enum plusplayer_state {
  PLUSPLAYER_STATE_NONE, /**<Player is created, but not opened*/
  PLUSPLAYER_STATE_IDLE, /**<Player is opened, but not prepared or player is
                            stopped*/
  PLUSPLAYER_STATE_TYPE_FINDER_READY,  /**<TypeFinder prepared*/
  PLUSPLAYER_STATE_TRACK_SOURCE_READY, /**<TrackSource prepared*/
  PLUSPLAYER_STATE_READY,              /**<Player is ready to play(start)*/
  PLUSPLAYER_STATE_PLAYING,            /**<Player is playing media*/
  PLUSPLAYER_STATE_PAUSED              /**<Player is playing media*/
} plusplayer_state;

/**
 * @brief  Enumerations for the display mode
 */
typedef enum plusplayer_display_mode {
  PLUSPLAYER_DISPLAY_MODE_LETTER_BOX,
  PLUSPLAYER_DISPLAY_MODE_ORIGIN_SIZE,
  PLUSPLAYER_DISPLAY_MODE_FULL_SCREEN,
  PLUSPLAYER_DISPLAY_MODE_CROPPED_FULL,
  PLUSPLAYER_DISPLAY_MODE_ORIGIN_OR_LETTER,
  PLUSPLAYER_DISPLAY_MODE_DST_ROI,
  PLUSPLAYER_DISPLAY_MODE_AUTO_ASPECT_RATIO,
  PLUSPLAYER_DISPLAY_MODE_ROI_AUTO_ASPECT_RATIO,
  PLUSPLAYER_DISPLAY_MODE_MAX
} plusplayer_display_mode;

/**
 * @brief  Enumerations for the display type
 */
typedef enum plusplayer_display_type {
  PLUSPLAYER_DISPLAY_TYPE_NONE,
  PLUSPLAYER_DISPLAY_TYPE_OVERLAY,
  PLUSPLAYER_DISPLAY_TYPE_EVAS,
  PLUSPLAYER_DISPLAY_TYPE_MIXER,
  PLUSPLAYER_DISPLAY_TYPE_OVERLAYSYNCUI
} plusplayer_display_type;

/**
 * @brief  Enumerations for the display rotation type
 */
typedef enum plusplayer_display_rotation_type {
  PLUSPLAYER_DISPLAY_ROTATION_TYPE_NONE,
  PLUSPLAYER_DISPLAY_ROTATION_TYPE_90,
  PLUSPLAYER_DISPLAY_ROTATION_TYPE_180,
  PLUSPLAYER_DISPLAY_ROTATION_TYPE_270
} plusplayer_display_rotation_type;

// TO DO eos cb's

// typedef void (*plusplayer_eos_cb)(void*);

//

typedef void (*plusplayer_error_cb)(plusplayer_error_type, void*);
typedef void (*plusplayer_prepare_async_done_cb)(bool, void*);
typedef void (*plusplayer_seek_done_cb)(void*);
typedef void* plusplayer_handle;

/**
 * @brief     Create a plusplayer handle.
 * @param     None
 * @return    return plusplayer handle pointer.
 * @code
 *            plusplayer_handle player = plusplayer_create();
 *            // ... your codes ...
 *            plusplayer_destroy(player);
 * @endcode
 * @pre       None
 * @post      The player state will be #PLUSPLAYER_STATE_NONE.
 * @exception None
 */
plusplayer_handle plusplayer_create();

/**
 * @brief     Open plusplayer handle.
 * @param     [in] handle : plusplayer handle
 * @param     [in] uri : content uri.
 * @return    @c plusplayer_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #plusplayer_ERROR_TYPE_NONE Successful
 * @retval    #plusplayer_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #plusplayer_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_handle player = plusplayer_create();
 *            plusplayer_open(player,uri);
 *            // ... your codes ...
 *            plusplayer_close(player);
 *            plusplayer_destroy(player);
 * @endcode
 * @pre       The player state must be #plusplayer_STATE_NONE.
 * @post      The player state will be #plusplayer_STATE_IDLE.
 * @exception None
 * @see       plusplayer_close()
 */
int plusplayer_open(plusplayer_handle handle, const char* uri);

/**
 * @brief     Prepares player.
 * @return    @c plusplayer_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #plusplayer_ERROR_TYPE_NONE Successful
 * @retval    #plusplayer_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #plusplayer_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_handle player = plusplayer_create();
 *            plusplayer_open(player,uri);
 *            plusplayer_prepare(player);
 *            // ... your codes ...
 * @endcode
 * @pre       The player state must be #plusplayer_STATE_IDLE.
 * @post      The player state will be #plusplayer_STATE_READY.
 * @exception None
 * @see       plusplayer_open()
 */
int plusplayer_prepare(plusplayer_handle handle);

/**
 * @brief     Start playback after prepare done.
 * @param     [in] handle : plusplayer handle.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * PLUSPLAYER_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            plusplayer_start(player);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state should be #PLUSPLAYER_STATE_READY.
 * @post      The player state will be #PLUSPLAYER_STATE_PLAYING.
 * @exception None
 * @see       plusplayer_open() \n
 *            plusplayer_prepare() \n
 *            plusplayer_start() \
 *            // ... your codes ...
 *            plusplayer_stop() \n
 *            plusplayer_close()
 */
int plusplayer_start(plusplayer_handle handle);

/**
 * @brief     Stop playing media content.
 * @param     [in] handle : PLUSPLAYER handle.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * PLUSPLAYER_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            plusplayer_stop(player);
 *             // ... your codes ...
 *            plusplayer_close(player);
 * @endcode
 * @pre       The player state must be all of #PLUSPLAYER_state except
 *            #PLUSPLAYER_STATE_NONE.
 * @post      The player state will be #PLUSPLAYER_STATE_IDLE.
 * @exception None
 * @remark    plusplayer_open() must be called once after player is stopped
 * @see       plusplayer_open() \n
 *            plusplayer_prepare() \n
 *            plusplayer_start() \n
 *            plusplayer_close()
 */
int plusplayer_stop(plusplayer_handle handle);

/**
 * @brief     Release all the player resources and all setting except callback
 *            functions.
 * @param     [in] handle : plusplayer handle.
 * @return    @c plusplayer_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #plusplayer_ERROR_TYPE_NONE Successful
 * @retval    #plusplayer_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #plusplayer_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @pre       The player state must be all of #plusplayer_state except
 *            #plusplayer_STATE_NONE.
 * @post      The player state will be #plusplayer_STATE_NONE.
 * @exception None
 * @see       plusplayer_open()
 */
int plusplayer_close(plusplayer_handle handle);

/**
 * @brief     Release a player handle after finish playing.
 * @param     [in] handle : plusplayer handle.
 * @return    @c plusplayer_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #plusplayer_ERROR_TYPE_NONE Successful
 * @retval    #plusplayer_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #plusplayer_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            refer to the sample code of plusplayer_create()
 * @endcode
 * @pre       The player state must be #plusplayer_STATE_NONE
 * @post      player handle will be removed.
 * @exception None
 * @see       plusplayer_create()
 */
int plusplayer_destroy(plusplayer_handle handle);

/**
 * @brief     Get current state of player.
 * @param     [in] handle : plusplayer handle.
 * @return    current #plusplayer_state of player.
 * @code
 *            plusplayer_handle player = plusplayer_create();
 *            // ... your codes ...
 *            plusplayer_state ret = plusplayer_get_state(player);
 *            // ... your codes ...
 *            plusplayer_destroy(player);
 * @endcode
 * @pre       None
 * @post      None
 * @exception None
 */
plusplayer_state plusplayer_get_state(plusplayer_handle handle);

/**
 * @brief     Set the video display with window object.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] type : display type.
 * @param     [in] window : the handle to display window.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_open(player);
 *            plusplayer_set_display(player,PLUSPLAYER_DISPLAY_TYPE_OVERLAY,window);
 *            // ... your codes ...
 *            plusplayer_close(player);
 * @endcode
 * @pre       The player state must be #PLUSPLAYER_STATE_IDLE.
 * @post      None
 * @exception None
 * @remark    Plusplayer is not supporting changing display. \n
 *            This API have to be called before calling
 *            plusplayer_prepare_async() to reflect the display type.
 * @see       plusplayer_open() \n
 *            plusplayer_set_display_mode() \n
 *            plusplayer_set_display_roi() \n
 *            plusplayer_set_display_visible()
 */
int plusplayer_set_display(plusplayer_handle handle,
                           plusplayer_display_type type, void* window);

/**
 * @brief     Set the video display with subsurface.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] type : display type.
 * @param     [in] subsurface : the ecore wayland subsurface handle.
 * @param     [in] x : the x coordinate of subsurface.
 * @param     [in] y : the y coordinate of subsurface.
 * @param     [in] width : the width of subsurface.
 * @param     [in] height : the height of subsurface.
 * @return    @c one of plusplayer_error_type values will be returned.
 * @pre       The player state must be #PLUSPLAYER_STATE_IDLE.
 * @post      None
 * @exception   None
 * @version   3.1
 * @see       plusplayer_set_display_mode() \n
 *            plusplayer_set_display_roi() \n
 *            plusplayer_set_display_visible()
 */
int plusplayer_set_display_subsurface(plusplayer_handle handle,
                                      plusplayer_display_type type,
                                      uint32_t subsurface, int x, int y,
                                      int width, int height);

/**
 * @brief     Set a callback function to be invoked when player is prepared to
 *            be started.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] prepare_async_done_cb : the repare async done callback
 * function to register.
 * @param     [in] userdata : userdata of prepare_async_done_cb()
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            refer to the sample code of plusplayer_prepare_async();
 * @endcode
 * @pre       The player state must be set to #PLUSPLAYER_STATE_NONE
 *            or #PLUSPLAYER_STATE_IDLE.
 * @post      plusplayer_prepare_async_done_cb() will be invoked.
 * @exception It is prohibited to call any player APIs at
 *            plusplayer_prepare_async_done_cb callback.
 * @remark    plusplayer_prepare_async_done_cb()
 *            if prepare_async_done_cb is set to null,
 *            plusplayer_prepare_async_done_cb() will not be
 *            invoked anymore.
 * @see       plusplayer_prepare_async
 */
int plusplayer_set_prepare_async_done_cb(
    plusplayer_handle handle,
    plusplayer_prepare_async_done_cb prepare_async_done_cb, void* userdata);

/**
 * @brief     Sets a callback function to be invoked when an error occurs.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] plusplayer_error_cb : the error callback function to
 * register.
 * @param     [in] userdata : userdata of plusplayer_error_cb()
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            static void OnError(const plusplayer_error_type err_code, void*
 *                userdata) {
 *                //Something you want to do when error occur
 *                printf ("OnError\n");
 *            }
 *            plusplayer_handle player = plusplayer_create();
 *            plusplayer_set_error_cb(player, OnError, nullptr);
 *            // ... your codes ...
 *            plusplayer_destroy(player);
 * @endcode
 * @pre       The player state must be set to #PLUSPLAYER_STATE_NONE
 *            or #PLUSPLAYER_STATE_IDLE.
 * @post      plusplayer_error_cb() will be invoked.
 * @exception It is prohibited to call any player APIs at
 *            plusplayer_error_cb callback.
 * @remark    plusplayer_error_cb()
 *            if plusplayer_error_cb is set to null,
 *            plusplayer_error_cb() will not be
 *            invoked anymore.
 */
int plusplayer_set_error_cb(plusplayer_handle handle,
                            plusplayer_error_cb error_cb, void* userdata);

/**
 * @brief     Set a callback function to be invoked when player seek is
 * completed.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] seek_done_cb : the seek done callback function to register.
 * @param     [in] userdata : userdata of plusplayer_seek_done_cb()
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            refer to the sample code of plusplayer_set_error_cb();
 * @endcode
 * @pre       The player state must be set to #PLUSPLAYER_STATE_NONE
 *            or #PLUSPLAYER_STATE_IDLE.
 * @post      plusplayer_seek_done_cb() will be invoked.
 *            if seek_done_cb is set to null, plusplayer_seek_done_cb() will
 *            not be invoked anymore.
 * @exception None
 */
int plusplayer_set_seek_done_cb(plusplayer_handle handle,
                                plusplayer_seek_done_cb seek_done_cb,
                                void* userdata);

/**
 * @brief     Prepare the player for playback, asynchronously.
 * @param     [in] handle : plusplayer handle.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            static void OnPrepareDone(bool ret, void* userdata) {
 *                //Something you want to do when prepare done, but, we strongly
 *                //recommend DO NOT CALL PLAYER APIs in this callbck
 *                printf("OnPrepareDone\n");
 *            }
 *            plusplayer_handle player = plusplayer_create();
 *            plusplayer_set_prepare_async_done_cb(player,
 * OnPrepareDone,nullptr); plusplayer_open(player);
 *            plusplayer_prepare_async(player);
 *            // ... your codes ...
 *            plusplayer_close(player);
 *            plusplayer_destroy(player);
 * @endcode
 * @pre       The player state must be #PLUSPLAYER_STATE_IDLE. \n
 *            Call at least one of plusplayer_set_video_stream_info() or
 *            plusplayer_set_audio_stream_info(). \n
 * @post      It invokes plusplayer_prepare_async_done_cb() when prepare is
 *            finished. \n
 *            Prepare result can be succeeded or not at this moment. \n
 *            If the result is succeeded, the player state will be
 *            #PLUSPLAYER_STATE_READY and one frame will be displayed
 *            unless plusplayer_set_display_visible() is set to @c false.
 * @exception None
 * @remark    plusplayer_prepare_async_done_cb() can be invoked only when as
 *            many es packets as at least one decoded frame is submitted. \n
 *            The player can receive es packets after
 *            plusplayer_ready_to_seek_cb() is called.
 * @see       plusplayer_open() \n
 *            plusplayer_stop() \n
 *            plusplayer_submit_packet() \n
 *            plusplayer_ready_to_prepare_cb() \n
 *            plusplayer_close()
 */
int plusplayer_prepare_async(plusplayer_handle handle);

/**
 * @brief     Pause playing media content.
 * @param     [in] handle : plusplayer handle.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            plusplayer_pause(player);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_READY or
 *            #PLUSPLAYER_STATE_PAUSED or #PLUSPLAYER_STATE_PLAYING.
 * @post      The player state will be #PLUSPLAYER_STATE_PAUSE.
 * @exception None
 * @see       plusplayer_start() \n
 *            plusplayer_resume() \n
 *            plusplayer_prepare_async()
 */
int plusplayer_pause(plusplayer_handle handle);

/**
 * @brief     Resume playing media content.
 * @param     [in] handle : plusplayer handle.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            plusplayer_pause(player);
 *            // ... your codes ...
 *            plusplayer_resume(player);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_PAUSED or
 *            #PLUSPLAYER_STATE_PLAYING.
 * @post      The player state will be #PLUSPLAYER_STATE_PLAYING.
 * @exception None
 * @see       plusplayer_start() \n
 *            plusplayer_pause() \n
 *            plusplayer_prepare_async()
 */
int plusplayer_resume(plusplayer_handle handle);

/**
 * @brief     Seek for playback, asynchronously.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] time : seek time default in milliseconds, can be set by
 * @plusplayer_set_timeunit_type
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            const uint64_t ms_to_seek = 0;
 *            plusplayer_seek(player,ms_to_seek);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_READY or
 *            #PLUSPLAYER_STATE_PAUSED or #PLUSPLAYER_STATE_PLAYING.
 *            In PLUSPLAYER_STATE_IDLE, this api can be called exceptionally
 *            between plusplayer_open() and plusplayer_prepare_async().
 *            the start time of plyabak can be set explicitly when starting
 *            first playback. In this case, plusplayer_set_seek_done_cb is not
 *            called.
 * @post      None
 * @exception None
 * @remark    plusplayer_set_seek_done_cb() will be invoked if seek operation
 *            is finished. \n
 *            Seek result can be succeeded or not at this moment. \n
 *            plusplayer_set_seek_done_cb() can be invoked only when as many
 *            es packets as at least one decoded frame is submitted. \n
 *            The player can receive es packets from seek time after
 *            plusplayer_ready_to_seek_cb() is invoked.
 * @see       plusplayer_ready_to_seek_cb() \n
 *            plusplayer_prepare_async()
 */
int plusplayer_seek(plusplayer_handle handle, uint64_t time);

/**
 * @brief     Enable prebuffer mode
 * @param     [in] handle : plusplayer handle.
 * @param     [in] prebuffer_mode : Boolean to set prebuffer mode
 * @code
 *            plusplayer_open(player);
 *            // ... your codes ...
 *            plusplayer_set_prebuffer_mode(player,prebuffer_mode);
 *            plusplayer_prepare(player);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_READY or
 *            #PLUSPLAYER_STATE_PAUSED or #PLUSPLAYER_STATE_PLAYING.
 * @post      The player state will be #PLUSPLAYER_STATE_PAUSE.
 * @exception None
 * @see       plusplayer_start() \n
 */
void plusplayer_set_prebuffer_mode(plusplayer_handle handle,
                                   bool prebuffer_mode);

/**
 * @brief     sets application id to resource manager to allocate
 *             hardware resources like decoder
 * @param     [in] handle : plusplayer handle.
 * @param     [in] app_id : app id
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_set_app_id(player,app_id);
 *            plusplayer_open(player,uri);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_READY or
 *            #PLUSPLAYER_STATE_PAUSED or #PLUSPLAYER_STATE_PLAYING.
 * @post      The player state will be #PLUSPLAYER_STATE_PAUSE.
 * @exception None
 * @see       plusplayer_start() \n
 */
int plusplayer_set_app_id(plusplayer_handle handle, char* app_id);

/**
 * @brief     Suspend playing media content. Player can be suspended and it will
 * be muted. \n The current playback status will be maintained internally \n App
 * can call suspend() when the app is switching to background
 * @param     [in] handle : plusplayer handle.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            plusplayer_suspend(player);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_PAUSED or
 *            #PLUSPLAYER_STATE_PLAYING.
 * @post      The player state will be #PLUSPLAYER_STATE_PLAYING.
 * @exception None
 * @see       plusplayer_start() \n
 *            plusplayer_pause() \n
 *            plusplayer_prepare_async()
 */
int plusplayer_suspend(plusplayer_handle handle);

/**
 * @brief     Restore the suspended player
 * @param     [in] handle : plusplayer handle.
 * @param     [in] target_state : State of player after restore \n
 *              playback will be started if State::kPlaying is set.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            plusplayer_suspend(player);
 *            // ... your codes ...
 *            plusplayer_restore(player,PLUSPLAYER_STATE_PLAYING);
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_PAUSED or
 *            #PLUSPLAYER_STATE_PLAYING.
 * @post      The player state will be #PLUSPLAYER_STATE_PLAYING.
 * @exception None
 * @see       plusplayer_start() \n
 *            plusplayer_pause() \n
 *            plusplayer_prepare_async()
 */
int plusplayer_restore(plusplayer_handle handle, plusplayer_state target_state);

/**
 * @brief     Get the current playing time of the associated media.
 * @param     [in] handle : plusplayer handle.
 * @param     [out] cur_time : current playing time default in milliseconds, can
 * be set by @plusplayer_set_timeunit_type
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            plusplayer_start(player);
 *            // ... your codes ...
 *            uint64_t cur_time = 0;
 *            plusplayer_get_playing_time(player, &cur_time);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player must be one of #PLUSPLAYER_STATE_PAUSE or
 *            #PLUSPLAYER_STATE_PLAYING.
 * @post      None
 * @exception None
 * @see       plusplayer_prepare_async()
 */
int plusplayer_get_playing_time(plusplayer_handle handle, uint64_t* cur_time);

/**
 * @brief     Set the video display mode.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] mode : display mode.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_open(player);
 *            plusplayer_set_display_mode(player,PLUSPLAYER_DISPLAY_MODE_DST_ROI);
 *            // ... your codes ...
 *            plusplayer_close(player);
 * @endcode
 * @pre       The player state can be all of #plusplayer_state except
 *            #PLUSPLAYER_STATE_NONE.
 * @post      None
 * @exception None
 * @remark    If no display is set, no operation is performed.
 * @see       plusplayer_open() \n
 *            plusplayer_set_display_mode() \n
 *            plusplayer_set_display_roi() \n
 *            plusplayer_set_display_visible()
 */
int plusplayer_set_display_mode(plusplayer_handle handle,
                                plusplayer_display_mode mode);

/**
 * @brief     Set the ROI(Region Of Interest) area of display.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] x : var startPointX in src video area.
 * @param     [in] y : var startPointY in src video area.
 * @param     [in] width : width of display in src video area.
 * @param     [in] height : height of display in src video area.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_open(player);
 *            plusplayer_set_display(player,PLUSPLAYER_DISPLAY_TYPE_OVERLAY,window);
 *            plusplayer_set_display_mode(player,PLUSPLAYER_DISPLAY_MODE_DST_ROI);
 *            plusplayer_set_display_roi(player,0,0,600,500);
 *            // ... your codes ...
 *            plusplayer_close(player);
 * @endcode
 * @pre       The player state can be all of #plusplayer_state except
 *            #PLUSPLAYER_STATE_NONE. \n
 *            Before set display ROI, #PLUSPLAYER_DISPLAY_MODE_DST_ROI
 *            must be set with plusplayer_set_display_mode().
 * @post      None
 * @exception None
 * @remark    The minimum value of width and height are 1.
 * @see       plusplayer_open() \n
 *            plusplayer_set_display() \n
 *            plusplayer_set_display_mode() \n
 *            plusplayer_set_display_visible()
 */
int plusplayer_set_display_roi(plusplayer_handle handle, int x, int y,
                               int width, int height);
/**
 * @brief     Set the rotate angle of the video display.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] rotation : the rotate angle of the display.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            plusplayer_open(player);
 *            plusplayer_set_display(player,PLUSPLAYER_DISPLAY_TYPE_OVERLAY,window);
 *            plusplayer_set_display_rotation(player_,PLUSPLAYER_DISPLAY_ROTATION_TYPE_90);
 *            // ... your codes ...
 *            plusplayer_close(player);
 * @endcode
 * @pre       The player state can be all of #plusplayer_state except
 *            #PLUSPLAYER_STATE_NONE.
 * @post      this API worked only when video sink created.
 * @exception None
 * @see       plusplayer_open() \n
 *            plusplayer_set_display()
 */
int plusplayer_set_display_rotation(plusplayer_handle handle,
                                    plusplayer_display_rotation_type rotation);

/**
 * @brief     Set buffer config parameters to buffer media contents
 * before starting playback.
 * @param     [in] config : config of buffer.\n
 *             config can be \n
 *               "total_buffer_size_in_byte" \n
 *               "total_buffer_size_in_time" \n
 *               "buffer_size_in_byte_for_play" \n
 *               "buffer_size_in_sec_for_play" \n
 *               "buffer_size_in_byte_for_resume" \n
 *               "buffer_size_in_sec_for_resume" \n
 *               "buffering_timeout_in_sec_for_play"
 * @param     [in] amount : data amount to be buffered
 * @pre       The player state can be all of #State except
 * #State::PLUSPLAYER_STATE_NONE
 * @post      same as @pre
 * @exception None
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 */
int plusplayer_set_buffer_config(plusplayer_handle handle, const char* option,
                                 int amount);

/**
 * @brief     Get the duration of the stream set by url.
 * @param     [out] duration_in_milliseconds : duration in milliseconds
 * @pre       The player must be one of PLUSPLAYER_STATE_TRACK_SOURCE_READY,
 * PLUSPLAYER_STATE_READY, PLUSPLAYER_STATE_PLAYING or PLUSPLAYER_STATE_PAUSED
 * @post      same as @pre
 * @exception None
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 */
int plusplayer_get_duration(plusplayer_handle handle,
                            int64_t* duration_in_milliseconds);

/**
 * @brief     Set the media playback rate.
 * @param     [in] handle : plusplayer handle.
 * @param     [in] playback_rate :  the playback rate from 0.0 to 2.0.
 * @param     [in] audio_mute :  the audio is mute on/off, true: mute on, false:
 * mute off.
 * @return    @c PLUSPLAYER_ERROR_TYPE_NONE on success,otherwise @c one of
 * plusplayer_error_type
 *            values will be returned.
 * @retval    #PLUSPLAYER_ERROR_TYPE_NONE Successful
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_PARAMETER Invalid parameter
 * @retval    #PLUSPLAYER_ERROR_TYPE_INVALID_OPERATION Internal operation
 * failed
 * @code
 *            prepare player done
 *            // ... your codes ...
 *            plusplayer_set_playback_rate(player,2,true);
 *            // ... your codes ...
 *            plusplayer_stop(player);
 * @endcode
 * @pre       The player state must be one of #PLUSPLAYER_STATE_READY or
 *            #PLUSPLAYER_STATE_PAUSED or #PLUSPLAYER_STATE_PLAYING. \n
 *            User has to push the data as fast as playback rate.
 * @post      None
 * @exception None
 * @see       plusplayer_prepare_async()
 */
int plusplayer_set_playback_rate(plusplayer_handle handle,
                                 const double playback_rate);

#ifdef __cplusplus
}
#endif

#endif  // __PLUSPLAYER_PLUSPLAYER_CAPI_PLUSPLAYER_CAPI_H__
