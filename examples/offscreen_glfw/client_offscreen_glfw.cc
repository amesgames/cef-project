// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "examples/offscreen_glfw/client_offscreen_glfw.h"

// #include "examples/shared/client_util.h"

namespace offscreen_glfw {

Client::Client() 
    // Create a windowed mode window and its OpenGL context.
    : window{glfwCreateWindow(640, 480, "CEF Offscreen GLFW Example", NULL, NULL)} {
  DCHECK(window);

  // Make the window's context current.
  glfwMakeContextCurrent(window.get());

  // Set the initial framebuffer width and height.
  glfwGetFramebufferSize(window.get(), &width, &height);
}

void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title) {
  // Call the default shared implementation.
  // shared::OnTitleChange(browser, title);
  LOG(INFO) << "Client::OnTitleChange";
}

void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  // Call the default shared implementation.
  // shared::OnAfterCreated(browser);
  LOG(INFO) << "Client::OnAfterCreated";
}

bool Client::DoClose(CefRefPtr<CefBrowser> browser) {
  // Call the default shared implementation.
  // return shared::DoClose(browser);
  LOG(INFO) << "Client::DoClose";

  // Allow the close.
  return false;
}

void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  // Call the default shared implementation.
  // shared::OnBeforeClose(browser);
  LOG(INFO) << "Client::OnBeforeClose";
}

void Client::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  LOG(INFO) << "Client::GetViewRect";
  rect = CefRect(0, 0, width, height);
}

void Client::OnPaint(CefRefPtr<CefBrowser> browser,
                     PaintElementType type,
                     const RectList& dirtyRects,
                     const void* buffer,
                     int width,
                     int height) {
  
}

}  // namespace offscreen_glfw
