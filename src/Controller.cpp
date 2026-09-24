#include <iostream>
#include "Timestamp.hpp"
#include "Controller.hpp"

namespace dlc {
    Controller::Controller() {
        if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        int count;
        SDL_JoystickID *gamepads = SDL_GetGamepads(&count);
        gamepad = nullptr;
        for (int i = count - 1; i >= 0; --i) {
            gamepad = SDL_OpenGamepad(gamepads[i]);
            if (gamepad != nullptr) {
                break;
            }
            if (!SDL_GamepadConnected(gamepad)) {
                SDL_CloseGamepad(gamepad);
                gamepad = nullptr;
                break;
            }
        }
        SDL_free(gamepads);
        if (gamepad == nullptr) {
            Timestamp(std::cerr);
            std::cerr << "Controller not found!" << std::endl;
            return;
        }
    }

    Controller::~Controller() {
        if (gamepad != nullptr) {
            SDL_CloseGamepad(gamepad);
            gamepad = nullptr;
        }
        SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
    }

    bool Controller::IsConnected() const {
        return gamepad != nullptr && SDL_GamepadConnected(gamepad);
    }

    bool Controller::Reconnect() {
        this->~Controller();
        new(this) Controller();
        return IsConnected();
    }

    static void UpdateButton(SDL_Gamepad *gamepad, Controller::Button &button, SDL_GamepadButton type) {
        if (SDL_GetGamepadButton(gamepad, type)) {
            if (button == Controller::Button::UP || button == Controller::Button::RELEASED) {
                button = Controller::Button::DOWN;
            } else {
                button = Controller::Button::HELD;
            }
        } else {
            if (button == Controller::Button::DOWN || button == Controller::Button::HELD) {
                button = Controller::Button::RELEASED;
            } else {
                button = Controller::Button::UP;
            }
        }
    }

    void Controller::Update() {
        if (gamepad == nullptr) {
            return;
        }
        if (!SDL_GamepadConnected(gamepad)) {
            SDL_memset(&buttons, 0, sizeof(buttons));
            SDL_memset(&axis, 0, sizeof(axis));
            return;
        }
        UpdateButton(gamepad, buttons.leftStickPress, SDL_GAMEPAD_BUTTON_LEFT_STICK);
        UpdateButton(gamepad, buttons.rightStickPress, SDL_GAMEPAD_BUTTON_RIGHT_STICK);
        UpdateButton(gamepad, buttons.north, SDL_GAMEPAD_BUTTON_NORTH);
        UpdateButton(gamepad, buttons.east, SDL_GAMEPAD_BUTTON_EAST);
        UpdateButton(gamepad, buttons.south, SDL_GAMEPAD_BUTTON_SOUTH);
        UpdateButton(gamepad, buttons.west, SDL_GAMEPAD_BUTTON_WEST);
        UpdateButton(gamepad, buttons.dpadNorth, SDL_GAMEPAD_BUTTON_DPAD_UP);
        UpdateButton(gamepad, buttons.dpadEast, SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
        UpdateButton(gamepad, buttons.dpadSouth, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
        UpdateButton(gamepad, buttons.dpadWest, SDL_GAMEPAD_BUTTON_DPAD_LEFT);
        UpdateButton(gamepad, buttons.leftBumper, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
        UpdateButton(gamepad, buttons.rightBumper, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
        UpdateButton(gamepad, buttons.select, SDL_GAMEPAD_BUTTON_BACK);
        UpdateButton(gamepad, buttons.start, SDL_GAMEPAD_BUTTON_START);
        UpdateButton(gamepad, buttons.menu, SDL_GAMEPAD_BUTTON_GUIDE);
        float magnitude;
        axis.leftStick.x = static_cast<float>(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX)) /
                           static_cast<float>(SDL_MAX_SINT16);
        axis.leftStick.y = static_cast<float>(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY)) /
                           static_cast<float>(SDL_MIN_SINT16);
        magnitude = Magnitude(axis.leftStick);
        if (magnitude > 1.0f) {
            axis.leftStick.x /= magnitude;
            axis.leftStick.y /= magnitude;
        } else if (magnitude < 0.15f) {
            axis.leftStick.x = 0.0f;
            axis.leftStick.y = 0.0f;
        }
        axis.rightStick.x = static_cast<float>(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX)) /
                            static_cast<float>(SDL_MAX_SINT16);
        axis.rightStick.y = static_cast<float>(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTY)) /
                            static_cast<float>(SDL_MIN_SINT16);
        magnitude = Magnitude(axis.rightStick);
        if (magnitude > 1.0f) {
            axis.rightStick.x /= magnitude;
            axis.rightStick.y /= magnitude;
        } else if (magnitude < 0.15f) {
            axis.rightStick.x = 0.0f;
            axis.rightStick.y = 0.0f;
        }
        axis.leftTrigger = static_cast<float>(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER)) /
                           static_cast<float>(SDL_MAX_SINT16);
        if (axis.leftTrigger < 0.15f) {
            axis.leftTrigger = 0.0f;
        }
        axis.rightTrigger = static_cast<float>(SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)) /
                            static_cast<float>(SDL_MAX_SINT16);
        if (axis.rightTrigger < 0.15f) {
            axis.rightTrigger = 0.0f;
        }
    }

    Controller::Button Controller::GetLeftStickPress() const {
        return buttons.leftStickPress;
    }

    Controller::Button Controller::GetRightStickPress() const {
        return buttons.rightStickPress;
    }

    Controller::Button Controller::GetNorth() const {
        return buttons.north;
    }

    Controller::Button Controller::GetEast() const {
        return buttons.east;
    }

    Controller::Button Controller::GetSouth() const {
        return buttons.south;
    }

    Controller::Button Controller::GetWest() const {
        return buttons.west;
    }

    Controller::Button Controller::GetDpadNorth() const {
        return buttons.dpadNorth;
    }

    Controller::Button Controller::GetDpadEast() const {
        return buttons.dpadEast;
    }

    Controller::Button Controller::GetDpadSouth() const {
        return buttons.dpadSouth;
    }

    Controller::Button Controller::GetDpadWest() const {
        return buttons.dpadWest;
    }

    Controller::Button Controller::GetLeftBumper() const {
        return buttons.leftBumper;
    }

    Controller::Button Controller::GetRightBumper() const {
        return buttons.rightBumper;
    }

    Controller::Button Controller::GetSelect() const {
        return buttons.select;
    }

    Controller::Button Controller::GetStart() const {
        return buttons.start;
    }

    Controller::Button Controller::GetMenu() const {
        return buttons.menu;
    }

    SDL_FPoint Controller::GetLeftStick() const {
        return axis.leftStick;
    }

    SDL_FPoint Controller::GetRightStick() const {
        return axis.rightStick;
    }

    float Controller::GetLeftTrigger() const {
        return axis.leftTrigger;
    }

    float Controller::GetRightTrigger() const {
        return axis.rightTrigger;
    }

    float SqrMagnitude(const SDL_FPoint &point) {
        return point.x * point.x + point.y * point.y;
    }

    float Magnitude(const SDL_FPoint &point) {
        return SDL_sqrtf(SqrMagnitude(point));
    }

    void Normalize(SDL_FPoint &point) {
        float magnitude = Magnitude(point);
        if (magnitude == 0.0f) {
            return;
        }
        point.x /= magnitude;
        point.y /= magnitude;
    }
}
