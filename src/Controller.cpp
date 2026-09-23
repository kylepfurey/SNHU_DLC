#include <iostream>
#include "Controller.hpp"

namespace dlc {
    Controller::Controller() {
        if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
            std::cout << SDL_GetError() << std::endl;
        }
    }

    Controller::~Controller() {
        SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
    }
}
