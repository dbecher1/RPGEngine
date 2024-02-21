//
// Created by Daniel Becher on 2/7/24.
//

#include "ResourceManager.h"
#include "../tools/extern/json.hpp"
#include "../tools/MapLoader.hpp"
#include "world/Maps.h"

#include <iostream>
#include <filesystem>
#include <fstream>

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

                    // TODO: this is where we make the map object
                    // FIXME: currently broken lol
                    // TODO: make layers 3-5 draw per-tile... way harder - still have to do this

                    // mapBuilder.map_layers.push_back({name_noExt, layer.layer, std::move(layer.srcRects)});
                    mapBuilder.map_layers.push_back({name_noExt, layer.layer});
                }
            }
            mapBuilder.set_name = dataRaw->tileSetName;
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

AnimationStateMachine ResourceManager::asmLoader() {
    std::cerr << "Using asmLoader method! For debugging only, ye been warned..." << std::endl;
    AnimationStateMachine stateMachine;
    for (const auto& file : iter_dir(ANIMATION_PATH)) {
        if (file.is_regular_file()) {
            std::ifstream istream(file);
            json manifest = json::parse(istream);
            istream.close();

            int framesX = manifest.at("FramesX");
            int framesY = manifest.at("FramesY");
            double speed = manifest.at("Speed");

            // chop off the extension
            std::string fileName = file.path().filename().string();
            fileName = fileName.substr(0, fileName.find('.'));

            auto rect = textureRects.at(fileName);
            stateMachine.AddAnimation(fileName, {framesX, framesY, speed, {static_cast<float>(rect.w), static_cast<float>(rect.h)}});
        }
    }
    return stateMachine;
}

bool ResourceManager::loadEntities() {

    // TODO: might want to -actually- put animation loading in its own function

    for (const auto& file : iter_dir(ENTITY_PATH)) {
        std::ifstream istream(file);
        json manifest = json::parse(istream);
        istream.close();

        std::string name = manifest["Name"];
        bool isAnimated = manifest["IsAnimated"];
        float moveSpeed = manifest["MoveSpeed"];
        int z = manifest["Z"];

        if (isAnimated) {
            std::string subDir(ANIMATION_PATH);
            subDir.append(manifest["Directory"]);

            std::vector<std::string> animationNames;
            for (auto& s : manifest["Animations"]) {
                animationNames.push_back(s);
            }
            AnimationStateMachine stateMachine;

            // TODO: rename file2 and other such variables to be clearer
            int eid = -1;

            for (const auto& animation_file : iter_dir(ANIMATION_PATH)) {
                if (animation_file.is_regular_file()) {

                    // chop off the extension
                    std::string animation_fileName = animation_file.path().filename().string();
                    animation_fileName = animation_fileName.substr(0, animation_fileName.find('.'));

                    if (std::count(animationNames.begin(), animationNames.end(), animation_fileName) == 0)
                        continue;

                    std::ifstream animation_istream(animation_file);
                    json animation_manifest = json::parse(animation_istream);
                    animation_istream.close();

                    int framesX = animation_manifest["FramesX"];
                    int framesY = animation_manifest["FramesY"];
                    double speed = animation_manifest["Speed"];

                    auto rect = textureRects[animation_fileName];

                    // animations will either be singular or plural
                    // If plural, it will contain the field "NumAnimations"
                    if (animation_manifest.contains("NumAnimations")) {
                        int numAnimations = animation_manifest["NumAnimations"];
                        int rowHeight = static_cast<int>(rect.h / numAnimations);
                        SDL_Rect subRect{rect.x, rect.y, rect.w, rowHeight};

                        // Have to create sub rects and store them in the map before creating animation
                        for (int i = 0; i < numAnimations; i++) {
                            SDL_Rect newSrc = {subRect.x,
                                               subRect.y + (rowHeight * i),
                                               rect.w,
                                               rowHeight};
                            std::string newSrcName = animation_manifest["Names"][i];
                            eid = Entity::EID; // the next entity to be created will have this value
                            stateMachine.AddAnimation(newSrcName, {framesX, framesY, speed, {static_cast<float>(rect.w), static_cast<float>(rowHeight)}}, eid);
                            newSrcName.append(std::to_string(eid));
                            //textureRects[newSrcName] = newSrc;
                            textureRects.emplace(newSrcName, newSrc);

                        }
                    }
                    else {
                        stateMachine.AddAnimation(animation_fileName, {framesX, framesY, speed, {static_cast<float>(rect.w), static_cast<float>(rect.h)}});
                    }
                }
            }
            EntityBuilder eb {
                .name = name,
                .animationStateMachine = stateMachine,
                .move_speed = moveSpeed,
                .animated = true,
                .z = z,
            };
            if (manifest.contains("DefaultState"))
                eb.defaultAnim = std::optional<std::string>(manifest["DefaultState"]);

            if (manifest.contains("IsPlayer") && (manifest["IsPlayer"]))
                eb.is_player = true;

            Entities.emplace_back(eb);

            std::cout << "Created new entity " << name << std::endl;
        }
        else {
            // TODO: non-animated entities?
        }
    }
    return true;
}

bool ResourceManager::loadAnimations() {
    // TODO
    return false;
}

// TODO: may be deleted (probably)
std::vector<Entity> *ResourceManager::getEntities() {
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





