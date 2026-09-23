#pragma once
#include <SDL3/SDL.h>

namespace dlc {
    //
    // The Camera class will abstract streaming from the camera feed.
    // We can connect this to the Window class to render video.
    //
    class Camera final {
    public:
        Camera();
        ~Camera();

        // Unsafe to copy
        Camera(const Camera &) = delete;
        Camera &operator=(const Camera &) = delete;

        // Unsafe to move
        Camera(Camera &&) = delete;
        Camera &operator=(Camera &&) = delete;

        // Methods here
    private:
        // Members here
    };
}
