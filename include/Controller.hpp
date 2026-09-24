#pragma once
#include <SDL3/SDL.h>

namespace dlc {
    //
    // The Controller class is just an abstraction for any gamepad connected.
    // We want to expose methods that represent all possible user inputs.
    // We can use these methods to directly control the Drone class later.
    //
    class Controller final {
    public:
        Controller();
        ~Controller();

        // Unsafe to copy
        Controller(const Controller &) = delete;
        Controller &operator=(const Controller &) = delete;

        // Unsafe to move
        Controller(Controller &&) = delete;
        Controller &operator=(Controller &&) = delete;

    private:
        SDL_Gamepad *gamepad = nullptr;
    };
}
