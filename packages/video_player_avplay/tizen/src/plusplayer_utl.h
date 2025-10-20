// Copyright 2025 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_PLUGIN_PLUSPLAYER_UTL_H_
#define FLUTTER_PLUGIN_PLUSPLAYER_UTL_H_

#include <string>
#include <plusplayer_capi.h>

plusplayer_track_type_e ConvertTrackType(std::string track_type) {
  if (track_type == "video") {
    return plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_VIDEO;
  }
  else if (track_type == "audio") {
    return plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_AUDIO;
  }
  else {
    return plusplayer_track_type_e::PLUSPLAYER_TRACK_TYPE_SUBTITLE;
  }
}

plusplayer_property_e ConvertPropertyType(std::string property_type) {
    if (property_type == "ADAPTIVE_INFO") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_ADAPTIVE_INFO;
    } else if (property_type == "LISTEN_SPARSE_TRACK") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_LISTEN_SPARSE_TRACK;
    } else if (property_type == "AVAILABLE_BITRATE") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_AVAILABLE_BITRATE;
    } else if (property_type == "CURRENT_BANDWIDTH") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_CURRENT_BANDWIDTH;
    } else if (property_type == "GET_LIVE_DURATION") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_LIVE_DURATION;
    } else if (property_type == "IS_LIVE") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_IS_LIVE;
    } else if (property_type == "TOKEN") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_TOKEN;
    } else if (property_type == "OPEN_HTTP_HEADER") {
        return plusplayer_property_e.PLUSPLAYER_PROPERTY_OPEN_HTTP_HEADER;
    }
                                    
                          
  PLUSPLAYER_PROPERTY_CONFIG_LOW_LATENCY,
  PLUSPLAYER_PROPERTY_ATSC3_L1_SERVER_TIME,
  PLUSPLAYER_PROPERTY_AUDIO_DESCRIPTION,
  PLUSPLAYER_PROPERTY_PRESELECTION_TAG,
  PLUSPLAYER_PROPERTY_USE_MAIN_OUT_SHARE,
  PLUSPLAYER_PROPERTY_USER_LOW_LATENCY,
  PLUSPLAYER_PROPERTY_MAX_BANDWIDTH,
  PLUSPLAYER_PROPERTY_CUR_LATENCY,
  PLUSPLAYER_PROPERTY_IS_DVB_DASH,
  PLUSPLAYER_PROPERTY_LIVE_PLAYER_START,
  PLUSPLAYER_PROPERTY_START_DATE,
  PLUSPLAYER_PROPERTY_MPEGH_METADATA,
  PLUSPLAYER_PROPERTY_DASH_STREAM_INFO,
  PLUSPLAYER_PROPERTY_HTTP_HEADER
}

#endif
