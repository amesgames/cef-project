// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_EXAMPLES_OFFSCREEN_GLFW_CLIENT_OFFSCREEN_GLFW_H_
#define CEF_EXAMPLES_OFFSCREEN_GLFW_CLIENT_OFFSCREEN_GLFW_H_

#include "include/cef_client.h"

#include <GLFW/glfw3.h>

namespace offscreen_glfw {

// Offscreen GLFW implementation of client handlers.
class Client : public CefClient,
               public CefDisplayHandler,
               public CefLifeSpanHandler,
               public CefRenderHandler {
 public:
  Client();
  virtual ~Client();

  // CefClient methods:
  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }

  // CefDisplayHandler methods:
  void OnTitleChange(CefRefPtr<CefBrowser> browser,
                     const CefString& title) override;

  // CefLifeSpanHandler methods:
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefRenderHandler methods:
  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
  void OnPaint(CefRefPtr<CefBrowser> browser,
               PaintElementType type,
               const RectList& dirtyRects,
               const void* buffer,
               int width,
               int height) override;

  // Other methods:
  GLFWwindow *getWindow() {
    return window.get();
  }
  void resize(int w, int h) {
    width = w;
    height = h;
  }
  void mouse(int x, int y) {
    mouseX = x;
    mouseY = y;
  }
  void getMouse(int *x, int *y) const {
    *x = mouseX;
    *y = mouseY;
  }
  void draw();

 private:
  IMPLEMENT_REFCOUNTING(Client);
  DISALLOW_COPY_AND_ASSIGN(Client);

  struct WindowDeleter {
    void operator()(GLFWwindow* window) { glfwDestroyWindow(window); }
  };

  std::unique_ptr<GLFWwindow, WindowDeleter> window;
  int width;
  int height;
  int mouseX;
  int mouseY;

  GLuint program;
  GLuint vao;
  GLuint vbo;
  GLuint texture;
  GLuint positionLoc;
};

}  // namespace offscreen_glfw

#endif  // CEF_EXAMPLES_OFFSCREEN_GLFW_CLIENT_OFFSCREEN_GLFW_H_
