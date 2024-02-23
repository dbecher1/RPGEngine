//
// Created by Daniel Becher on 2/20/24.
//

#include "Camera.h"
#include "../misc/macros.h"

#define ZOOM 3.0f

// TODO: set a "player locked" variable for pans/cutscenes etc
// TODO: look into making viewport an int rect

Camera::Camera(SDL_Renderer* renderer_, int screen_width, int screen_height) :
renderer(renderer_), screenWidth(screen_width), screenHeight(screen_height) {
    viewPort = {
            0,
            0,
            static_cast<float>(static_cast<float>(screen_width) / ZOOM),
            static_cast<float>(static_cast<float>(screen_height) / ZOOM),
    };
}

// For some reason that I still don't know, the backbuffer just -didn't- work right when made in the constructor
void Camera::Init_BackBuffer() {
    backBuffer = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    int access;
    SDL_QueryTexture(backBuffer, nullptr, &access, nullptr, nullptr);
    // std::cout << access << " should be " << SDL_TEXTUREACCESS_TARGET << std::endl;
    if (backBuffer == nullptr) {
        SDL_PRINT_ERR("Couldn't create texture!");
    }
    // std::cerr << backBuffer << std::endl;
}

Camera::~Camera() {
    SDL_DestroyTexture(backBuffer);
}

void Camera::Update(Vector2 position) {
    // TODO: create sub-square where movement doesn't register
    // Also, make camera movement smoother and lag behind character ever so slightly
    viewPort.x = static_cast<float>(std::fmax(position.x - (viewPort.w * 0.5f), 0));
    viewPort.y = static_cast<float>(std::fmax(position.y - (viewPort.h * 0.5f), 0));
}


void Camera::setCamera() {
    if (SDL_SetRenderTarget(renderer, backBuffer) != 0) {
            SDL_PRINT_ERR("Couldn't set render target!");
        }
    SDL_RenderClear(renderer);
}

void Camera::unsetCamera() {
    SDL_Rect vp = convert(viewPort);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, backBuffer, &vp, nullptr);
}

SDL_FRect Camera::getViewport() {
    return viewPort;
}

bool Camera::rectInFrame(SDL_FRect *rect) {
    return SDL_IntersectFRect(reinterpret_cast<const SDL_FRect *>(&viewPort), rect, nullptr) == SDL_TRUE;
}
