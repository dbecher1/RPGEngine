//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_UIELEMENT_H
#define RPG_UIELEMENT_H

#include "SDL_rect.h"
#include "Text.h"
#include <vector>
#include <array>
#include <string>

class SpriteBatch;
class UIElement;

struct UIElementBuilder {
    // Location is given as float coordinates, 0-1
    SDL_FRect Location{};
    std::string name;
    float curve = 0.03f;
    bool is_active = false;
    std::vector<TextBuilder> text_builders;
    std::vector<UIElementBuilder> nodes;
    SDL_Rect* parent = nullptr;
};

class UIElement {
public:
    UIElement() = delete;
    explicit UIElement(UIElementBuilder& builder);
    void Update();
    void Draw(SpriteBatch* sb) const;
    [[nodiscard]] bool isActive() const;
    void setActiveState(bool state);
private:
    std::string name;
    std::vector<UIElement> children;
    std::vector<Text> text;
    SDL_Rect raw_rect{};
    SDL_Rect inner_rect{};
    std::vector<SDL_Rect> rects;
    std::array<std::vector<SDL_Point>, 4> curved_edges;
    std::vector<SDL_Point> outline_straight;
    std::vector<SDL_Point> outline_curves;
    bool is_active;

    static SDL_Rect convertRectToScreen(SDL_FRect r);
    static void GenerateCircle(std::vector<SDL_Point> *points, int radius, SDL_Point origin, int corner);

    friend class SpriteBatch;
};


#endif //RPG_UIELEMENT_H
