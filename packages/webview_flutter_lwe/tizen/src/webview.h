// Copyright 2021 Samsung Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_PLUGIN_WEBVIEW_H_
#define FLUTTER_PLUGIN_WEBVIEW_H_

#include <flutter/encodable_value.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_message_codec.h>
#include <flutter/texture_registrar.h>
#include <flutter_platform_view.h>

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>

#include "message_dispatcher.h"

typedef flutter::MethodCall<flutter::EncodableValue> FlMethodCall;
typedef flutter::MethodResult<flutter::EncodableValue> FlMethodResult;
typedef flutter::MethodChannel<flutter::EncodableValue> FlMethodChannel;

namespace LWE {
class WebContainer;
}

class BufferPool;
class BufferUnit;

class WebView : public PlatformView {
 public:
  WebView(flutter::PluginRegistrar* registrar, int view_id,
          flutter::TextureRegistrar* texture_registrar, double width,
          double height, const flutter::EncodableValue& params);
  ~WebView();

  virtual void Dispose() override;

  virtual void Resize(double width, double height) override;
  virtual void Touch(int type, int button, double x, double y, double dx,
                     double dy) override;
  virtual void SetDirection(int direction) override;

  virtual void ClearFocus() override {}

  virtual bool SendKey(const char* key, const char* string, const char* compose,
                       uint32_t modifiers, uint32_t scan_code,
                       bool is_down) override;

  LWE::WebContainer* GetWebViewInstance() { return webview_instance_; }

  FlutterDesktopGpuSurfaceDescriptor* ObtainGpuSurface(size_t width,
                                                       size_t height);

 private:
  void HandleWebViewMethodCall(const FlMethodCall& method_call,
                               std::unique_ptr<FlMethodResult> result);
  void HandleCookieMethodCall(const FlMethodCall& method_call,
                              std::unique_ptr<FlMethodResult> result);

  template <typename T>
  void SetBackgroundColor(const T& color);

  void RegisterJavaScriptChannelName(const std::string& name);
  void UnregisterJavaScriptChannelName(const std::string& name);
  std::string GetWebViewChannelName();
  std::string GetNavigationDelegateChannelName();

  void InitWebView();

  LWE::WebContainer* webview_instance_ = nullptr;
  flutter::TextureRegistrar* texture_registrar_;
  double width_;
  double height_;
  BufferUnit* working_surface_ = nullptr;
  BufferUnit* candidate_surface_ = nullptr;
  BufferUnit* rendered_surface_ = nullptr;
  bool is_mouse_lbutton_down_ = false;
  bool has_navigation_delegate_ = false;
  std::unique_ptr<MessageDispatcher> dispatcher_;
  std::unique_ptr<FlMethodChannel> webview_channel_;
  std::unique_ptr<FlMethodChannel> navigation_delegate_channel_;
  std::unique_ptr<flutter::TextureVariant> texture_variant_;
  std::mutex mutex_;
  std::unique_ptr<BufferPool> tbm_pool_;
  bool use_sw_backend_;
};

#endif  // FLUTTER_PLUGIN_WEBVIEW_H_
