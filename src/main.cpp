#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

#include <Timestamp.hpp>
#include <Config.hpp>
#include <Window.hpp>
#include <Text.hpp>
#include <Controller.hpp>
#include <Camera.hpp>
#include <Drone.hpp>

using namespace std;
using namespace chrono;
using namespace dlc;

//
// main()'s job is to simply initialize our classes and connect them.
// Here is where we will initialize our two threads for input and video feed.
// The input thread will poll gamepad input and issue commands to the drone.
// The video feed thread will pipe data from the camera directly to a window->
//
int main() {
    // Create thread objects
    Config CONFIG;                                  // INPUT THREAD
    ReloadConfig(CONFIG);
    auto window = make_shared<Window>();            // BOTH THREADS
    if (!window->IsValid()) {
        return -1;
    }
    Text text("Calibri.ttf");                       // VIDEO THREAD
    if (!text.IsValid()) {
        return -1;
    }
    auto controller = make_shared<Controller>();    // BOTH THREADS
    auto camera = make_shared<Camera>();            // BOTH THREADS
    auto drone = make_shared<Drone>();              // BOTH THREADS
    atomic<bool> running = true;                    // BOTH THREADS
    auto time = steady_clock::now();                // INPUT THREAD
    atomic<double> deltaTime = 1.0;                 // BOTH THREADS

    // Create threads
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
                            if (!camera->IsConnected()) {
                                Timestamp(cout);
                                cout << "Reconnecting camera." << endl;
                                camera->Reconnect();
                            }
                            break;
                        // 2 - Refresh the controller
                        case SDLK_2:
                            if (!controller->IsConnected()) {
                                Timestamp(cout);
                                cout << "Reconnecting controller." << endl;
                                controller->Reconnect();
                            }
                            break;
                        // 0 - Refresh the config file
                        case SDLK_0:
                            Timestamp(cout);
                            cout << "Reloading Config.json." << endl;
                            ReloadConfig(CONFIG);
                            break;
                    }
                }
            }

            // Update input
            controller->Update();

            // Update delta time
            auto current = steady_clock::now();
            deltaTime = duration_cast<duration<double> >(current - time).count();
            time = current;
        }
    };
    auto updateVideo = [&] {
        while (running) {
            // Clear screen
            window->ClearScreen();

            // Render video feed
            camera->Render(window->GetRenderer());

            // Display title text
            text << "SNHU DLC";
            text.Render(
                window->GetRenderer(),
                {0.5f, 0.0f},
                {255, 255, 255, 255},
                {0.5f, 0.0f},
                {0, 0, 0, 255}
            );

            // Display controller status
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

            // Display camera status
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

            // Display stick inputs
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

            // Display framerate
            text << static_cast<int>(1.0 / deltaTime) << " FPS";
            text.Render(
                window->GetRenderer(),
                {0.0f, 0.0f},
                {255, 255, 255, 255},
                {0.0f, 0.0f},
                {0, 0, 0, 255}
            );

            // Render screen
            window->UpdateScreen();
        }
    };

    // Run threads
    Timestamp(cout);
    cout << "Starting threads." << endl;
    thread inputThread(updateInput);
    updateVideo();
    inputThread.join();

    return 0;
}
