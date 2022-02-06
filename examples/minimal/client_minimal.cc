// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "examples/minimal/client_minimal.h"

#include "examples/shared/client_util.h"

namespace minimal {

Client::Client() {
  LOG(INFO) << "Client::Client()";
}

Client::~Client() {
  LOG(INFO) << "Client::~Client()";
}

void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title) {
  LOG(INFO) << "Client::OnTitleChange()";

  // Call the default shared implementation.
  shared::OnTitleChange(browser, title);
}

void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "Client::OnAfterCreated()";

  // Call the default shared implementation.
  shared::OnAfterCreated(browser);
}

bool Client::DoClose(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "Client::DoClose()";

  // Call the default shared implementation.
  return shared::DoClose(browser);
}

void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "Client::OnBeforeClose()";

  // Call the default shared implementation.
  return shared::OnBeforeClose(browser);
}

}  // namespace minimal
