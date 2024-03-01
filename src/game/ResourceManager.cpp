//
// Created by Daniel Becher on 2/7/24.
//

#include "ResourceManager.h"
#include "../tools/extern/json.hpp"
#include "../tools/MapLoader.hpp"
#include "world/Maps.h"
#include "EntityManager.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "battle/ElementalAffinity.h"

/// TODO: probably set an upper limit on texture packing and create a way for a multi-level hierarchy of images
// TODO: Create an Entity manifest, create AnimationStateMachine loader from this

#define TEST_PATH "../resources/images/"
#define IMAGE_PATH "../resources/images/"
#define ANIMATION_PATH "../resources/animations/"
#define ENTITY_PATH "../resources/entities/"
#define ENVIRONMENT_PATH "../resources/misc/environments.json"
#define MAP_PATH "../resources/maps/"

using json = nlohmann::json;

ResourceManager::~ResourceManager() {
    // ResourceManager has ownership over its resources, so it needs to free the SDL texture
    SDL_DestroyTexture(atlas);
}

bool ResourceManager::loadAllResources(SDL_Renderer* renderer) {
    bool flag = this->loadMaps(renderer);
    flag = flag && this->loadTextures(renderer);
    if (DEBUG_PRINT) {
        std::cout << "\n===TEXTURE RECT INFORMATION===" << std::endl;
        for (const auto &r : this->textureRects) {
            std::cout << r.first << " - ";
            std::cout << "x: " << r.second.x << ", ";
            std::cout << "y: " << r.second.y << ", ";
            std::cout << "w: " << r.second.w << ", ";
            std::cout << "h: " << r.second.h << std::endl;
        }
        std::cout << "===END TEXTURE RECT INFORMATION===\n" << std::endl;
    }
    flag = flag && this->loadEntities() && this->loadEnvironments();
    return flag;
}

/**
 * Loads map data from TMX files.
 * This should be called before image loading (in dev) as it generates new image data for the texture loader.
 * @return True if successful
 */
bool ResourceManager::loadMaps(SDL_Renderer* renderer) {
    // TODO: add in a check to only run the generation if the files have been updated
    for (const auto &p : iter_dir(MAP_PATH)) {

        if (p.is_directory()) {

            std::string mapName = p.path().filename().string();
            MapBuilder mapBuilder{
                .map_name = mapName,
            };

            // this allocates on the heap, needs to be deleted at end of loop iteration
            auto dataRaw = MapLoader::loadRawMapData(p.path(), renderer);

            if (dataRaw == nullptr)
                continue;

            for (auto& layer : dataRaw->layerData) {

                std::string name_noExt = layer.mapName;
                name_noExt.append(std::to_string(layer.layer));

                if (layer.layer >= 3 && layer.layer <= 5) {
                    mapBuilder.map_layers.push_back({name_noExt, layer.layer, std::move(layer.tileOverrides)});
                }
                else {
                    auto layerFileName = createLocalFileName(name_noExt);

                    writeTexture(layer.texture, renderer, layerFileName);

                    SDL_DestroyTexture(layer.texture);

                    // this is where we make the map object
                    mapBuilder.map_layers.push_back({name_noExt, layer.layer});
                }
            }
            mapBuilder.set_name = dataRaw->tileSetName;
            mapBuilder.map_width = dataRaw->width;
            mapBuilder.map_height = dataRaw->height;
            maps.emplace(mapName, mapBuilder);

            if (std::count(TEMP_fileNames.begin(), TEMP_fileNames.end(), dataRaw->tileSetName) == 0) {
                // tileset texture has not yet been loaded
                TEMP_fileNames.push_back(dataRaw->tileSetName);
                auto fileName = createLocalFileName(dataRaw->tileSetName);
                writeTexture(dataRaw->tileSet, renderer, fileName);
            }
            delete dataRaw;
        }
    }
    return true;
}

SDL_Rect ResourceManager::getRectFromTextureName(const std::string &fileName) {
    if (this->textureRects.count(fileName) == 0)
        return {};
    return this->textureRects.at(fileName);
}

bool ResourceManager::loadTextures(SDL_Renderer* renderer) {
    ImagePacker packer;
    packer.loadImages(renderer, this, TEST_PATH);

    return true;
}

bool ResourceManager::loadEntities() {

    // TODO: might want to -actually- put animation loading in its own function

    for (const auto& file : iter_dir(ENTITY_PATH)) {

        // Downsides to macosx development lol
        if (file.path().filename().string()[0] == '.') continue;

        std::ifstream istream(file);
        json manifest = json::parse(istream);
        istream.close();

        std::string name, affinity_;

        try {
            float moveSpeed;
            int z;
            name = manifest["Name"];
            affinity_ = manifest["Element"];
            //ElementalAffinity affinity = getTypeFromString(affinity_.c_str());
            // TODO: Attributes

            moveSpeed = manifest["Overworld"]["MoveSpeed"];
            z = manifest["Overworld"]["Z"];
            std::string dir = manifest["Directory"];

            AnimationStateMachine overworld(dir), battle(dir);

            for (auto& anim : manifest["Overworld"]["Animations"]) {
                std::string anim_name = anim["Name"];
                // The texture rect is stored with the identifier, so we need to reconstruct that
                std::string rect_name = anim_name;
                rect_name.append("_").append(dir);
                auto [_, _a, w, h] = textureRects[rect_name];
                int framesX = anim["FramesX"];
                int framesY = anim["FramesY"];
                double speed = anim["Speed"];
                overworld.AddAnimation(anim_name, Animation{framesX, framesY, speed, {static_cast<float>(w), static_cast<float>(h)}});
            }

            for (auto& anim : manifest["Battle"]["Animations"]) {
                std::string fn = anim["Name"];
                //fn.append(dir);
                auto [_, _a, w, h] = textureRects[fn];
                int fx = anim["FramesX"];
                int fy = anim["FramesY"];
                double fs = anim["Speed"];
                battle.AddAnimation(fn, Animation{fx, fy, fs, {static_cast<float>(w), static_cast<float>(h)}});
            }

            EntityBuilder eb{
                name, moveSpeed, true, z, true, overworld, battle};
            if (manifest.contains("IsPlayer") && manifest["IsPlayer"]) {
                eb.is_player = true;
            }
            //Entities[name] = GlobalEntity(eb);
            Entities.emplace(name, eb);
        } catch (json::type_error &e) {
            std::cerr << e.what() << std::endl;
        } catch (json::parse_error &e) {
            std::cerr << e.what() << std::endl;
        }

    }
    return true;
}

bool ResourceManager::loadAnimations() {
    // TODO
    return false;
}

// TODO: may be deleted (probably)
std::map<std::string, GlobalEntity> *ResourceManager::getEntities() {
    return &Entities;
}

bool ResourceManager::loadEnvironments() {

    std::ifstream istream(ENVIRONMENT_PATH);
    json manifest = json::parse(istream);
    istream.close();
    for (const auto& env : manifest) {
        EnvironmentBuilder eb;
        eb.name = env["Name"];
        if (env.contains("SpriteName")) {
            eb.use_spriteName = true;
            eb.sprite_name = env["SpriteName"];
        }
        eb.z = env["Z"];
        environments.emplace(eb.name, eb);
    }

    return true;
}

Environment *ResourceManager::getEnvironment(const std::string &name) {
    return (environments.count(name) > 0) ? &environments.at(name) : nullptr;
}

// Shout out to some bloke on stackoverflow for this main idea
void ResourceManager::writeTexture(SDL_Texture *texture, SDL_Renderer *renderer, const std::string &name) {
    SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

    SDL_SetRenderTarget(renderer, texture);
    int w, h;
    Uint32 f;
    SDL_QueryTexture(texture, &f, nullptr, &w, &h);
    std::cout << name << " " << SDL_GetPixelFormatName(f) << std::endl;

    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, PIXEL_FORMAT);
    //SDL_Surface* surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, nullptr, 0, surf->pixels, surf->pitch);

    IMG_SavePNG(surf, name.c_str());
    SDL_FreeSurface(surf);

    SDL_SetRenderTarget(renderer, prev_target);
}

Maps *ResourceManager::getMap(const std::string &name) {
    return (maps.count(name) > 0) ? &maps.at(name) : nullptr;
}

/**
 * Takes in a file name (as of now assumed to be a .png image with no extension) and returns a full path
 * If I want to make this more robust, I'll implement an enum
 * @param name The file name
 * @return A string representation of the path to save the file at
 */
std::string ResourceManager::createLocalFileName(const std::string &name) {
    std::string out = IMAGE_PATH;
    out.append(name);
    out.append(".png");
    return out;
}

SDL_Texture *ResourceManager::getAtlas() {
    return atlas;
}

GlobalEntity *ResourceManager::getEntity(const std::string &name) {
    return &Entities.at(name);
}





