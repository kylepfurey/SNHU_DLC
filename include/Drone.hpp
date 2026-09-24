#pragma once
#include <mavsdk/mavsdk.hpp>
#include <mavsdk/plugins/action/action.hpp>
#include <mavsdk/plugins/telemetry/telemetry.hpp>

namespace dlc {
    //
    // The Drone class will have the sole responsibility of talking to the drone.
    // Here we want to abstract MAVSDK into methods that represent our needs.
    // We can pipe input from the Controller class into this to give direct control.
    //
    class Drone final {
    public:
        Drone();
        ~Drone();

        // Unsafe to copy
        Drone(const Drone &) = delete;
        Drone &operator=(const Drone &) = delete;

        // Unsafe to move
        Drone(Drone &&) = delete;
        Drone &operator=(Drone &&) = delete;

    private:
        mavsdk::Mavsdk mavsdk{
            mavsdk::Mavsdk::Configuration{
                mavsdk::ComponentType::GroundStation
            }
        };
    };
}
