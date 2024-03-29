//
// Created by Daniel Becher on 3/4/24.
//

#ifndef TEXT_H
#define TEXT_H

#include "SDL_rect.h"
#include <vector>
#include <string>

constexpr int TEXT_SPACE = 3;
constexpr float TEXT_WIDTH_MANUAL_MOD = 0.75f; // set to 1.0 for the normal width of the staticText

using Text = std::vector<std::pair<SDL_Rect, SDL_Rect>>;

// If no alignment enum is given, staticText is drawn from the origin
// With origin = upper left rect of the first letter

// If alignment is given, the size of the container is considered, and the staticText is anchored there

enum TextAlignmentX {
    X_ALIGN_NONE = 0,
    X_ALIGN_LEFT,
    X_ALIGN_CENTER,
    X_ALIGN_RIGHT
};

enum TextAlignmentY {
    Y_ALIGN_NONE = 0,
    Y_ALIGN_TOP,
    Y_ALIGN_MIDDLE,
    Y_ALIGN_BOTTOM
};

class Font;

struct TextBuilder {
    Font* font = nullptr;
    std::string text_raw;
    int size = 12;
    TextAlignmentX alignment_x = X_ALIGN_NONE;
    TextAlignmentY alignment_y = Y_ALIGN_NONE;
    SDL_Point origin{};
    SDL_Point offset{}; // manual adjust after alignment for fine tuning
    bool is_dynamic = false; // for text that is updated
    std::string dyn_id; // used with dynamic text
    [[nodiscard]] int calculateWidth() const {
        int w = 0;
        const int len = text_raw.length();
        for (int _ = 0; _ < len; _++) {
            w += (size * TEXT_WIDTH_MANUAL_MOD) + TEXT_SPACE;
        }
        w -= TEXT_SPACE; // lazy way to remove the trail
        return w;
    }
    [[nodiscard]] int calculateHeight() const {
        return size;
    }
};

enum TextObjectType {
    TEXT_TYPE_STATIC = 0,
    TEXT_TYPE_DYNAMIC,
};

/**
 * Essentially a wrapper struct for the Text alias, intended for text that's frequently updated.
 * All information from its builder is saved except for the string itself
 */
struct DynamicText {
    Text data;
    Font* font;
    int size;
    TextAlignmentX alignment_x;
    TextAlignmentY alignment_y;
    SDL_Point origin{};
    SDL_Point offset{};
};

#endif //TEXT_H
