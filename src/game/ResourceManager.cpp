//
// Created by Daniel Becher on 2/7/24.
//

#include "ResourceManager.h"
#include "../tools/extern/json.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>

/// TODO: probably set an upper limit on texture packing and create a way for a multi-level hierarchy of images
// TODO: Create an Entity manifest, create AnimationStateMachine loader from this

#define TEST_PATH "../resources/images/"
#define ANIMATION_PATH "../resources/animations/"
#define ENTITY_PATH "../resources/entities/"
#define RESOURCE_IMAGE_PATH ""

using dir_iter = std::filesystem::recursive_directory_iterator;
using json = nlohmann::json;


ResourceManager::~ResourceManager() {
    // ResourceManager has ownership over its resources, so it needs to free the SDL texture
    SDL_DestroyTexture(atlas);
}

bool ResourceManager::loadAllResources(SDL_Renderer* renderer) {
    bool flag = this->loadTextures(renderer);
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
    flag = flag && this->loadEntities();
    return flag;
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
    for (const auto& file : dir_iter(ANIMATION_PATH)) {
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

    for (const auto& file : dir_iter(ENTITY_PATH)) {
        std::ifstream istream(file);
        json manifest = json::parse(istream);
        istream.close();

        std::string name = manifest["Name"];
        bool isAnimated = manifest["IsAnimated"];
        float moveSpeed = manifest["MoveSpeed"];

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

            for (const auto& animation_file : dir_iter(ANIMATION_PATH)) {
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
            Entity entity(name, stateMachine, moveSpeed, isAnimated);

            // Optional Fields here

            if (manifest.contains("DefaultState"))
                entity.setDefaultAnimationState(manifest["DefaultState"]);

            if (manifest.contains("IsPlayer") && (manifest["IsPlayer"]))
                entity.setPlayer();

            Entities.emplace_back(std::move(entity));

            std::cout << "Created new entity " << name << std::endl;
        }
        else {

        }
    }

    return true;
}

bool ResourceManager::loadAnimations() {
    // TODO
    return false;
}

std::vector<Entity> *ResourceManager::getEntities() {
    return &Entities;
}



