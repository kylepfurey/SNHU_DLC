#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <Timestamp.hpp>
#include <Config.hpp>
#include <Window.hpp>
#include <Text.hpp>
#include <Controller.hpp>
#include <Camera.hpp>
#include <Drone.hpp>

using namespace std;
using namespace dlc;

//
// main()'s job is to simply initialize our classes and connect them.
// Here is where we will initialize our two threads for input and video feed.
// The input thread will poll gamepad input and issue commands to the drone.
// The video feed thread will pipe data from the camera directly to a window->
//
int main() {
    // Create objects
    Config CONFIG;                           // INPUT THREAD
    ReloadConfig(CONFIG);
    auto window = std::make_shared<Window>();     // BOTH THREADS
    if (!window->IsValid()) {
        return -1;
    }
    Text text("Calibri.ttf");         // VIDEO THREAD
    if (!text.IsValid()) {
        return -1;
    }
    auto controller = std::make_shared<Controller>(); // BOTH THREADS
    auto camera = std::make_shared<Camera>();     // BOTH THREADS
    Timestamp(cout);
    cout << "Starting." << endl;

    // Create threads
    atomic<bool> running = true;
    auto updateInput = [&] {
        while (running) {
            // Handle events
            while (window->UpdateEvent()) {
                if (window->ShouldClose()) {
                    Timestamp(cout);
                    cout << "Quitting." << endl;
                    running = false;
                } else if (window->GetEvent().type == SDL_EVENT_KEY_DOWN) {
                    switch (window->GetEvent().key.key) {
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
                            cout << "Reconnecting camera." << endl;
                            camera->Reconnect();
                            break;
                        // 2 - Refresh the controller
                        case SDLK_2:
                            Timestamp(cout);
                            cout << "Reconnecting controller." << endl;
                            controller->Reconnect();
                            break;
                        // 0 - Refresh the config file
                        case SDLK_0:
                            Timestamp(cout);
                            cout << "Reloading Config.json." << endl;
                            ReloadConfig(CONFIG);
                            break;
                    }
                } else if ((window->GetEvent().type == SDL_EVENT_CAMERA_DEVICE_ADDED ||
                            window->GetEvent().type == SDL_EVENT_CAMERA_DEVICE_APPROVED)) {
                    // Reconnect camera
                    if (!camera->IsConnected()) {
                        Timestamp(cout);
                        cout << "Reconnecting camera." << endl;
                        camera->Reconnect();
                    }
                } else if (window->GetEvent().type == SDL_EVENT_GAMEPAD_ADDED) {
                    // Reconnect controller
                    if (!controller->IsConnected()) {
                        Timestamp(cout);
                        cout << "Reconnecting controller." << endl;
                        controller->Reconnect();
                    }
                }
            }
            // Update input
            controller->Update();
        }
    };
    auto updateVideo = [&] {
        while (running) {
            // Clear screen
            window->ClearScreen();
            // Video feed
            camera->Render(window->GetRenderer());
            // Title text
            text << "SNHU DLC";
            text.Render(
                window->GetRenderer(),
                {0.5f, 0.0f},
                {255, 255, 255, 255},
                {0.5f, 0.0f},
                {0, 0, 0, 255}
            );
            // Controller status
            bool controllerConnected = controller->IsConnected();
            text << "Controller";
            text.Render(
                window->GetRenderer(),
                {0.0f, 0.5f},
                {
                    static_cast<Uint8>(controllerConnected ? 0 : 255),
                    static_cast<Uint8>(controllerConnected ? 255 : 0),
                    0,
                    255
                },
                {0.0f, 0.0f},
                {0, 0, 0, 255}
            );
            // Camera status
            bool cameraConnected = camera->IsConnected();
            text << "Camera";
            text.Render(
                window->GetRenderer(),
                {0.0f, 0.5f},
                {
                    static_cast<Uint8>(cameraConnected ? 0 : 255),
                    static_cast<Uint8>(cameraConnected ? 255 : 0),
                    0,
                    255
                },
                {0.0f, 1.0f},
                {0, 0, 0, 255}
            );
            // Stick inputs
            SDL_FPoint leftStick = controller->GetLeftStick();
            text << '{' << leftStick.x << ", " << leftStick.y << "} LS";
            text.Render(
                window->GetRenderer(),
                {1.0f, 0.5f},
                {0, 0, 255, 255},
                {1.0f, 1.0f},
                {0, 0, 0, 255}
            );
            SDL_FPoint rightStick = controller->GetRightStick();
            text << '{' << rightStick.x << ", " << rightStick.y << "} RS";
            text.Render(
                window->GetRenderer(),
                {1.0f, 0.5f},
                {255, 255, 0, 255},
                {1.0f, 0.0f},
                {0, 0, 0, 255}
            );
            // Render
            window->UpdateScreen();
        }
    };

    // Run threads
    thread inputThread(updateInput);
    updateVideo();
    inputThread.join();

    return 0;
}
