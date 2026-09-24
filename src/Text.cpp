#include <iostream>
#include "Timestamp.hpp"
#include "Text.hpp"

namespace dlc {
    Text::Text(const char fontFile[], float fontSize) {
        if (!TTF_Init()) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        font = TTF_OpenFont(fontFile, fontSize);
        if (font == nullptr) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
    }

    Text::~Text() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        if (font != nullptr) {
            TTF_CloseFont(font);
            font = nullptr;
        }
        TTF_Quit();
    }

    bool Text::IsValid() const {
        return font != nullptr;
    }

    bool Text::Rebuild(const char fontFile[], float fontSize) {
        this->~Text();
        new(this) Text(fontFile, fontSize);
        return IsValid();
    }

    void Text::Render(
        SDL_Renderer *renderer,
        SDL_FPoint screenPosition,
        SDL_FPoint textPivot,
        SDL_Color textColor,
        SDL_Color backgroundColor
    ) {
        if (renderer == nullptr || font == nullptr) {
            return;
        }
        SDL_Surface *surface = TTF_RenderText_Blended(
            font,
            text.str().c_str(),
            0,
            textColor
        );
        if (surface == nullptr) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        if (texture == nullptr || texture->w != surface->w || texture->h != surface->h) {
            if (texture != nullptr) {
                SDL_DestroyTexture(texture);
            }
            texture = SDL_CreateTexture(
                renderer,
                surface->format,
                SDL_TEXTUREACCESS_STREAMING,
                surface->w,
                surface->h
            );
            if (texture == nullptr) {
                SDL_DestroySurface(surface);
                Timestamp(std::cerr);
                std::cerr << SDL_GetError() << std::endl;
                return;
            }
        }
        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        SDL_DestroySurface(surface);
        int width, height;
        if (!SDL_GetRenderOutputSize(renderer, &width, &height)) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        float scaleX = static_cast<float>(width) / 1920.0f;
        float scaleY = static_cast<float>(height) / 1080.0f;
        SDL_FRect destination{
            screenPosition.x * 1920.0f * scaleX -
            textPivot.x * static_cast<float>(texture->w) * scaleX,
            screenPosition.y * 1080.0f * scaleY -
            textPivot.y * static_cast<float>(texture->h) * scaleY,
            static_cast<float>(texture->w) * scaleX,
            static_cast<float>(texture->h) * scaleY
        };
        if (backgroundColor.a > 0) {
            SDL_SetRenderDrawColor(
                renderer,
                backgroundColor.r,
                backgroundColor.g,
                backgroundColor.b,
                backgroundColor.a
            );
            SDL_RenderFillRect(renderer, &destination);
        }
        SDL_RenderTexture(renderer, texture, nullptr, &destination);
        text.str("");
        text.clear();
    }
}
