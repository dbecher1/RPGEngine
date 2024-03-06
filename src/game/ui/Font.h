//
// Created by Daniel Becher on 3/2/24.
//

#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include <string>
#include <deque>
#include <vector>
#include "Text.h"

constexpr int MAX_CACHE_SIZE = 256;

class ResourceManager;

class Font {
public:
    Font() = default;
    explicit Font(const ResourceManager* resourceManager);
    Text GenerateText(const std::string& text, int text_size, SDL_Point origin);
    Text GenerateText(const TextBuilder& builder);
private:
    static Uint32 generateSimpleCustomHash(const std::string& text, int size, int x, int y);
    int count = 0;
    std::string test;
    std::unordered_map<char, SDL_Rect> charToRect;
    std::unordered_map<Uint32, Text> cache;
    std::deque<Uint32> access_order;
};



#endif //FONT_H
