//
// Created by Daniel Becher on 2/20/24.
//

#ifndef RPG_CAMERA_H
#define RPG_CAMERA_H

#include <SDL_render.h>
#include "SDL_rect.h"
#include "../../math/Vector2.h"

class SpriteBatch;

class Camera {
public:
    Camera() = default;
    Camera(SDL_Renderer* renderer_, int screen_width, int screen_height);
    ~Camera();

    void Update(Vector2 position);

    void setCamera();
    void unsetCamera();

    SDL_Rect getViewport();
private:
    SDL_Rect viewPort;
    SDL_Texture* backBuffer;
    SDL_Renderer* renderer;
};


#endif //RPG_CAMERA_H
