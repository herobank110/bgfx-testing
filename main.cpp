#include "sdl_backend.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <iostream>

void render() {
  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xebeefff);
  // Needs a dummy render primitive otherwise nothing will be rendered.
  bgfx::touch(0);
  bgfx::frame();
}

// Argc, argv and return 0 needed for SDL_main main function linkage.
int main(int argc, char **argv) {
  SDLBackend backend;

  // Calling renderFrame establishes this thread as the render thread. Otherwise
  // it crashes.
  // Comment this line out to use multi threaded mode.
  // bgfx::renderFrame();
  bgfx::Init init;
  init.platformData = backend.platform_data();
  if (!bgfx::init(init)) {
    return 1;
  }
  std::cout << "init done\n"; // never reached in multi threaded mode (when renderFrame commented out)
  bgfx::reset(1280, 720, BGFX_RESET_VSYNC);
  bgfx::setViewRect(0, 0, 0, 1280, 720);
  backend.main_loop(render);

  bgfx::shutdown();
  return 0;
}
