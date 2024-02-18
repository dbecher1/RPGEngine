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
            stateMachine.AddAnimation(fileName, {framesX, framesY, speed, {rect.w, rect.h}});
        }
    }
    return stateMachine;
}

bool ResourceManager::loadEntities() {

    // TODO: might want to -actually- put animation loading in its own folder

    for (const auto& file : dir_iter(ENTITY_PATH)) {
        std::ifstream istream(file);
        json manifest = json::parse(istream);
        istream.close();

        std::string name = manifest["Name"];
        bool isAnimated = manifest["IsAnimated"];
        int moveSpeed = manifest["MoveSpeed"];

        if (isAnimated) {
            std::string subDir(ANIMATION_PATH);
            subDir.append(manifest["Directory"]);

            std::vector<std::string> animationNames;
            for (auto& s : manifest["Animations"]) {
                animationNames.push_back(s);
            }
            AnimationStateMachine stateMachine;

            for (const auto& file2 : dir_iter(ANIMATION_PATH)) {
                if (file2.is_regular_file()) {

                    // chop off the extension
                    std::string fileName = file2.path().filename().string();
                    fileName = fileName.substr(0, fileName.find('.'));

                    if (std::count(animationNames.begin(), animationNames.end(),fileName) == 0)
                        continue;

                    std::ifstream istream2(file2);
                    json manifest2 = json::parse(istream2);
                    istream2.close();

                    int framesX = manifest2["FramesX"];
                    int framesY = manifest2["FramesY"];
                    double speed = manifest2["Speed"];

                    auto rect = textureRects[fileName];
                    stateMachine.AddAnimation(fileName, {framesX, framesY, speed, {rect.w, rect.h}});
                }
            }
            Entities.emplace_back(name, stateMachine, moveSpeed, isAnimated);
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



