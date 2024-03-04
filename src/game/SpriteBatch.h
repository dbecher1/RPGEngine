//
// Created by Daniel Becher on 2/16/24.
//

#ifndef RPG_SPRITEBATCH_H
#define RPG_SPRITEBATCH_H

#include "SDL.h"
#include "gfx/DrawCommand.h"
#include "ResourceManager.h"
#include "gfx/Camera.h"
#include "ui/UIElement.h"
#include <vector>
#include <array>
#include <thread>

/// Important note regarding rendering layers!!!
/// As of right now, the Z layers are strict and specific
/// 0, 1, 2 - Ground/below entity layer
/// 3, 4, 5 - Entity layer (includes buildings!)
/// 6, 7, 8 - Above entity layer
/// 9 - Maybe UI?
/// This is important because only the Entity layers will be Y-sorted

constexpr int NUM_DRAW_LAYERS = 10;
constexpr bool USE_LETTERBOXING = false;
using Text = std::vector<std::pair<SDL_Rect, SDL_Rect>>;

struct SpriteBatchBuilder {
    SDL_Window* window;
    ResourceManager* resourceManager;
};

class SceneManager;
class ResourceManager;

class SpriteBatch {
public:
    SpriteBatch() = delete;
    explicit SpriteBatch(SpriteBatchBuilder sbb);
    ~SpriteBatch();
    void SubmitDraw();
    void Add(DrawCommand drawCommand);
    void Add(UIElement* ui);
    void Add(Text& t);

    void Update(Vector2 position);

    void setCameraBoundaries(int w, int h);
    void setCameraBoundaries(SDL_Point p);

    void resetDefaultWindowSize(SDL_Window* window);
    void windowResizeEvent(int new_width, int new_height);
private:
    void calculateResize();
    int windowWidth, windowHeight, screenWidth{}, screenHeight{};
    float aspectRatio;
    bool letterbox{};
    int letter_offset{};
    Camera camera;
    SDL_Renderer* renderer;
    SDL_Texture* atlas = nullptr;
    ResourceManager* resourceManager;
    std::array<std::vector<DrawCommand>, NUM_DRAW_LAYERS> DrawCommands;
    std::vector<UIElement*> uiDrawQueue;
    std::vector<Text> textQueue;

    SDL_Texture* renderTarget{};
    SDL_Texture* ui_target{};
    //BackbufferTexture ui_target;

    friend class SceneManager;
    friend class ResourceManager;
};


#endif //RPG_SPRITEBATCH_H
