#pragma once

#include <SDL2/SDL.h>
#include <functional>
#include <memory>

namespace bgfx {
struct PlatformData;
}

/// @brief C++ object wrapper of SDL_Window.
class SDLWindow {
public:
  SDLWindow(const char *title, int x, int y, int w, int h, uint32_t flags);
  ~SDLWindow();
  auto raw() const { return raw_; }
  auto &operator*() const { return *raw(); }
  auto &operator->() const { return *raw(); }

private:
  SDL_Window *raw_;
};

/// @brief Windowing backend using SDL2.
class SDLBackend {
public:
  SDLBackend();
  ~SDLBackend(); 
  void main_loop(const std::function<void()> &per_frame_callback);
  auto &window() const { return *window_; }
  bgfx::PlatformData platform_data() const;

private:
  /// @returns Whether to quit the app.
  bool poll_events();
  std::unique_ptr<SDLWindow> window_;
};
