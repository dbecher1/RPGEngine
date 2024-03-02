//
// Created by Daniel Becher on 2/7/24.
//

#include "ImagePacker.h"
#include "SDL2_image/SDL_image.h"
#include "../game/ResourceManager.h"

constexpr bool DEBUG_SAVE_IMAGE = true;

#include <iostream>
#include <filesystem>
#include <vector>

bool ImagePacker::loadImages(SDL_Renderer* renderer, ResourceManager *resourceManager, const std::string& path) {
    // TODO: add error checking
    // TODO: fix pathing issues for release builds (long term)

    std::vector<ImageData> images;

    std::cout << std::filesystem::current_path() << std::endl;

    for (const auto &file : std::filesystem::recursive_directory_iterator(path)) {
        if (file.is_regular_file()) {
            SDL_Surface* temp = IMG_Load(file.path().c_str());

            if (temp == nullptr) continue;

            std::string fName = file.path().filename().string();

            if (!INCLUDE_EXTENSION) {
                fName = fName.substr(0, fName.find('.'));
            }

            if (std::string parent = file.path().parent_path().filename().string(); parent != "images") {
                fName.append("_");
                fName.append(parent);
            }

            images.push_back({temp, fName, temp->w, temp->h});
        }
    }
    sortImageData(&images);

    // Packing sequence begins here after images are sorted

    int x = 0, y = 0, max_height = 0, final_height = 0, boundary = 512;
    constexpr int border = 0; // if we want a space in between images, opting for 0
    bool success;
    do {
        success = true;
        for (const auto& img : images) {
            // Check the edge case here where the image is larger than the boundary itself
            if (img.width > boundary) {
                resetPacking(resourceManager, &success, &boundary, &x, &y);
            }
            // test the width against the boundary
            if (x + img.width > boundary) {
                if (max_height == 0)
                    max_height = img.height;
                // reset to move to next line
                y += max_height + border;
                x = 0;
                max_height = 0;
            }
            if (y + img.height > boundary) {
                resetPacking(resourceManager, &success, &boundary, &x, &y);
                break;
            }
            SDL_Rect rect {x, y, img.width, img.height};
            resourceManager->textureRects.emplace(img.fileName, rect);

            x += img.width + border;

            if (img.height > max_height) {
                max_height = img.height;
                final_height = y + max_height + border;
            }
        }
    } while (!success);

    SDL_Surface* final_surf = SDL_CreateRGBSurfaceWithFormat(0, boundary, final_height, 32, PIXEL_FORMAT);

    for (const auto& i : images) {
        SDL_BlitSurface(i.data, nullptr, final_surf, &resourceManager->textureRects.at(i.fileName));
    }

    if (DEBUG_SAVE_IMAGE)
        IMG_SavePNG(final_surf, "img.png");

    resourceManager->atlas = SDL_CreateTextureFromSurface(renderer, final_surf);
    SDL_SetTextureBlendMode(resourceManager->atlas, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(final_surf);
    return true;
}

inline void ImagePacker::resetPacking(ResourceManager* rm, bool* success, int* boundary, int* x, int* y) {
    *success = false;
    *boundary = static_cast<int>(*boundary * 1.5);
    rm->textureRects.clear();
    *x = 0;
    *y = 0;
}

/**
 * Simple custom selection sort implementation; I couldn't get things to work with the standard library functions (due to ImageData being a user defined class). I know there's a way to do that but honestly, this is shorter than figuring that out.
 * @param images The vector to sort
 */
static void sortImageData(std::vector<ImageData>* images) {

    for (int i = 0; i < images->size() - 1; i++) {
        int max = images->at(i).height, max_loc = i;

        for (int j = i + 1; j < images->size(); j++) {
            // using >= because if we have a series of images with equal height,
            // we want to store the last index of the image with the same height
            if (images->at(j).height >= max) {
                max = images->at(j).height;
                max_loc = j;
            }
        }
        // if heights are equal, compare widths
        // if widths are equal, same image or at least same dimensions, don't swap
        // else swap based on width
        // if the width of a later image is less or equal, we don't want to swap
        if ((max == images->at(i).height) &&
            (images->at(i).width <= images->at(max_loc).width))
            continue;

        // perform the swap
        ImageData swap_temp = images->at(i);
        std::swap(images->at(i), images->at(max_loc));
        images->at(max_loc) = std::move(swap_temp);
    }
}
