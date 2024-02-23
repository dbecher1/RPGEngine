//
// Created by Daniel Becher on 2/20/24.
//

#ifndef RPG_CAMERA_H
#define RPG_CAMERA_H

#include "SDL_render.h"
#include "SDL_rect.h"
#include "../../math/Vector2.h"

inline SDL_Rect convert(const SDL_FRect& r) {
    return {
        static_cast<int>(r.x),
        static_cast<int>(r.y),
        static_cast<int>(r.w),
        static_cast<int>(r.h)
    };
};

inline SDL_FRect convert(const SDL_Rect& r) {
    return {
            static_cast<float>(r.x),
            static_cast<float>(r.y),
            static_cast<float>(r.w),
            static_cast<float>(r.h)
    };
};

class SpriteBatch;

class Camera {
public:
    Camera() = default;
    Camera(SDL_Renderer* renderer_, int screen_width, int screen_height);
    ~Camera();

    void Init_BackBuffer();

    void Update(Vector2 position);

    void setCamera();
    void unsetCamera();

    bool rectInFrame(SDL_FRect* rect);
    SDL_FRect getViewport();
private:
    SDL_FRect viewPort{};
    SDL_Texture* backBuffer = nullptr;
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;
};


#endif //RPG_CAMERA_H
