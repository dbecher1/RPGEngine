//
// Created by Daniel Becher on 2/20/24.
//

#include "Maps.h"
#include "../SpriteBatch.h"

Maps::Maps(MapBuilder mapBuilder) :
mapName(mapBuilder.map_name),
layers(std::move(mapBuilder.map_layers)),
tileSetName(mapBuilder.set_name) {}

void Maps::Draw(SpriteBatch *sb) {
    for (auto &l : layers) {
        DrawCommand dc;
        dc.z = l.layer;
        if (l.layer >= 3 && l.layer <= 5) {
            for (auto& tile : l.tileOverrides) {
                dc.SpriteName = tileSetName.c_str();
                dc.overrideSrcRect = true;
                // dc.override = &l.srcRects;
                dc.tileOverride = &tile;
                // Need to set the y value so that we get y-sorting
                dc.position.y = tile.dest.y;
                dc.dimensions.y = static_cast<float>(tile.src.h);
                sb->Add(dc);
            }
        }
        else {
            dc.SpriteName = l.name.c_str();
            sb->Add(dc);
        }
    }
}

void Maps::Dump() {
    for (const auto &l : layers) {
        std::cout << "Layer " << l.layer << ", Name: " << l.name << std::endl;
    }
}
