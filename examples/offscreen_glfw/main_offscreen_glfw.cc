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
  Context(const char *url) {
    LOG(INFO) << "Context::Context()";

    client = new Client();

    // Information used when creating the native window.
    CefWindowInfo window_info;

    // Do not create an OS window.
    window_info.SetAsWindowless(0);

    LOG(INFO) << "CefBrowserHost::CreateBrowserSync()";

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

void glfwFramebuffersizeCallback(GLFWwindow *window, int width, int height) {
  auto context = (Context *)glfwGetWindowUserPointer(window);
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  context->client->resize(w, h);
  context->browser->GetHost()->WasResized();
}

void glfwCursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  auto context = (Context *)glfwGetWindowUserPointer(window);
  context->client->mouse((int)xpos, (int)ypos);

  CefMouseEvent event;
  event.x = (int)xpos;
  event.y = (int)ypos;

  context->browser->GetHost()->SendMouseMoveEvent(event, false);
}

void glfwMouseButtonCallback(GLFWwindow* window, int btn, int state, int mods)
{
  auto context = (Context *)glfwGetWindowUserPointer(window);

  CefMouseEvent event;
  context->client->getMouse(&event.x, &event.y);

  cef_mouse_button_type_t type
    = (GLFW_MOUSE_BUTTON_LEFT == btn)
      ? cef_mouse_button_type_t::MBT_LEFT
      : (GLFW_MOUSE_BUTTON_RIGHT == btn)
        ? cef_mouse_button_type_t::MBT_RIGHT
        : cef_mouse_button_type_t::MBT_MIDDLE;

  bool mouseUp = (GLFW_RELEASE == state) ? true : false;

  context->browser->GetHost()->SendMouseClickEvent(event, type, mouseUp, /*clickCount=*/1);
}

// From ui/events/keycodes/keyboard_codes_posix.h.
enum KeyboardCode {
  VKEY_BACK = 0x08,
  VKEY_TAB = 0x09,
  VKEY_BACKTAB = 0x0A,
  VKEY_CLEAR = 0x0C,
  VKEY_RETURN = 0x0D,
  VKEY_SHIFT = 0x10,
  VKEY_CONTROL = 0x11,
  VKEY_MENU = 0x12,
  VKEY_PAUSE = 0x13,
  VKEY_CAPITAL = 0x14,
  VKEY_KANA = 0x15,
  VKEY_HANGUL = 0x15,
  VKEY_JUNJA = 0x17,
  VKEY_FINAL = 0x18,
  VKEY_HANJA = 0x19,
  VKEY_KANJI = 0x19,
  VKEY_ESCAPE = 0x1B,
  VKEY_CONVERT = 0x1C,
  VKEY_NONCONVERT = 0x1D,
  VKEY_ACCEPT = 0x1E,
  VKEY_MODECHANGE = 0x1F,
  VKEY_SPACE = 0x20,
  VKEY_PRIOR = 0x21,
  VKEY_NEXT = 0x22,
  VKEY_END = 0x23,
  VKEY_HOME = 0x24,
  VKEY_LEFT = 0x25,
  VKEY_UP = 0x26,
  VKEY_RIGHT = 0x27,
  VKEY_DOWN = 0x28,
  VKEY_SELECT = 0x29,
  VKEY_PRINT = 0x2A,
  VKEY_EXECUTE = 0x2B,
  VKEY_SNAPSHOT = 0x2C,
  VKEY_INSERT = 0x2D,
  VKEY_DELETE = 0x2E,
  VKEY_HELP = 0x2F,
  VKEY_0 = 0x30,
  VKEY_1 = 0x31,
  VKEY_2 = 0x32,
  VKEY_3 = 0x33,
  VKEY_4 = 0x34,
  VKEY_5 = 0x35,
  VKEY_6 = 0x36,
  VKEY_7 = 0x37,
  VKEY_8 = 0x38,
  VKEY_9 = 0x39,
  VKEY_A = 0x41,
  VKEY_B = 0x42,
  VKEY_C = 0x43,
  VKEY_D = 0x44,
  VKEY_E = 0x45,
  VKEY_F = 0x46,
  VKEY_G = 0x47,
  VKEY_H = 0x48,
  VKEY_I = 0x49,
  VKEY_J = 0x4A,
  VKEY_K = 0x4B,
  VKEY_L = 0x4C,
  VKEY_M = 0x4D,
  VKEY_N = 0x4E,
  VKEY_O = 0x4F,
  VKEY_P = 0x50,
  VKEY_Q = 0x51,
  VKEY_R = 0x52,
  VKEY_S = 0x53,
  VKEY_T = 0x54,
  VKEY_U = 0x55,
  VKEY_V = 0x56,
  VKEY_W = 0x57,
  VKEY_X = 0x58,
  VKEY_Y = 0x59,
  VKEY_Z = 0x5A,
  VKEY_LWIN = 0x5B,
  VKEY_COMMAND = VKEY_LWIN,  // Provide the Mac name for convenience.
  VKEY_RWIN = 0x5C,
  VKEY_APPS = 0x5D,
  VKEY_SLEEP = 0x5F,
  VKEY_NUMPAD0 = 0x60,
  VKEY_NUMPAD1 = 0x61,
  VKEY_NUMPAD2 = 0x62,
  VKEY_NUMPAD3 = 0x63,
  VKEY_NUMPAD4 = 0x64,
  VKEY_NUMPAD5 = 0x65,
  VKEY_NUMPAD6 = 0x66,
  VKEY_NUMPAD7 = 0x67,
  VKEY_NUMPAD8 = 0x68,
  VKEY_NUMPAD9 = 0x69,
  VKEY_MULTIPLY = 0x6A,
  VKEY_ADD = 0x6B,
  VKEY_SEPARATOR = 0x6C,
  VKEY_SUBTRACT = 0x6D,
  VKEY_DECIMAL = 0x6E,
  VKEY_DIVIDE = 0x6F,
  VKEY_F1 = 0x70,
  VKEY_F2 = 0x71,
  VKEY_F3 = 0x72,
  VKEY_F4 = 0x73,
  VKEY_F5 = 0x74,
  VKEY_F6 = 0x75,
  VKEY_F7 = 0x76,
  VKEY_F8 = 0x77,
  VKEY_F9 = 0x78,
  VKEY_F10 = 0x79,
  VKEY_F11 = 0x7A,
  VKEY_F12 = 0x7B,
  VKEY_F13 = 0x7C,
  VKEY_F14 = 0x7D,
  VKEY_F15 = 0x7E,
  VKEY_F16 = 0x7F,
  VKEY_F17 = 0x80,
  VKEY_F18 = 0x81,
  VKEY_F19 = 0x82,
  VKEY_F20 = 0x83,
  VKEY_F21 = 0x84,
  VKEY_F22 = 0x85,
  VKEY_F23 = 0x86,
  VKEY_F24 = 0x87,
  VKEY_NUMLOCK = 0x90,
  VKEY_SCROLL = 0x91,
  VKEY_LSHIFT = 0xA0,
  VKEY_RSHIFT = 0xA1,
  VKEY_LCONTROL = 0xA2,
  VKEY_RCONTROL = 0xA3,
  VKEY_LMENU = 0xA4,
  VKEY_RMENU = 0xA5,
  VKEY_BROWSER_BACK = 0xA6,
  VKEY_BROWSER_FORWARD = 0xA7,
  VKEY_BROWSER_REFRESH = 0xA8,
  VKEY_BROWSER_STOP = 0xA9,
  VKEY_BROWSER_SEARCH = 0xAA,
  VKEY_BROWSER_FAVORITES = 0xAB,
  VKEY_BROWSER_HOME = 0xAC,
  VKEY_VOLUME_MUTE = 0xAD,
  VKEY_VOLUME_DOWN = 0xAE,
  VKEY_VOLUME_UP = 0xAF,
  VKEY_MEDIA_NEXT_TRACK = 0xB0,
  VKEY_MEDIA_PREV_TRACK = 0xB1,
  VKEY_MEDIA_STOP = 0xB2,
  VKEY_MEDIA_PLAY_PAUSE = 0xB3,
  VKEY_MEDIA_LAUNCH_MAIL = 0xB4,
  VKEY_MEDIA_LAUNCH_MEDIA_SELECT = 0xB5,
  VKEY_MEDIA_LAUNCH_APP1 = 0xB6,
  VKEY_MEDIA_LAUNCH_APP2 = 0xB7,
  VKEY_OEM_1 = 0xBA,
  VKEY_OEM_PLUS = 0xBB,
  VKEY_OEM_COMMA = 0xBC,
  VKEY_OEM_MINUS = 0xBD,
  VKEY_OEM_PERIOD = 0xBE,
  VKEY_OEM_2 = 0xBF,
  VKEY_OEM_3 = 0xC0,
  VKEY_OEM_4 = 0xDB,
  VKEY_OEM_5 = 0xDC,
  VKEY_OEM_6 = 0xDD,
  VKEY_OEM_7 = 0xDE,
  VKEY_OEM_8 = 0xDF,
  VKEY_OEM_102 = 0xE2,
  VKEY_OEM_103 = 0xE3,  // GTV KEYCODE_MEDIA_REWIND
  VKEY_OEM_104 = 0xE4,  // GTV KEYCODE_MEDIA_FAST_FORWARD
  VKEY_PROCESSKEY = 0xE5,
  VKEY_PACKET = 0xE7,
  VKEY_DBE_SBCSCHAR = 0xF3,
  VKEY_DBE_DBCSCHAR = 0xF4,
  VKEY_ATTN = 0xF6,
  VKEY_CRSEL = 0xF7,
  VKEY_EXSEL = 0xF8,
  VKEY_EREOF = 0xF9,
  VKEY_PLAY = 0xFA,
  VKEY_ZOOM = 0xFB,
  VKEY_NONAME = 0xFC,
  VKEY_PA1 = 0xFD,
  VKEY_OEM_CLEAR = 0xFE,
  VKEY_UNKNOWN = 0,

  // POSIX specific VKEYs. Note that as of Windows SDK 7.1, 0x97-9F, 0xD8-DA,
  // and 0xE8 are unassigned.
  VKEY_WLAN = 0x97,
  VKEY_POWER = 0x98,
  VKEY_BRIGHTNESS_DOWN = 0xD8,
  VKEY_BRIGHTNESS_UP = 0xD9,
  VKEY_KBD_BRIGHTNESS_DOWN = 0xDA,
  VKEY_KBD_BRIGHTNESS_UP = 0xE8,

  // Windows does not have a specific key code for AltGr. We use the unused 0xE1
  // (VK_OEM_AX) code to represent AltGr, matching the behaviour of Firefox on
  // Linux.
  VKEY_ALTGR = 0xE1,
  // Windows does not have a specific key code for Compose. We use the unused
  // 0xE6 (VK_ICO_CLEAR) code to represent Compose.
  VKEY_COMPOSE = 0xE6,
};

KeyboardCode KeyboardCodeFromGlfwKey(int key) {
  switch(key) {
    case GLFW_KEY_BACKSPACE:
      return VKEY_BACK;
    case GLFW_KEY_DELETE:
      return VKEY_DELETE;
    case GLFW_KEY_TAB:
      return VKEY_TAB;
    case GLFW_KEY_ENTER:
      return VKEY_RETURN;
    case GLFW_KEY_HOME:
      return VKEY_HOME;
    case GLFW_KEY_END:
      return VKEY_END;
    case GLFW_KEY_PAGE_UP:
      return VKEY_PRIOR;
    case GLFW_KEY_PAGE_DOWN:
      return VKEY_NEXT;
    case GLFW_KEY_LEFT:
      return VKEY_LEFT;
    case GLFW_KEY_RIGHT:
      return VKEY_RIGHT;
    case GLFW_KEY_DOWN:
      return VKEY_DOWN;
    case GLFW_KEY_UP:
      return VKEY_UP;
    case GLFW_KEY_ESCAPE:
      return VKEY_ESCAPE;

    case GLFW_KEY_SPACE: // < 128
    case GLFW_KEY_A: // < 128
    case GLFW_KEY_B: // < 128
    case GLFW_KEY_C: // < 128
    case GLFW_KEY_D: // < 128
    case GLFW_KEY_E: // < 128
    case GLFW_KEY_F: // < 128
    case GLFW_KEY_G: // < 128
    case GLFW_KEY_H: // < 128
    case GLFW_KEY_I: // < 128
    case GLFW_KEY_J: // < 128
    case GLFW_KEY_K: // < 128
    case GLFW_KEY_L: // < 128
    case GLFW_KEY_M: // < 128
    case GLFW_KEY_N: // < 128
    case GLFW_KEY_O: // < 128
    case GLFW_KEY_P: // < 128
    case GLFW_KEY_Q: // < 128
    case GLFW_KEY_R: // < 128
    case GLFW_KEY_S: // < 128
    case GLFW_KEY_T: // < 128
    case GLFW_KEY_U: // < 128
    case GLFW_KEY_V: // < 128
    case GLFW_KEY_W: // < 128
    case GLFW_KEY_X: // < 128
    case GLFW_KEY_Y: // < 128
    case GLFW_KEY_Z: // < 128
    case GLFW_KEY_0: // < 128
    case GLFW_KEY_1: // < 128
    case GLFW_KEY_2: // < 128
    case GLFW_KEY_3: // < 128
    case GLFW_KEY_4: // < 128
    case GLFW_KEY_5: // < 128
    case GLFW_KEY_6: // < 128
    case GLFW_KEY_7: // < 128
    case GLFW_KEY_8: // < 128
    case GLFW_KEY_9: // < 128
      return static_cast<KeyboardCode>(key);

    case GLFW_KEY_KP_0:
    case GLFW_KEY_KP_1:
    case GLFW_KEY_KP_2:
    case GLFW_KEY_KP_3:
    case GLFW_KEY_KP_4:
    case GLFW_KEY_KP_5:
    case GLFW_KEY_KP_6:
    case GLFW_KEY_KP_7:
    case GLFW_KEY_KP_8:
    case GLFW_KEY_KP_9:
      return static_cast<KeyboardCode>(VKEY_NUMPAD0 + (key - GLFW_KEY_KP_0));

    case GLFW_KEY_KP_MULTIPLY:
      return VKEY_MULTIPLY;
    case GLFW_KEY_KP_ADD:
      return VKEY_ADD;
    // @TODO: Separator
    // case XK_KP_Separator:
    //   return VKEY_SEPARATOR;
    case GLFW_KEY_KP_SUBTRACT:
      return VKEY_SUBTRACT;
    case GLFW_KEY_KP_DECIMAL:
      return VKEY_DECIMAL;
    case GLFW_KEY_KP_DIVIDE:
      return VKEY_DIVIDE;
    case GLFW_KEY_KP_EQUAL:
    case GLFW_KEY_EQUAL:
      return VKEY_OEM_PLUS;
    case GLFW_KEY_COMMA:
      return VKEY_OEM_COMMA;
    case GLFW_KEY_MINUS:
      return VKEY_OEM_MINUS;
    case GLFW_KEY_PERIOD:
      return VKEY_OEM_PERIOD;
    case GLFW_KEY_SEMICOLON:
      return VKEY_OEM_1;
    case GLFW_KEY_SLASH:
      return VKEY_OEM_2;
    case GLFW_KEY_GRAVE_ACCENT:
      return VKEY_OEM_3;
    case GLFW_KEY_LEFT_BRACKET:
      return VKEY_OEM_4;
    case GLFW_KEY_BACKSLASH:
      return VKEY_OEM_5;
    case GLFW_KEY_RIGHT_BRACKET:
      return VKEY_OEM_6;
    case GLFW_KEY_APOSTROPHE:
      return VKEY_OEM_7;
    // case XK_ISO_Level5_Shift:
    //   return VKEY_OEM_8;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
      return VKEY_SHIFT;
    case GLFW_KEY_LEFT_CONTROL:
    case GLFW_KEY_RIGHT_CONTROL:
      return VKEY_CONTROL;
    case GLFW_KEY_LEFT_ALT:
    case GLFW_KEY_RIGHT_ALT:
      return VKEY_MENU;
    // case XK_ISO_Level3_Shift:
    //   return VKEY_ALTGR;
    // case XK_Multi_key:
    //   return VKEY_COMPOSE;
    case GLFW_KEY_PAUSE:
      return VKEY_PAUSE;
    case GLFW_KEY_CAPS_LOCK:
      return VKEY_CAPITAL;
    case GLFW_KEY_NUM_LOCK:
      return VKEY_NUMLOCK;
    case GLFW_KEY_SCROLL_LOCK:
      return VKEY_SCROLL;
    // case XK_Select:
    //   return VKEY_SELECT;
    case GLFW_KEY_PRINT_SCREEN:
      return VKEY_PRINT;
    // case XK_Execute:
    //   return VKEY_EXECUTE;
    case GLFW_KEY_INSERT:
      return VKEY_INSERT;
    // case XK_Help:
    //   return VKEY_HELP;
    case GLFW_KEY_LEFT_SUPER:
      return VKEY_LWIN;
    case GLFW_KEY_RIGHT_SUPER:
      return VKEY_RWIN;
    case GLFW_KEY_MENU:
      return VKEY_APPS;

    case GLFW_KEY_F1:
    case GLFW_KEY_F2:
    case GLFW_KEY_F3:
    case GLFW_KEY_F4:
    case GLFW_KEY_F5:
    case GLFW_KEY_F6:
    case GLFW_KEY_F7:
    case GLFW_KEY_F8:
    case GLFW_KEY_F9:
    case GLFW_KEY_F10:
    case GLFW_KEY_F11:
    case GLFW_KEY_F12:
    case GLFW_KEY_F13:
    case GLFW_KEY_F14:
    case GLFW_KEY_F15:
    case GLFW_KEY_F16:
    case GLFW_KEY_F17:
    case GLFW_KEY_F18:
    case GLFW_KEY_F19:
    case GLFW_KEY_F20:
    case GLFW_KEY_F21:
    case GLFW_KEY_F22:
    case GLFW_KEY_F23:
    case GLFW_KEY_F24:
      return static_cast<KeyboardCode>(VKEY_F1 + (key - GLFW_KEY_F1));

    // case XK_KP_F1:
    // case XK_KP_F2:
    // case XK_KP_F3:
    // case XK_KP_F4:
    //   return static_cast<KeyboardCode>(VKEY_F1 + (keysym - XK_KP_F1));

    // case XK_guillemotleft:
    // case XK_guillemotright:
    // case XK_degree:
    // // In the case of canadian multilingual keyboard layout, VKEY_OEM_102 is
    // // assigned to ugrave key.
    // case XK_ugrave:
    // case XK_Ugrave:
    // case XK_brokenbar:
    //   return VKEY_OEM_102;  // international backslash key in 102 keyboard.

    // When evdev is in use, /usr/share/X11/xkb/symbols/inet maps F13-18 keys
    // to the special XF86XK symbols to support Microsoft Ergonomic keyboards:
    // https://bugs.freedesktop.org/show_bug.cgi?id=5783
    // In Chrome, we map these X key symbols back to F13-18 since we don't have
    // VKEYs for these XF86XK symbols.
    // case XF86XK_Tools:
    //   return VKEY_F13;
    // case XF86XK_Launch5:
    //   return VKEY_F14;
    // case XF86XK_Launch6:
    //   return VKEY_F15;
    // case XF86XK_Launch7:
    //   return VKEY_F16;
    // case XF86XK_Launch8:
    //   return VKEY_F17;
    // case XF86XK_Launch9:
    //   return VKEY_F18;
    // case XF86XK_Refresh:
    // case XF86XK_History:
    // case XF86XK_OpenURL:
    // case XF86XK_AddFavorite:
    // case XF86XK_Go:
    // case XF86XK_ZoomIn:
    // case XF86XK_ZoomOut:
    //   // ui::AcceleratorGtk tries to convert the XF86XK_ keysyms on Chrome
    //   // startup. It's safe to return VKEY_UNKNOWN here since ui::AcceleratorGtk
    //   // also checks a Gdk keysym. http://crbug.com/109843
    //   return VKEY_UNKNOWN;

    // For supporting multimedia buttons on a USB keyboard.
    // case XF86XK_Back:
    //   return VKEY_BROWSER_BACK;
    // case XF86XK_Forward:
    //   return VKEY_BROWSER_FORWARD;
    // case XF86XK_Reload:
    //   return VKEY_BROWSER_REFRESH;
    // case XF86XK_Stop:
    //   return VKEY_BROWSER_STOP;
    // case XF86XK_Search:
    //   return VKEY_BROWSER_SEARCH;
    // case XF86XK_Favorites:
    //   return VKEY_BROWSER_FAVORITES;
    // case XF86XK_HomePage:
    //   return VKEY_BROWSER_HOME;
    // case XF86XK_AudioMute:
    //   return VKEY_VOLUME_MUTE;
    // case XF86XK_AudioLowerVolume:
    //   return VKEY_VOLUME_DOWN;
    // case XF86XK_AudioRaiseVolume:
    //   return VKEY_VOLUME_UP;
    // case XF86XK_AudioNext:
    //   return VKEY_MEDIA_NEXT_TRACK;
    // case XF86XK_AudioPrev:
    //   return VKEY_MEDIA_PREV_TRACK;
    // case XF86XK_AudioStop:
    //   return VKEY_MEDIA_STOP;
    // case XF86XK_AudioPlay:
    //   return VKEY_MEDIA_PLAY_PAUSE;
    // case XF86XK_Mail:
    //   return VKEY_MEDIA_LAUNCH_MAIL;
    // case XF86XK_LaunchA:  // F3 on an Apple keyboard.
    //   return VKEY_MEDIA_LAUNCH_APP1;
    // case XF86XK_LaunchB:  // F4 on an Apple keyboard.
    // case XF86XK_Calculator:
    //   return VKEY_MEDIA_LAUNCH_APP2;
    // case XF86XK_WLAN:
    //   return VKEY_WLAN;
    // case XF86XK_PowerOff:
    //   return VKEY_POWER;
    // case XF86XK_MonBrightnessDown:
    //   return VKEY_BRIGHTNESS_DOWN;
    // case XF86XK_MonBrightnessUp:
    //   return VKEY_BRIGHTNESS_UP;
    // case XF86XK_KbdBrightnessDown:
    //   return VKEY_KBD_BRIGHTNESS_DOWN;
    // case XF86XK_KbdBrightnessUp:
    //   return VKEY_KBD_BRIGHTNESS_UP;
    
    // @TODO: some keycodes are still missing.
    default:
      return VKEY_UNKNOWN;
  }
}

// From content/browser/renderer_host/input/web_input_event_util_posix.cc.
KeyboardCode GetWindowsKeyCodeWithoutLocation(KeyboardCode key_code) {
  switch(key_code) {
    case VKEY_LCONTROL:
    case VKEY_RCONTROL:
      return VKEY_CONTROL;
    case VKEY_LSHIFT:
    case VKEY_RSHIFT:
      return VKEY_SHIFT;
    case VKEY_LMENU:
    case VKEY_RMENU:
      return VKEY_MENU;
    default:
      return key_code;
  }
}

int GetCefStateModifiers(int mods) {
  int modifiers = 0;
  if(mods & GLFW_MOD_SHIFT)
    modifiers |= EVENTFLAG_SHIFT_DOWN;
  if(mods & GLFW_MOD_CAPS_LOCK)
    modifiers |= EVENTFLAG_CAPS_LOCK_ON;
  if(mods & GLFW_MOD_CONTROL)
    modifiers |= EVENTFLAG_CONTROL_DOWN;
  if(mods & GLFW_MOD_ALT)
    modifiers |= EVENTFLAG_ALT_DOWN;

  // @TODO: Mouse button modifier flags
  // if(mods & GDK_BUTTON1_MASK)
  //   modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  // if(mods & GDK_BUTTON2_MASK)
  //   modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  // if(mods & GDK_BUTTON3_MASK)
  //   modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

  return modifiers;
}

// From content/browser/renderer_host/input/web_input_event_builders_gtk.cc.
// Gets the corresponding control character of a specified key code. See:
// http://en.wikipedia.org/wiki/Control_characters
// We emulate Windows behavior here.
int GetControlCharacter(KeyboardCode windows_key_code, bool shift) {
  // ctrl-A ~ ctrl-Z map to \x01 ~ \x1A
  if(windows_key_code >= VKEY_A && windows_key_code <= VKEY_Z)
    return windows_key_code - VKEY_A + 1;

  if(shift)
    // following graphics chars require shift key to input.
    switch(windows_key_code) {
      // ctrl-@ maps to \x00 (Null byte)
      case VKEY_2:
        return 0;
      // ctrl-^ maps to \x1E (Record separator, Information separator two)
      case VKEY_6:
        return 0x1E;
      // ctrl-_ maps to \x1F (Unit separator, Information separator one)
      case VKEY_OEM_MINUS:
        return 0x1F;
      // Returns 0 for all other keys to avoid inputting unexpected chars.
      default:
        return 0;
    }
  else
    switch(windows_key_code) {
      // ctrl-[ maps to \x1B (Escape)
      case VKEY_OEM_4:
        return 0x1B;
      // ctrl-\ maps to \x1C (File separator, Information separator four)
      case VKEY_OEM_5:
        return 0x1C;
      // ctrl-] maps to \x1D (Group separator, Information separator three)
      case VKEY_OEM_6:
        return 0x1D;
      // ctrl-Enter maps to \x0A (Line feed)
      case VKEY_RETURN:
        return 0x0A;
      // Returns 0 for all other keys to avoid inputting unexpected chars.
      default:
        return 0;
    }
}

cef_key_event_type_t GetCefType(int action) {
  switch(action) {
    case GLFW_PRESS:
      return KEYEVENT_RAWKEYDOWN;
    case GLFW_REPEAT:
      return KEYEVENT_KEYDOWN;
    case GLFW_RELEASE:
    default:
      return KEYEVENT_KEYUP;
  }
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  LOG(INFO) << "glfwKeyCallback(): key = " << key << ", scancode = " << scancode
    << ", action = " << action << ", mods = " << mods;

  // For now, do not worry about unknown keys.
  if(GLFW_KEY_UNKNOWN == key)
    return;

  auto context = (Context *)glfwGetWindowUserPointer(window);

  // @TODO: OS X command key
  CHECK(0 == (GLFW_MOD_SUPER & mods));

  CefKeyEvent event;
  KeyboardCode windows_key_code = KeyboardCodeFromGlfwKey(key);
  event.windows_key_code =
      GetWindowsKeyCodeWithoutLocation(windows_key_code);
  event.native_key_code = scancode;

  event.modifiers = GetCefStateModifiers(mods);
  if(key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL)
    event.modifiers |= EVENTFLAG_IS_KEY_PAD;
  if(event.modifiers & EVENTFLAG_ALT_DOWN)
    event.is_system_key = true;

  // We need to treat the enter key as a key press of character \r.  This
  // is apparently just how webkit handles it and what it expects.
  if(windows_key_code == VKEY_RETURN)
    event.unmodified_character = '\r';
  else {
    // @TODO: fix for non BMP chars
    const char* name = glfwGetKeyName(key, scancode);
    if(name)
      event.unmodified_character = (char16)name[0] | (char16)((name[0] ? name[1] : 0) << 8);
  }

  // If ctrl key is pressed down, then control character shall be input.
  if(event.modifiers & EVENTFLAG_CONTROL_DOWN)
    event.character = GetControlCharacter(
        windows_key_code, event.modifiers & EVENTFLAG_SHIFT_DOWN);
  else
    event.character = event.unmodified_character;

  event.type = GetCefType(action);

  context->browser->GetHost()->SendKeyEvent(event);

  if(windows_key_code == VKEY_RETURN) {
    if(KEYEVENT_KEYUP != action) {
      event.type = cef_key_event_type_t::KEYEVENT_CHAR;
      context->browser->GetHost()->SendKeyEvent(event);
    }
  }
}

void glfwCharCallback(GLFWwindow *window, unsigned int codepoint) {
  LOG(INFO) << "glfwCharCallback(): codepoint = " << codepoint;

  auto context = (Context *)glfwGetWindowUserPointer(window);

  CefKeyEvent event;
  event.type = cef_key_event_type_t::KEYEVENT_CHAR;
  event.character = codepoint;

  context->browser->GetHost()->SendKeyEvent(event);
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
  LOG(INFO) << "CefInitialize()";
  CefInitialize(main_args, settings, app, nullptr);

  // Create the context for both the client and browser.
  Context context(kStartupURL);

  // Setup GLFW callbacks
  glfwSetWindowUserPointer(context.client->getWindow(), &context);

  glfwSetInputMode(context.client->getWindow(), GLFW_LOCK_KEY_MODS, GLFW_TRUE);

  glfwSetFramebufferSizeCallback(context.client->getWindow(), glfwFramebuffersizeCallback);
  glfwSetCursorPosCallback(context.client->getWindow(), glfwCursorPosCallback);
  glfwSetMouseButtonCallback(context.client->getWindow(), glfwMouseButtonCallback);
  glfwSetKeyCallback(context.client->getWindow(), glfwKeyCallback);
  glfwSetCharCallback(context.client->getWindow(), glfwCharCallback);

  // Set clear color to red.
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

  // Loop until the user closes the window
  while(!glfwWindowShouldClose(context.client->getWindow()))
  {
    CefDoMessageLoopWork();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the browser to the window.
    context.client->draw();

    // Swap front and back buffers
    glfwSwapBuffers(context.client->getWindow());

    // Poll for and process events
    glfwPollEvents();
  }

  // Run the CEF message loop. This will block until CefQuitMessageLoop() is
  // called.
  // LOG(INFO) << "CefRunMessageLoop()";
  // CefRunMessageLoop();

  context.browser->GetHost()->CloseBrowser(/*force_close=*/true);
  context.browser.reset();

  // CefDoMessageLoopWork();

  // Shut down CEF.
  LOG(INFO) << "CefShutdown()";
  CefShutdown();

  return 0;
}
#endif
