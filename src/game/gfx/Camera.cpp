//
// Created by Daniel Becher on 2/20/24.
//

#include "Camera.h"
#include "../misc/macros.h"

#define ZOOM 0.5f

Camera::Camera(SDL_Renderer* renderer_, int screen_width, int screen_height) :
renderer(renderer_) {
    viewPort = {
            0,
            0,
            static_cast<float>(static_cast<float>(screen_width) / ZOOM),
            static_cast<float>(static_cast<float>(screen_height) / ZOOM),
    };
    backBuffer = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, static_cast<int>(viewPort.w), static_cast<int>(viewPort.h));
}

Camera::~Camera() {
    SDL_DestroyTexture(backBuffer);
}

void Camera::Update(Vector2 position) {
    // TODO
}

void Camera::setCamera() {
    SDL_SetRenderTarget(renderer, backBuffer);
    SDL_RenderClear(renderer);
}

void Camera::unsetCamera() {
    SDL_SetRenderTarget(renderer, nullptr);
    //SDL_RenderCopy(renderer, backBuffer, nullptr, nullptr);
    SDL_RenderCopyF(renderer, backBuffer, nullptr, &viewPort);
}

SDL_FRect Camera::getViewport() {
    return viewPort;
}
