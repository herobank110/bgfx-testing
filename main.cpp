#include "sdl_backend.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <iostream>
#include <thread>
using namespace std::literals;

bool quit{false};

void render() {
  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xebeefff);
  // Needs a dummy render primitive otherwise nothing will be rendered.
  bgfx::touch(0);
  bgfx::frame();
}

// Argc, argv and return 0 needed for SDL_main main function linkage.
int main(int, char **) {
  SDLBackend backend;

  std::thread render_thread{[&backend] {
    // Calling renderFrame establishes this thread as the render thread.
    bgfx::renderFrame();
    bgfx::Init init;
    init.platformData = backend.platform_data();
    if (!bgfx::init(init)) {
      throw std::runtime_error{"Failed to init bgfx"};
    }
    bgfx::reset(1280, 720, BGFX_RESET_VSYNC);
    bgfx::setViewRect(0, 0, 0, 1280, 720);
    while (!quit) {
      render();
    }
    std::cout << "Shutting down render thread... ";
    bgfx::shutdown();
    std::this_thread::sleep_for(2s);
    std::cout << "finished\n";
  }};

  backend.main_loop([] {});
  quit = true;
  render_thread.join();

  return 0;
}
