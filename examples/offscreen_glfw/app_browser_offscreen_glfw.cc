// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "examples/shared/app_factory.h"
#include "examples/shared/browser_util.h"

#include <GLFW/glfw3.h>

#include <dlfcn.h>

namespace offscreen_glfw {

// Offscreen GLFW implementation of CefApp for the browser process.
class BrowserApp : public CefApp, public CefBrowserProcessHandler {
 public:
  BrowserApp() {
    LOG(INFO) << "BrowserApp::BrowserApp()";

    // Initialize GLFW
    CHECK(glfwInit());

    // OpenGL core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  virtual ~BrowserApp() {
    LOG(INFO) << "BrowserApp::~BrowserApp()";

    // Terminate GLFW
    glfwTerminate();
  }

  // CefApp methods:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
    return this;
  }

  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override {
    // Command-line flags can be modified in this callback.
    // |process_type| is empty for the browser process.
    if (process_type.empty()) {
      command_line->AppendSwitch("off-screen-rendering-enabled");
      command_line->AppendSwitch("disable-gpu-compositing");
      command_line->AppendSwitch("enable-gpu");
      // @TODO: For Windows ???
      // command_line->AppendSwitch("shared-texture-enabled");
#if defined(OS_MACOSX)
      // Disable the macOS keychain prompt. Cookies will not be encrypted.
      command_line->AppendSwitch("use-mock-keychain");
#endif
    }
  }

  // CefBrowserProcessHandler methods:
  void OnContextInitialized() override {
    LOG(INFO) << "BrowserApp::OnContextInitialized";
  }

 private:
  IMPLEMENT_REFCOUNTING(BrowserApp);
  DISALLOW_COPY_AND_ASSIGN(BrowserApp);
};

}  // namespace offscreen_glfw

namespace shared {

CefRefPtr<CefApp> CreateBrowserProcessApp() {
  return new offscreen_glfw::BrowserApp();
}

}  // namespace shared
