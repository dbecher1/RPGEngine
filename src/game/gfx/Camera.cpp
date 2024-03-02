//
// Created by Daniel Becher on 2/20/24.
//

#include "Camera.h"
#include "../misc/macros.h"

#define ZOOM 3.0f

// TODO: set a "player locked" variable for pans/cutscenes etc
// TODO: look into making viewport an int rect

Camera::Camera(SDL_Renderer* renderer_, SDL_Point window, SDL_Point screen) :
renderer(renderer_), screenWidth(screen.x), screenHeight(screen.y),
windowWidth(window.x), windowHeight(window.y) {
    viewPort = {
            0,
            0,
            static_cast<float>(screen.x) / ZOOM,
            static_cast<float>(screen.y) / ZOOM,
    };

    subRect = {
            static_cast<float>(windowWidth) * 0.15f,
            static_cast<float>(windowHeight) * 0.15f,
            static_cast<float>(windowWidth) * 0.7f,
            static_cast<float>(windowHeight) * 0.7f
    };
}

// For some reason that I still don't know, the backbuffer just -didn't- work right when made in the constructor
void Camera::Init_BackBuffer() {
    backBuffer = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    int access;
    SDL_QueryTexture(backBuffer, nullptr, &access, nullptr, nullptr);
    if (backBuffer == nullptr) {
        SDL_PRINT_ERR("Couldn't create texture!");
    }
}

Camera::~Camera() {
    SDL_DestroyTexture(backBuffer);
}

void Camera::Update(Vector2 position) {
    // TODO: create sub-square where movement doesn't register
    // Also, make camera movement smoother and lag behind character ever so slightly

    viewPort.x = (position.x - (viewPort.w * 0.5f));
    viewPort.y = (position.y - (viewPort.h * 0.5f));

    // Camera bounds

    if (viewPort.x < 0)
        viewPort.x = 0;
    if (viewPort.y < 0)
        viewPort.y = 0;

    if (worldWidth > 0) {
        if (viewPort.x + viewPort.w > static_cast<float>(worldWidth)) {
            viewPort.x = static_cast<float>(worldWidth) - viewPort.w;
        }
    }
    if (worldHeight > 0) {
        if (viewPort.y + viewPort.h > static_cast<float>(worldHeight)) {
            viewPort.y = static_cast<float>(worldHeight) - viewPort.h;
        }
    }
}


void Camera::setCamera() const {
    if (SDL_SetRenderTarget(renderer, backBuffer) != 0)
            SDL_PRINT_ERR("Couldn't set render target!");
    SDL_RenderClear(renderer);
}

void Camera::unsetCamera() const {
    const SDL_Rect vp = convert(viewPort);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, backBuffer, &vp, nullptr);
}

SDL_FRect Camera::getViewport() const {
    return viewPort;
}

bool Camera::rectInFrame(const SDL_FRect *rect) const {
    return SDL_IntersectFRect(&viewPort, rect, nullptr) == SDL_TRUE;
}

void Camera::setCurrentWorldDimensions(int w, int h) {
    worldWidth = w;
    worldHeight = h;
}

SDL_FRect Camera::getSubRect() const {
    return subRect;
}
