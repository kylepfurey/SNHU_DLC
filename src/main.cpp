#include <iostream>
#include <Timestamp.hpp>
#include <Window.hpp>
#include <Camera.hpp>
#include <Controller.hpp>
#include <Drone.hpp>

using namespace std;
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
    Controller controller;
    bool running = true;
    while (running) {
        while (window.UpdateEvent()) {
            if (window.ShouldClose()) {
                Timestamp(cout);
                cout << "Quitting." << endl;
                running = false;
            } else if (window.GetEvent().type == SDL_EVENT_KEY_DOWN) {
                switch (window.GetEvent().key.key) {
                    default:
                        break;
                    // Escape - Quit the program
                    case SDLK_ESCAPE:
                        Timestamp(cout);
                        cout << "Quitting." << endl;
                        running = false;
                        break;
                    // 1 - Refresh the camera
                    case SDLK_1:
                        Timestamp(cout);
                        cout << "Refreshing camera." << endl;
                        camera.~Camera();
                        new(&camera) Camera();
                        break;
                    // 2 - Refresh the controller
                    case SDLK_2:
                        Timestamp(cout);
                        cout << "Refreshing controller." << endl;
                        controller.~Controller();
                        new(&controller) Controller();
                        break;
                }
            }
        }
        camera.Refresh(window.GetRenderer());
        window.Render(camera.GetTexture());
    }
    return 0;
}
