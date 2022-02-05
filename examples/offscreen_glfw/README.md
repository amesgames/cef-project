# Offscreen GLFW Example Application

This directory contains the "offscreen_glfw" target which demonstrates the Offscreen functionality with GLFW using the [shared library](../shared) target.

See the [shared library](../shared) target for details common to all executable targets.

## Implementation Overview

The "offscreen_glfw" target is implemented as follows:

 * Define the target-specific [CMake](https://cmake.org/) build configuration in the [CMakeLists.txt](CMakeLists.txt) file.
 * Call the shared [entry point functions](https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-entry-point-function) that initialize, run and shut down CEF.
     * Main process (all platforms): [main_offscreen_glfw.cc](main_offscreen_glfw.cc)
     * Sub-process (macOS only): [process_helper_mac_offscreen_glfw.cc](process_helper_mac_offscreen_glfw.cc)
 * Implement the [shared::Create*ProcessApp](../shared/app_factory.h) functions to create a [CefApp](https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-cefapp) instance appropriate to the [process type](https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-processes).
     * Browser process: [app_browser_offscreen_glfw.cc](app_browser_offscreen_glfw.cc) implements the `shared::CreateBrowserProcessApp` method to return a `CefApp` instance.
         * The `OnContextInitialized` method creates the initial [CefBrowser](https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-cefbrowser-and-cefframe) instance using the [shared::CreateBrowser](../shared/browser_util.h) helper function.
     * Renderer process: [app_renderer_offscreen_glfw.cc](app_renderer_offscreen_glfw.cc) implements the `shared::CreateRendererProcessApp` method to return NULL (no `CefApp` for this process).
     * Other sub-processes: [app_other_offscreen_glfw.cc](app_other_offscreen_glfw.cc) implements the `shared::CreateOtherProcessApp` method to return NULL (no `CefApp` for this process).
 * Provide a concrete [CefClient](https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-cefclient) implementation in [client_offscreen_glfw.cc](client_offscreen_glfw.cc) and [client_offscreen_glfw.h](client_offscreen_glfw.h) to handle [CefBrowser](https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-cefbrowser-and-cefframe) callbacks.

Other targets use the offscreen_glfw implementation source files (*_offscreen_glfw.cc) in cases where example-specific customization is not required.

## Configuration

See the [shared library](../shared) target for configuration details.

## Setup and Build

See the [shared library](../shared) target for setup and build instructions.
