// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "examples/offscreen_glfw/client_offscreen_glfw.h"

#include "include/cef_app.h"

#include <fstream>

namespace offscreen_glfw {

static GLuint compileShader(GLenum shaderType, const GLchar* source) {
  GLuint shader = glCreateShader(shaderType);

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  CHECK(GL_TRUE == status);

  return shader;
}

void GLAPIENTRY
MessageCallback(GLenum source,
                GLenum type,
                GLuint id,
                GLenum severity,
                GLsizei length,
                const GLchar* message,
                const void* userParam)
{
  if(type == GL_DEBUG_TYPE_ERROR)
    LOG(ERROR) << "GL ERROR: type = " << type << ", severity = " << severity << ", message = " << message;
  else
    LOG(INFO) << "GL: type = " << type << ", severity = " << severity << ", message = " << message;
}

Client::Client() {
  LOG(INFO) << "Client::Client()";

  // Create a windowed mode window and its OpenGL context.
  window = decltype(window){glfwCreateWindow(640, 480, "CEF Offscreen GLFW Example", NULL, NULL)};
  CHECK(window);

  // Make the window's context current.
  glfwMakeContextCurrent(window.get());

  // Set the initial framebuffer width and height.
  glfwGetFramebufferSize(window.get(), &width, &height);

  CHECK(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

  // During init, enable debug output
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(MessageCallback, 0);

  glGenTextures(1, &texture);

  const GLchar* vertexShaderSource =
    "#version 330 core\n"
    "in vec2 position;\n"
    "out vec2 Texcoord;\n"
    "void main() {\n"
    "  Texcoord = (vec2(position.x + 1.0f, position.y - 1.0f) * 0.5);\n"
    "  Texcoord.y *= -1.0f;\n"
    "  gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\n"
    "}\n";
  const GLchar* fragmentShaderSource =
    "#version 330 core\n"
    "in vec2 Texcoord;\n"
    "out vec4 outputColor;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "  outputColor = texture2D(tex, Texcoord);\n"
    "}\n";

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if(GL_FALSE == isLinked)
  {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    LOG(ERROR) << "Failed to link OpenGL program: " << infoLog.data();

    // Provide the infolog in whatever manner you deem best.
    // Exit with failure.
    CHECK(false);
  }

  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  positionLoc = glGetAttribLocation(program, "position");

  float coords[] = {-1.0,-1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,-1.0,1.0,1.0,1.0};

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Client::~Client() {
  LOG(INFO) << "Client::~Client()";
}

void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title) {
  LOG(INFO) << "Client::OnTitleChange()";

  // Call the default shared implementation.
  // shared::OnTitleChange(browser, title);
}

void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "Client::OnAfterCreated()";

  // Call the default shared implementation.
  // shared::OnAfterCreated(browser);
}

bool Client::DoClose(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "Client::DoClose()";

  // Call the default shared implementation.
  // return shared::DoClose(browser);

  // Allow the close.
  return false;
}

void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "Client::OnBeforeClose()";

  // Call the default shared implementation.
  // shared::OnBeforeClose(browser);
}

void Client::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  LOG(INFO) << "Client::GetViewRect()";

  rect = CefRect(0, 0, width, height);
}

void Client::OnPaint(CefRefPtr<CefBrowser> browser,
                     PaintElementType type,
                     const RectList& dirtyRects,
                     const void* buffer,
                     int width,
                     int height) {
  glBindTexture(GL_TEXTURE_2D, texture);
  #define GL_BGRA_EXT 0x80E1
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (unsigned char*) buffer);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Client::draw() {
  glViewport(0, 0, width, height);

  glBindTexture(GL_TEXTURE_2D, texture);

  glUseProgram(program);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
  glUseProgram(0);

  glBindTexture(GL_TEXTURE_2D, 0);
}

}  // namespace offscreen_glfw
