#include <iostream>
#include "Camera.hpp"

namespace dlc {
    Camera::Camera() {
        if (!SDL_InitSubSystem(SDL_INIT_CAMERA)) {
            std::cout << SDL_GetError() << std::endl;
        }
    }

    Camera::~Camera() {
        SDL_QuitSubSystem(SDL_INIT_CAMERA);
    }
}
