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
        // Possible controller button states
        enum class Button : signed char {
            UP = 0,
            DOWN = 1,
            HELD = 2,
            RELEASED = -1,
        };

        Controller();
        ~Controller();

        // Unsafe to copy
        Controller(const Controller &) = delete;
        Controller &operator=(const Controller &) = delete;

        // Unsafe to move
        Controller(Controller &&) = delete;
        Controller &operator=(Controller &&) = delete;

        bool IsConnected() const;
        bool Reconnect();
        void Update();
        Button GetLeftStickPress() const;
        Button GetRightStickPress() const;
        Button GetNorth() const;
        Button GetEast() const;
        Button GetSouth() const;
        Button GetWest() const;
        Button GetDpadNorth() const;
        Button GetDpadEast() const;
        Button GetDpadSouth() const;
        Button GetDpadWest() const;
        Button GetLeftBumper() const;
        Button GetRightBumper() const;
        Button GetSelect() const;
        Button GetStart() const;
        Button GetMenu() const;
        SDL_FPoint GetLeftStick() const;
        SDL_FPoint GetRightStick() const;
        float GetLeftTrigger() const;
        float GetRightTrigger() const;

    private:
        struct {
            Button leftStickPress;
            Button rightStickPress;
            Button north;
            Button east;
            Button south;
            Button west;
            Button dpadNorth;
            Button dpadEast;
            Button dpadSouth;
            Button dpadWest;
            Button leftBumper;
            Button rightBumper;
            Button select;
            Button start;
            Button menu;
        } buttons{};
        struct {
            SDL_FPoint leftStick;
            SDL_FPoint rightStick;
            float leftTrigger;
            float rightTrigger;
        } axis{};
        SDL_Gamepad *gamepad = nullptr;
    };

    float SqrMagnitude(const SDL_FPoint &point);
    float Magnitude(const SDL_FPoint &point);
    void Normalize(SDL_FPoint &point);
}
