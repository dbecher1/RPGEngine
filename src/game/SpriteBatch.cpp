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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
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

    // Draw UI
    for (auto &ui : uiDrawQueue) {

        SDL_SetRenderDrawColor(renderer, ui->elementColor.r, ui->elementColor.g, ui->elementColor.b, ui->elementColor.a);

        SDL_RenderFillRects(renderer, ui->rects.data(), static_cast<int>(ui->rects.size()));
        for (auto& edge : ui->curved_edges) {
            SDL_RenderDrawLines(renderer, edge.data(), static_cast<int>(edge.size()));
        }

        // outlines
        SDL_SetRenderDrawColor(renderer, ui->outlineColor.r, ui->outlineColor.g, ui->outlineColor.b, ui->outlineColor.a);

        for (int i = 0; i < ui->outline_straight.size() - 1; i += 2) {
            SDL_RenderDrawLine(renderer, ui->outline_straight[i].x, ui->outline_straight[i].y, ui->outline_straight[i + 1].x, ui->outline_straight[i + 1].y);
        }

        SDL_RenderDrawPoints(renderer, ui->outline_curves.data(), static_cast<int>(ui->outline_curves.size()));
    }
    uiDrawQueue.clear();

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

void SpriteBatch::Convert_Rect_toScreen(SDL_FRect* r) const {
    r->x *= static_cast<float>(screenWidth);
    r->y *= static_cast<float>(screenHeight);
    r->w *= static_cast<float>(screenWidth);
    r->h *= static_cast<float>(screenHeight);
}

void SpriteBatch::DrawUI() {

    // TODO: scale the curve to the size of the widget
    // TODO: outline
    // TODO: sub-widgets
    // TODO: ellipse mode (width != height)

    const float curve = 0.05f;

    std::vector<SDL_FRect> rects;
    std::vector<SDL_Point> points1;
    std::vector<SDL_Point> points2;
    std::vector<SDL_Point> points3;
    std::vector<SDL_Point> points4;

    std::vector<SDL_Point> outline_straight;
    std::vector<SDL_Point> outline_curves;

    SDL_SetRenderDrawColor(renderer, 94, 109, 218, 0xFF);
    SDL_FRect r{0.1f, 0.1f, 0.8f, 0.8f};
    Convert_Rect_toScreen(&r);

    int radius = (int)(curve * (float)screenWidth);
    GenerateCircle2(&points1, radius, {(int) r.x, (int) r.y}, 0);
    GenerateCircle2(&points2, radius, {(int)r.x + (int)r.w, (int)r.y}, 1);
    GenerateCircle2(&points3, radius, {(int) r.x, (int) r.y + (int)r.h}, 2);
    GenerateCircle2(&points4, radius, {(int) r.x + (int)r.w, (int) r.y + (int)r.h}, 3);

    SDL_FRect left_side = r;
    left_side.x += 1;
    left_side.w = radius;
    left_side.y += radius;
    left_side.h -= (radius * 2);

    SDL_FRect right_side;
    right_side.x = left_side.x + r.w - radius;
    right_side.y = left_side.y;
    right_side.w = left_side.w - 2;
    right_side.h = left_side.h;

    r.y += 1;
    r.h -= 2;
    r.x += radius;
    r.w -= (radius * 2) - 2;

    rects.push_back(r);
    rects.push_back(left_side);
    rects.push_back(right_side);

    // OUTLINE
    outline_straight.push_back({static_cast<int>(r.x), static_cast<int>(r.y)});
    outline_straight.push_back({static_cast<int>(r.x + r.w), static_cast<int>(r.y)});
    outline_straight.push_back({static_cast<int>(r.x), static_cast<int>(r.y + r.h)});
    outline_straight.push_back({static_cast<int>(r.x + r.w), static_cast<int>(r.y + r.h)});
    outline_straight.push_back({static_cast<int>(left_side.x), static_cast<int>(left_side.y)});
    outline_straight.push_back({static_cast<int>(left_side.x), static_cast<int>(left_side.y + left_side.h)});
    outline_straight.push_back({static_cast<int>(right_side.x + right_side.w), static_cast<int>(right_side.y)});
    outline_straight.push_back({static_cast<int>(right_side.x + right_side.w), static_cast<int>(right_side.y + right_side.h)});

    // hacky, but it works
    for (const auto& p : points1) {
        if (p.x <= static_cast<int>(r.x) && p.y <= static_cast<int>(left_side.y)) {
            outline_curves.push_back(p);
        }
    }
    for (const auto& p : points3) {
        if (p.x <= static_cast<int>(r.x) && p.y >= static_cast<int>(left_side.y + left_side.h)) {
            outline_curves.push_back(p);
        }
    }

    for (const auto& p : points2) {
        if (p.x >= static_cast<int>(right_side.x) && p.y <= static_cast<int>(right_side.y)) {
            outline_curves.push_back(p);
        }
    }
    for (const auto& p : points4) {
        if (p.x >= static_cast<int>(right_side.x) && p.y >= static_cast<int>(right_side.y + right_side.h) ) {
            outline_curves.push_back(p);
        }
    }

    // Middle rect/side bars
    SDL_RenderFillRectsF(renderer, rects.data(), (int)rects.size());

    // Curved edges
    SDL_RenderDrawLines(renderer, points1.data(), static_cast<int>(points1.size()));
    SDL_RenderDrawLines(renderer, points2.data(), static_cast<int>(points2.size()));
    SDL_RenderDrawLines(renderer, points3.data(), static_cast<int>(points3.size()));
    SDL_RenderDrawLines(renderer, points4.data(), static_cast<int>(points4.size()));

    // draw outlines
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (int i = 0; i < outline_straight.size() - 1; i += 2) {
        SDL_RenderDrawLine(renderer, outline_straight[i].x, outline_straight[i].y, outline_straight[i + 1].x, outline_straight[i + 1].y);
    }
    SDL_RenderDrawPoints(renderer, outline_curves.data(), static_cast<int>(outline_curves.size()));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void SpriteBatch::GenerateCircle2(std::vector<SDL_Point> *points, int radius_, SDL_Point origin, int corner) {
    int radius = radius_;
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    int x_offset = radius, y_offset = radius;
    // 0 - upper left (default)
    // 1 - upper right
    // 2 - lower left
    // 3 - lower right
    switch (corner) {
        case 1: {
            x_offset *= -1;
            break;
        }
        case 2: {
            y_offset *= -1;
            break;
        }
        case 3: {
            x_offset *= -1;
            y_offset *= -1;
            break;
        }
        default: break;
    }
    origin.x += x_offset;
    origin.y += y_offset;

    while (x >= y) {
        points->push_back({origin.x + x, origin.y + y});
        points->push_back({origin.x + x, origin.y - y});
        points->push_back({origin.x + y, origin.y + x});
        points->push_back({origin.x + y, origin.y - x});

        points->push_back({origin.x - x, origin.y + y});
        points->push_back({origin.x - x, origin.y - y});
        points->push_back({origin.x - y, origin.y + x});
        points->push_back({origin.x - y, origin.y - x});

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void SpriteBatch::Add(UIElement *ui) {
    uiDrawQueue.push_back(ui);
}

