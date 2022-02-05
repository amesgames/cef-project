// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <X11/Xlib.h>

#include "include/base/cef_logging.h"

#include "examples/shared/app_factory.h"
#include "examples/shared/client_manager.h"
#include "examples/shared/main_util.h"

#include "examples/offscreen_glfw/client_offscreen_glfw.h"

#include <GLFW/glfw3.h>

using namespace shared;
using namespace offscreen_glfw;

// For interacting with both the client and the browser from GLFW callbacks
struct Context {
  Context(const char *url)
    : client{new Client()} {
    // Information used when creating the native window.
    CefWindowInfo window_info;

    // Do not create an OS window.
    window_info.SetAsWindowless(0);

    // Create the browser.
    browser = CefBrowserHost::CreateBrowserSync(window_info, client,
                                                url, CefBrowserSettings(),
                                                nullptr, nullptr);
  }

  Client *client;
  CefRefPtr<CefBrowser> browser;
};

// Main program entry point function.
#if defined(OS_WIN)
#pragma error "TODO: Implement Windows side"
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine,
                      int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);
  return shared::wWinMain(hInstance);
}
#else
namespace {

const char kStartupURL[] = "https://www.google.com";

int XIOErrorHandlerImpl(Display* display) {
  return 0;
}

void glfwframebuffersizeCallback(GLFWwindow *window, int width, int height) {
  auto context = (Context *)glfwGetWindowUserPointer(window);
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  context->client->resize(w, h);
  context->browser->GetHost()->WasResized();
}

} // namespace

// Entry point function for all processes.
int main(int argc, char* argv[]) {
  // Provide CEF with command-line arguments.
  CefMainArgs main_args(argc, argv);

  // Create a temporary CommandLine object.
  CefRefPtr<CefCommandLine> command_line = CreateCommandLine(main_args);

  // Create a CefApp of the correct process type.
  CefRefPtr<CefApp> app;
  switch(GetProcessType(command_line)) {
    case PROCESS_TYPE_BROWSER:
      app = CreateBrowserProcessApp();
      break;
    case PROCESS_TYPE_RENDERER:
      app = CreateRendererProcessApp();
      break;
    case PROCESS_TYPE_OTHER:
      app = CreateOtherProcessApp();
      break;
  }

  // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
  // that share the same executable. This function checks the command-line and,
  // if this is a sub-process, executes the appropriate logic.
  int exit_code = CefExecuteProcess(main_args, app, nullptr);
  if(exit_code >= 0) {
    // The sub-process has completed so return here.
    return exit_code;
  }

  // Install xlib IO error handler so that the application won't be terminated
  // on non-fatal errors.
  XSetIOErrorHandler(XIOErrorHandlerImpl);

  // Create the singleton manager instance.
  // ClientManager manager;

  // Specify CEF global settings here.
  CefSettings settings;
  settings.multi_threaded_message_loop = false;

  // Initialize CEF for the browser process. The first browser instance will be
  // created in CefBrowserProcessHandler::OnContextInitialized() after CEF has
  // been initialized.
  CefInitialize(main_args, settings, app, nullptr);

  // Create the context for both the client and browser.
  Context context(kStartupURL);

  // Setup GLFW callbacks
  glfwSetWindowUserPointer(context.client->getWindow(), &context);

  glfwSetFramebufferSizeCallback(context.client->getWindow(), glfwframebuffersizeCallback);

  // Set clear color to red so we can tell if OpenGL is working
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

  // Loop until the user closes the window
  while(!glfwWindowShouldClose(context.client->getWindow()))
  {
    CefDoMessageLoopWork();

    // Render here
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap front and back buffers
    glfwSwapBuffers(context.client->getWindow());

    // Poll for and process events
    glfwPollEvents();
  }

  // context.browser->GetHost()->CloseBrowser(/*force_close=*/true);
  // context.browser.reset();

  // Shut down CEF.
  CefShutdown();

  return 0;
}
#endif
