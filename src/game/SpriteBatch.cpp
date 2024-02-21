//
// Created by Daniel Becher on 2/16/24.
//

#include "SpriteBatch.h"
#include "../math/MiscMath.h"
#include <memory>

#define USE_THREADING false
#define SCALE false
#define SCALE_FACTOR 1

// TODO: Fix resize glitch where the width starts to get clipped
// I know -where- the bug is, just haven't figured out the best way to fix it...

SpriteBatch::SpriteBatch(SDL_Renderer *renderer_, ResourceManager *resourceManager_)
: renderer(renderer_), resourceManager(resourceManager_) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    atlas = resourceManager->atlas;
    DrawCommands.fill({});
    // Set an initial capacity of 50 objects
    // This maths out to just under 20kb of memory reserved for the draw commands
    for (auto& dc : DrawCommands) {
        dc.reserve(DRAW_LAYER_COUNT);
    }
    windowWidth = INITIAL_WIDTH;
    windowHeight = INITIAL_HEIGHT;
    aspectRatio = ASPECT_RATIO;

    calculateResize();

    // create render target for possible letterboxing
    // spritebatch has ownership over this
    renderTarget = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
}

SpriteBatch::~SpriteBatch() {
    SDL_DestroyTexture(renderTarget);
}

void SpriteBatch::Draw() {

    SDL_RenderClear(renderer);

    if (letterbox) {
        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_RenderClear(renderer);
    }

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

    for (int i = 0; i < NUM_DRAW_LAYERS; i++) {
        // If entity layer
        if ((i >= 3) && (i <= 5)) {
            if (USE_THREADING) {
                threads[i - 3]->join();
                delete threads[i - 3];
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

        for (const auto& drawable : DrawCommands[i]) {

            SDL_Rect rect;
            rect = resourceManager->getRectFromTextureName(drawable.SpriteName);
            SDL_FRect r, *dest = nullptr;

            if (!drawable.staticSprite && !drawable.overrideSrcRect) {
                r.x = drawable.position.x;
                r.y = drawable.position.y;
                if (drawable.useDimensions) {
                    r.w = drawable.dimensions.x;
                    r.h = drawable.dimensions.y;
                }
                else {
                    r.w = static_cast<float>(rect.w);
                    r.h = static_cast<float>(rect.h);
                }
                if (drawable.useOffset) {
                    rect.x = rect.x + static_cast<int>(drawable.offset.x * drawable.dimensions.x);
                    rect.y = rect.y + static_cast<int>(drawable.offset.y * drawable.dimensions.y);
                    rect.w = static_cast<int>(drawable.dimensions.x);
                    rect.h = static_cast<int>(drawable.dimensions.y);
                }
                dest = &r;
            }
            if (SCALE && dest) {
                dest->w *= SCALE_FACTOR;
                dest->h *= SCALE_FACTOR;
            }
            if (drawable.overrideSrcRect) {

                // TODO: kill me, please

                SDL_Rect src = drawable.tileOverride->src;
                // need to add the offset to get the accurate source rect in theory
                rect.x += src.x;
                rect.y += src.y;
                //rect.x = src.x;
                //rect.y = src.y;
                rect.w = src.w;
                rect.h = src.h;
                dest = &drawable.tileOverride->dest;
            }
            else {
                // SDL_RenderCopyExF(renderer, atlas, &rect, dest, 0, nullptr, SDL_FLIP_NONE);
            }
            SDL_RenderCopyExF(renderer, atlas, &rect, dest, 0, nullptr, SDL_FLIP_NONE);

            // SDL_RenderCopy(renderer, atlas, &rect, dest);
        }
        DrawCommands[i].clear();
    }

    if (letterbox) {
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_Rect r{letter_offset, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, renderTarget, nullptr, &r);
    }
    SDL_RenderPresent(renderer);
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
