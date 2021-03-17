#include "sdl_backend.hpp"
#include <SDL2/SDL_platform.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <cassert>
#include <stdexcept>

SDLWindow::SDLWindow(
    const char *title, int x, int y, int w, int h, uint32_t flags) {
  if (raw_ = SDL_CreateWindow(title, x, y, w, h, flags); raw_ == nullptr) {
    throw std::runtime_error{SDL_GetError()};
  }
}

SDLWindow::~SDLWindow() {
  SDL_DestroyWindow(raw_);
}

SDLBackend::SDLBackend() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error{SDL_GetError()};
  }

  window_ = std::make_unique<SDLWindow>(
      "BGFX SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
}

SDLBackend::~SDLBackend() {
  SDL_Quit();
}

void SDLBackend::main_loop(const std::function<void()> &per_frame_callback) {
  bool quit{false};
  while (!quit) {
    quit = poll_events();
    per_frame_callback();
  }
}

bgfx::PlatformData SDLBackend::platform_data() const {
  SDL_SysWMinfo wmi;
  SDL_VERSION(&wmi.version);
  [[maybe_unused]] const bool is_valid{
      static_cast<bool>(SDL_GetWindowWMInfo(window().raw(), &wmi))};
  // Could fail in case of invalid SDL version or if not implemented on
  // platform. But really it should always be valid!
  assert(is_valid);
  bgfx::PlatformData pd;

#if __WIN32__
  pd.nwh = wmi.info.win.window;
#elif __MACOSX__
  pd.nwh = wmi.info.cocoa.window;
#elif __LINUX__
  pd.ndt = wmi.info.x11.display;
  pd.nwh = wmi.info.x11.window;
#else
#  error Only Windows, Mac and Linux platforms implemented
// TODO: Test on other platforms SDL supports. May need to set other pd fields.
#endif

  return pd;
}

bool SDLBackend::poll_events() {
  bool quit{false};
  SDL_Event current_event;
  while (SDL_PollEvent(&current_event) != 0) {
    if (current_event.type == SDL_QUIT) {
      quit = true;
    }
  }
  return quit;
}
