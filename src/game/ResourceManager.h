//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_RESOURCEMANAGER_H
#define RPG_RESOURCEMANAGER_H

#include "SDL.h"
#include "../tools/ImagePacker.h"
#include "entities/Entity.h"
#include "gfx/Environment.h"
#include "world/Maps.h"

#include <string>
#include <vector>
#include <map>

class ImagePacker;
class SpriteBatch;
class Maps;

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
    // THIS IS ALSO A TEST
    std::vector<Entity>* getEntities();

    Environment* getEnvironment(const std::string& name);
    Maps* getMap(const std::string& name);

private:

    bool loadTextures(SDL_Renderer* renderer);
    bool loadMaps(SDL_Renderer* renderer);
    bool loadAnimations();
    bool loadEntities();
    bool loadEnvironments();

    // Static helpers for textures and file management
    static void writeTexture(SDL_Texture* texture, SDL_Renderer* renderer, const std::string& name);
    static std::string createLocalFileName(const std::string& name);

    std::map<std::string, SDL_Rect> textureRects;
    std::map<std::string, Environment> environments;
    std::map<std::string, Maps> maps;
    std::vector<Entity> Entities;
    SDL_Texture* atlas;

    std::vector<std::string> TEMP_fileNames; // used to make sure we don't double load tilemap textures

    friend class ImagePacker;
    friend class SpriteBatch;
};


#endif //RPG_RESOURCEMANAGER_H
