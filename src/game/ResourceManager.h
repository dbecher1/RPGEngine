//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_RESOURCEMANAGER_H
#define RPG_RESOURCEMANAGER_H

#include "SDL.h"
#include "../tools/ImagePacker.h"
#include "gfx/Environment.h"
#include "world/Maps.h"
#include "entities/GlobalEntity.h"

#include <string>
#include <vector>
#include <unordered_map>

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

    [[nodiscard]] SDL_Rect getRectFromTextureName(const std::string& fileName) const;
    [[nodiscard]] SDL_Texture* getAtlas() const;

    // TODO get rid of this eventually
    std::unordered_map<std::string, GlobalEntity>* getEntities();

    Environment* getEnvironment(const std::string& name);
    Maps* getMap(const std::string& name);
    GlobalEntity* getEntity(const std::string& name);

    //static const std::string FONT_CHARACTERS;

private:

    bool loadTextures(SDL_Renderer* renderer);
    bool loadMaps(SDL_Renderer* renderer);
    bool loadFonts(SDL_Renderer *renderer);
    bool loadAnimations();
    bool loadEntities();
    bool loadEnvironments();

    // Static helpers for textures and file management
    static void writeTexture(SDL_Texture* texture, SDL_Renderer* renderer, const std::string& name);
    static std::string createLocalFileName(const std::string& name);

    std::unordered_map<std::string, SDL_Rect> textureRects;
    std::unordered_map<std::string, Environment> environments;
    std::unordered_map<std::string, Maps> maps;
    std::unordered_map<std::string, GlobalEntity> Entities;
    SDL_Texture* atlas;

    std::vector<std::string> TEMP_fileNames; // used to make sure we don't double load tilemap textures

    friend class ImagePacker;
    friend class SpriteBatch;
};


#endif //RPG_RESOURCEMANAGER_H
