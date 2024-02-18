//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_IMAGEPACKER_H
#define RPG_IMAGEPACKER_H

#include "SDL.h"
#include "SDL_image.h"
#include "../game/ResourceManager.h"
#include "../game/misc/macros.h"

#include <string>

struct ImageData {
    SDL_Surface* data;
    std::string fileName;
    int width;
    int height;
};

class ResourceManager;

static inline void sortImageData(std::vector<ImageData>* images);
static inline void printImageData(std::vector<ImageData>* images);

class ImagePacker {
public:
    bool loadImages(SDL_Renderer* renderer, ResourceManager* resourceManager, const std::string& path);

private:

};


#endif //RPG_IMAGEPACKER_H
