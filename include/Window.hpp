#pragma once
#include <SDL3/SDL.h>

namespace dlc {
    //
    // The Window class is simply a window for the application.
    // We can use this to render the video feed to the screen.
    //
    class Window final {
    public:
        Window();
        ~Window();

        // Unsafe to copy
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        // Unsafe to move
        Window(Window &&) = delete;
        Window &operator=(Window &&) = delete;

        bool IsValid() const;
        bool Rebuild();
        bool UpdateEvent();
        bool ShouldClose() const;
        SDL_Event &GetEvent();
        const SDL_Event &GetEvent() const;
        SDL_Renderer *GetRenderer();
        const SDL_Renderer *GetRenderer() const;
        void UpdateScreen() const;

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Event event{};
    };
}
