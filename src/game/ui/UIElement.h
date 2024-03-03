//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_UIELEMENT_H
#define RPG_UIELEMENT_H

#include "SDL_rect.h"
#include <vector>
#include <array>
#include <string>

class SpriteBatch;

class UIElement;

struct UIElementBuilder {
    SDL_FRect Location{};
    std::string name;
    float curve = 0.03f;
    SDL_Color color = {0x5E, 0x6D, 0xDA, 0xFF};
    SDL_Color outline_color = {0xFF, 0xFF, 0xFF, 0xFF};
    bool is_active = false;
};

class UIElement {
public:
    UIElement() = delete;
    explicit UIElement(const UIElementBuilder& builder);
    void Update();
    [[nodiscard]] bool isActive() const;
    void setActiveState(bool state);
private:
    std::string name;
    SDL_Color elementColor{};
    SDL_Color outlineColor{};
    SDL_Rect raw_rect{};
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
