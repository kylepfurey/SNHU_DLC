#pragma once
#include <sstream>
#include <SDL3_ttf/SDL_ttf.h>

namespace dlc {
    //
    // The job of the Text class is to render text to the screen.
    // Like std::cout, we want to << displayed data and render it each frame.
    //
    class Text final {
    public:
        Text(const char fontFile[], float fontSize = 64.0f);
        ~Text();

        // Unsafe to copy
        Text(const Text &) = delete;
        Text &operator=(const Text &) = delete;

        // Unsafe to move
        Text(Text &&) = delete;
        Text &operator=(Text &&) = delete;

        bool IsValid() const;
        bool Rebuild(const char fontFile[], float fontSize = 64.0f);
        void Render(
            SDL_Renderer *renderer,
            SDL_FPoint screenPosition,
            SDL_FPoint textPivot,
            SDL_Color textColor,
            SDL_Color backgroundColor = {}
        );

        // Stream operator used for building text
        template<typename T>
        Text &operator<<(const T &obj) {
            text << obj;
            return *this;
        }

    private:
        std::stringstream text{};
        TTF_Font *font = nullptr;
        SDL_Texture *texture = nullptr;
    };
}
