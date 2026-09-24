#include <iostream>
#include "Timestamp.hpp"
#include "Window.hpp"

namespace dlc {
    Window::Window() {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        event.type = SDL_EVENT_QUIT;
        window = SDL_CreateWindow(
            "SNHU DLC",
            1920,
            1080,
            SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
        );
        if (window == nullptr) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        renderer = SDL_CreateRenderer(window, nullptr);
        if (renderer == nullptr) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    Window::~Window() {
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
        if (window != nullptr) {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    bool Window::IsValid() const {
        return window != nullptr && renderer != nullptr;
    }

    bool Window::Rebuild() {
        this->~Window();
        new(this) Window();
        return IsValid();
    }

    bool Window::UpdateEvent() {
        if (window == nullptr) {
            return false;
        }
        return SDL_PollEvent(&event);
    }

    bool Window::ShouldClose() const {
        return event.type == SDL_EVENT_QUIT;
    }

    SDL_Event &Window::GetEvent() {
        return event;
    }

    const SDL_Event &Window::GetEvent() const {
        return event;
    }

    void Window::ClearScreen() const {
        if (renderer == nullptr) {
            return;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Renderer *Window::GetRenderer() {
        return renderer;
    }

    const SDL_Renderer *Window::GetRenderer() const {
        return renderer;
    }

    void Window::UpdateScreen() const {
        if (renderer == nullptr) {
            return;
        }
        SDL_RenderPresent(renderer);
    }
}
