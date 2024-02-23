//
// Created by Daniel Becher on 2/16/24.
//

#include "SpriteBatch.h"
#include "../math/MiscMath.h"
#include <memory>
#include <chrono>

// TODO: revisit camera + resizing

SpriteBatch::SpriteBatch(SpriteBatchBuilder sbb)
: resourceManager(sbb.resourceManager) {

    renderer = SDL_CreateRenderer(sbb.window, -1, RENDERER_FLAGS);
    if (renderer == nullptr) {
        SDL_PRINT_ERR("Couldn't create renderer!");
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // atlas = resourceManager->atlas;
    DrawCommands.fill({});

    windowWidth = INITIAL_WIDTH;
    windowHeight = INITIAL_HEIGHT;
    aspectRatio = ASPECT_RATIO;

    screenWidth = windowWidth;
    screenHeight = windowHeight;

    calculateResize();

    // create render target for possible letterboxing
    // spritebatch has ownership over this
    //renderTarget = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);

    camera = Camera{renderer, screenWidth, screenHeight};
    camera.Init_BackBuffer();
}

SpriteBatch::~SpriteBatch() {
    //SDL_DestroyTexture(renderTarget);
    SDL_DestroyRenderer(renderer);
}

void SpriteBatch::SubmitDraw() {

    SDL_RenderClear(renderer);

    /*
    if (letterbox) {
        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_RenderClear(renderer);
    }
     */

    camera.setCamera();

    /*
    std::array<std::thread*, NUM_THREADS> threads{};
    if (USE_THREADING) {
        for (int i = 3; i <= 5; i++) {
            threads[i - 3] = new std::thread([this, &i]() {
                std::sort(
                        DrawCommands[i].begin(),
                        DrawCommands[i].end(),
                        [](DrawCommand& dc1, DrawCommand& dc2) -> bool {
                            return dc1.position.y < dc2.position.y;
                        });
            });
        }
    }
    */
    for (int i = 0; i < NUM_DRAW_LAYERS; i++) {
        // If entity layer
        if ((i >= 3) && (i <= 5)) {
            if (USE_THREADING) {
                /*
                threads[i - 3]->join();
                delete threads[i - 3];
                */
            }
            else {
                // Y sort
                // TODO: play with this
                std::sort(
                        DrawCommands[i].begin(),
                        DrawCommands[i].end(),
                        [](DrawCommand& dc1, DrawCommand& dc2) -> bool {
                            return (dc1.position.y + (dc1.dimensions.y)) < (dc2.position.y + (dc2.dimensions.y));
                        });
            }
        }
        for (auto& drawable : DrawCommands[i]) {

            SDL_Rect source_rect;
            source_rect = resourceManager->getRectFromTextureName(drawable.SpriteName);

            SDL_FRect dest, *dest_ptr = nullptr;

            if (!drawable.staticSprite && !drawable.overrideSrcRect) {
                dest.x = drawable.position.x;
                dest.y = drawable.position.y;
                if (drawable.useDimensions) {
                    dest.w = drawable.dimensions.x;
                    dest.h = drawable.dimensions.y;
                }
                else {
                    dest.w = static_cast<float>(source_rect.w);
                    dest.h = static_cast<float>(source_rect.h);
                }
                if (drawable.useOffset) {
                    source_rect.x = source_rect.x + static_cast<int>(drawable.offset.x * drawable.dimensions.x);
                    source_rect.y = source_rect.y + static_cast<int>(drawable.offset.y * drawable.dimensions.y);
                    source_rect.w = static_cast<int>(drawable.dimensions.x);
                    source_rect.h = static_cast<int>(drawable.dimensions.y);
                }
                dest_ptr = &dest;
            }
            if (drawable.overrideSrcRect) {
                SDL_Rect src = drawable.tileOverride->src;
                // need to add the offset to get the accurate source rect
                source_rect.x += src.x;
                source_rect.y += src.y;
                source_rect.w = src.w;
                source_rect.h = src.h;
                dest_ptr = &drawable.tileOverride->dest;
            }

            SDL_RenderCopyExF(renderer, atlas, &source_rect, dest_ptr, 0, nullptr, SDL_FLIP_NONE);
        }
        DrawCommands[i].clear();
    }
    camera.unsetCamera();
    /*
    if (letterbox) {
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_Rect r{letter_offset, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, renderTarget, nullptr, &r);
    }
    */

    SDL_RenderPresent(renderer);
}

/**
 * Exists for the purpose of updating the camera only (for now)
 * @param position Character position
 */
void SpriteBatch::Update(Vector2 position) {
    camera.Update(position);
}

void SpriteBatch::Add(DrawCommand drawCommand) {
    // TODO: viewport filtering?
    DrawCommands[drawCommand.z].emplace_back(drawCommand);
}

/**
 * Called from the main loop when the window is resized
 * @param new_width The new window width
 * @param new_height The new window height
 */
void SpriteBatch::windowResizeEvent(int new_width, int new_height) {
    windowWidth = new_width;
    windowHeight = new_height;
    calculateResize();
}

// Helper method for the above
void SpriteBatch::calculateResize() {
    float curr_aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    if (float_eq(curr_aspect, aspectRatio)) {
        std::cout << "Valid aspect ratio!" << std::endl;
        screenWidth = windowWidth;
        screenHeight = windowHeight;
        letterbox = false;
    }
    else {
        std::cerr << "Invalid aspect ratio!" << std::endl;
        // If the aspect ratio is under the goal, shrink the height
        // If over the goal, shrink the width
        letterbox = true;
        screenHeight = windowHeight;
        screenWidth = static_cast<int>(static_cast<float>(windowHeight) * aspectRatio);
        letter_offset = (windowWidth - screenWidth) / 2;
        float new_aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
        std::cout << "Aspect ratio: " << new_aspect << std::endl;
    }
    std::cout << "Screen width: " << screenWidth << std::endl;
    std::cout << "Screen height: " << screenHeight << std::endl;
}

// This may be removed if I don't find a good purpose for it; helpful for debugging
void SpriteBatch::resetDefaultWindowSize(SDL_Window* window) {
    SDL_SetWindowSize(window, INITIAL_WIDTH, INITIAL_HEIGHT);
    windowResizeEvent(INITIAL_WIDTH, INITIAL_HEIGHT);
}

