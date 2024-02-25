//
// Created by Daniel Becher on 2/20/24.
//

#ifndef RPG_MAPS_H
#define RPG_MAPS_H

#include <string>
#include <vector>
#include "../gfx/DrawCommand.h"
#include "../../tools/Tile.h"

class SpriteBatch;

struct MapLayer {
    std::string name;
    int layer;
    // std::vector<SDL_Rect> srcRects;
    std::vector<Tile> tileOverrides;
};

struct MapBuilder {
    std::string map_name;
    std::string set_name;
    std::vector<MapLayer> map_layers;
    int map_width, map_height;
};

class Maps {
public:
    Maps() = delete;
    explicit Maps(MapBuilder mapBuilder);
    void Draw(SpriteBatch* sb);
    void Dump();
    [[nodiscard]] std::string getMapName() const;
    SDL_Point getDimensions();
private:
    std::string mapName;
    std::string tileSetName;
    std::vector<MapLayer> layers;
    int mapWidth, mapHeight;
};


#endif //RPG_MAPS_H
