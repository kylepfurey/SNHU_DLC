#include <iostream>
#include "Window.hpp"

namespace dlc {
    Window::Window() {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            std::cout << SDL_GetError() << std::endl;
        }
    }

    Window::~Window() {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}
