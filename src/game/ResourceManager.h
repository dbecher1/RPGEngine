//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_RESOURCEMANAGER_H
#define RPG_RESOURCEMANAGER_H

#include "SDL.h"
#include "../tools/ImagePacker.h"
#include "gfx/animations/AnimationStateMachine.h"

#include <string>
#include <map>

class ImagePacker;
class SpriteBatch;

class ResourceManager {
public:
    ~ResourceManager();
    /**
     * Loads all resources in the game. This includes all textures, audio, level data, etc.
     * In dev, this will include image packing.
     * @param renderer A pointer to the renderer used to load textures.
     * @return True upon success, else false
     */
    bool loadAllResources(SDL_Renderer* renderer);

    SDL_Rect getRectFromTextureName(const std::string& fileName);

    // THIS IS A TEST METHOD
    AnimationStateMachine asmLoader();

private:

    bool loadTextures(SDL_Renderer* renderer);
    bool loadAnimations();

    std::map<std::string, SDL_Rect> textureRects;
    SDL_Texture* atlas;

    friend class ImagePacker;
    friend class SpriteBatch;
};


#endif //RPG_RESOURCEMANAGER_H
