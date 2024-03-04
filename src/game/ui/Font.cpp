//
// Created by Daniel Becher on 3/2/24.
//

#include "Font.h"
#include "../ResourceManager.h"

Font::Font(const ResourceManager* resourceManager) {
    const std::string characters{FONT_CHARACTERS};

    for (const auto c : characters) {
        std::string name{c};
        if (c >= 'a' && c <= 'z')
            name.append("_");
        name.append("_font");
        charToRect.emplace(c, resourceManager->getRectFromTextureName(name));
    }
}

/**
 *  Returns a vector of tuples of rects in the form of source, dest
 *  (Text is an alias for vector<pair<Rect, Rect>>)
 */
Text Font::GenerateText(const std::string& text, const int text_size, const SDL_Point origin) {
    auto hash = generateSimpleCustomHash(text, text_size, origin.x, origin.y);
    if (cache.count(hash) > 0)
        return cache[hash];
    Text out;
    const int len = text.size();
    out.reserve(len);
    // generate the dest rects
    for (int i = 0; i < len; i++) {
        if (text[i] == ' ') continue;
            out.push_back({
                charToRect[text[i]],
                {origin.x + ((text_size + 3) * i), origin.y, text_size, text_size}
            });
        }
    if (count < MAX_CACHE_SIZE)
        count++;
    else {
        cache.erase(hash);
        access_order.pop_front();
    }
    cache[hash] = out;
    access_order.push_back(hash);
    return out;
}

/**
 * I needed a way to make sure I could store different instances of the same text
 */
Uint32 Font::generateSimpleCustomHash(const std::string &text, const int size, const int x, const int y) {
    const int len = text.size();
    Uint32 result = len << 8;
    bool flip = false;
    for (int i = 0; i < len; i++) {
        result += text[i] * (i + 1);
        if (!flip)
            result <<= 1;
        else
            result >>= 1;
        if ((i - 8) % 16 == 0)
            flip = !flip;
    }
    result += size + x + y;
    return result;
}
