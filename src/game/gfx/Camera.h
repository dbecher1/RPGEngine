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
    Camera(SDL_Renderer* renderer_, SDL_Point window, SDL_Point screen);
    ~Camera();

    void Init_BackBuffer();

    void Update(Vector2 position);
    void setCurrentWorldDimensions(int w, int h);

    void setCamera() const;
    void unsetCamera() const;

    bool rectInFrame(const SDL_FRect* rect) const;
    [[nodiscard]] SDL_FRect getViewport() const;
    [[nodiscard]] SDL_FRect getSubRect() const;
private:
    SDL_FPoint last_pos{};
    SDL_FRect viewPort{};
    SDL_FRect subRect{};
    SDL_Texture* backBuffer = nullptr;
    SDL_Renderer* renderer{};
    int screenWidth{};
    int screenHeight{};
    int windowWidth{};
    int windowHeight{};
    int worldWidth{};
    int worldHeight{};
};


#endif //RPG_CAMERA_H
