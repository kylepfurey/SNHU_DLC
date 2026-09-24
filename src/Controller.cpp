#include <iostream>
#include "Timestamp.hpp"
#include "Controller.hpp"

namespace dlc {
    Controller::Controller() {
        if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        int count;
        SDL_JoystickID *joysticks = SDL_GetJoysticks(&count);
        gamepad = nullptr;
        for (int i = count - 1; i >= 0; --i) {
            if (SDL_IsGamepad(joysticks[i])) {
                gamepad = SDL_OpenGamepad(joysticks[i]);
                if (gamepad != nullptr) {
                    break;
                }
            }
        }
        SDL_free(joysticks);
        if (gamepad == nullptr) {
            Timestamp(std::cerr);
            std::cerr << "Controller not found!" << std::endl;
            return;
        }
    }

    Controller::~Controller() {
        SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
    }
}
