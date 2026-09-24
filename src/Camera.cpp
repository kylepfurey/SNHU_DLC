#include <iostream>
#include "Timestamp.hpp"
#include "Camera.hpp"

namespace dlc {
    Camera::Camera() {
        if (!SDL_InitSubSystem(SDL_INIT_CAMERA)) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        int count = 0;
        SDL_CameraID *cameras = SDL_GetCameras(&count);
        if (cameras == nullptr || count == 0) {
            Timestamp(std::cerr);
            std::cerr << "Camera not found!" << std::endl;
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
        SDL_CameraID cameraID = cameras[count - 1];
        SDL_free(cameras);
        camera = SDL_OpenCamera(cameraID, nullptr);
        if (camera == nullptr) {
            Timestamp(std::cerr);
            std::cerr << SDL_GetError() << std::endl;
            return;
        }
    }

    Camera::~Camera() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        if (camera != nullptr) {
            SDL_CloseCamera(camera);
            camera = nullptr;
        }
        SDL_QuitSubSystem(SDL_INIT_CAMERA);
    }

    void Camera::Refresh(SDL_Renderer *renderer) {
        if (renderer == nullptr || camera == nullptr) {
            return;
        }
        Uint64 timestamp = 0;
        SDL_Surface *frame = SDL_AcquireCameraFrame(camera, &timestamp);
        if (frame == nullptr) {
            return;
        }
        if (texture == nullptr || texture->w != frame->w || texture->h != frame->h) {
            if (texture != nullptr) {
                SDL_DestroyTexture(texture);
            }
            texture = SDL_CreateTexture(
                renderer,
                frame->format,
                SDL_TEXTUREACCESS_STREAMING,
                frame->w,
                frame->h
            );
        }
        SDL_UpdateTexture(texture, nullptr, frame->pixels, frame->pitch);
        SDL_ReleaseCameraFrame(camera, frame);
    }

    SDL_Texture *Camera::GetTexture() {
        return texture;
    }

    const SDL_Texture *Camera::GetTexture() const {
        return texture;
    }
}
