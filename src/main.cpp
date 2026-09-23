#include <Window.hpp>
#include <Controller.hpp>
#include <Drone.hpp>
#include <Camera.hpp>

using namespace dlc;

//
// main()'s job is to simply initialize our classes and connect them.
// Here is where we will initialize our two threads for input and video feed.
// The input thread will poll gamepad input and issue commands to the drone.
// The video feed thread will pipe data from the camera directly to a window.
//
int main() {
    Controller controller;
    Drone drone;
    Window window;
    Camera camera;
    // Driver logic here
    return 0;
}
