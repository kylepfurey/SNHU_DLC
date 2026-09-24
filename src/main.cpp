#include <Window.hpp>
#include <Camera.hpp>
#include <Controller.hpp>
#include <Drone.hpp>

using namespace dlc;

//
// main()'s job is to simply initialize our classes and connect them.
// Here is where we will initialize our two threads for input and video feed.
// The input thread will poll gamepad input and issue commands to the drone.
// The video feed thread will pipe data from the camera directly to a window.
//
int main() {
    Window window;
    Camera camera;
    bool running = true;
    while (running) {
        while (window.UpdateEvent()) {
            if (window.ShouldClose() ||
                (window.GetEvent().type == SDL_EVENT_KEY_DOWN &&
                 window.GetEvent().key.key == SDLK_ESCAPE)) {
                running = false;
            }
        }
        camera.Refresh(window.GetRenderer());
        window.Render(camera.GetTexture());
    }
    return 0;
}
