//
// Created by Daniel Becher on 2/16/24.
//

#include "SpriteBatch.h"
#include <memory>

#define USE_THREADING false
#define SCALE true
#define SCALE_FACTOR 3

SpriteBatch::SpriteBatch(SDL_Renderer *renderer_, ResourceManager *resourceManager_)
: renderer(renderer_), resourceManager(resourceManager_) {
    atlas = resourceManager->atlas;
    DrawCommands.fill({});
}

void SpriteBatch::Draw() {

    SDL_RenderClear(renderer);
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
                            return dc1.position.y < dc2.position.y;
                        });
            }
        }

        for (const auto& drawable : DrawCommands[i]) {

            SDL_Rect rect = resourceManager->getRectFromTextureName(drawable.SpriteName);
            SDL_Rect r, *dest = nullptr;

            if (!drawable.staticSprite) {
                r.x = drawable.position.x;
                r.y = drawable.position.y;
                if (drawable.useDimensions) {
                    r.w = drawable.dimensions.x;
                    r.h = drawable.dimensions.y;
                }
                else {
                    r.w = rect.w;
                    r.h = rect.h;
                }
                if (drawable.useOffset) {
                    rect.x = rect.x + (drawable.offset.x * drawable.dimensions.x);
                    rect.y = rect.y + (drawable.offset.y * drawable.dimensions.y);
                    rect.w = drawable.dimensions.x;
                    rect.h = drawable.dimensions.y;
                }
                dest = &r;
            }
            if (SCALE && dest) {
                dest->w *= SCALE_FACTOR;
                dest->h *= SCALE_FACTOR;
            }
            SDL_RenderCopy(renderer, atlas, &rect, dest);

        }
        DrawCommands[i].clear();
    }

    SDL_RenderPresent(renderer);
}

void SpriteBatch::Add(DrawCommand drawCommand) {
    // TODO: viewport filtering?
    DrawCommands[drawCommand.z].emplace_back(drawCommand);
}
