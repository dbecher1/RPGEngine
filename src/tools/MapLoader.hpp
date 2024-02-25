//
// Created by Daniel Becher on 2/20/24.
//

#ifndef RPG_MAPLOADER_HPP
#define RPG_MAPLOADER_HPP

#include "SDL.h"
#include "SDL2_image/SDL_image.h"
#include "extern/json.hpp"
#include "../game/misc/macros.h"
#include "Tile.h"

#include <vector>
#include <filesystem>
#include <fstream>

using json = nlohmann::json;
using iter_dir = std::filesystem::recursive_directory_iterator;
using Path = std::filesystem::path;

// TODO: collision data

namespace MapLoader {

    struct RawMapLayerData {
        std::string mapName;
        int layer;
        SDL_Texture* texture;
        // std::vector<SDL_Rect> srcRects;
        std::vector<Tile> tileOverrides;
    };

    struct RawMapData {
        std::vector<RawMapLayerData> layerData;
        SDL_Texture* tileSet;
        std::string tileSetName;
        int width, height;
    };

    struct Chunk {
        int x, y, width, height;
        std::vector<int> tileData;
    };

    struct TileLayer {
        int start_x, start_y, width, height, layer_id;
        std::vector<Chunk> chunks;
    };

    struct TileSetData {
        int tileWidth, tileHeight, imageWidth, imageHeight, numTiles, mapWidth, mapHeight;
        inline SDL_Rect getSrcRectFromId(int id) {
            return {((id % mapWidth) - 1) * tileWidth,
                    (id / mapWidth) * tileHeight,
                    tileWidth, tileHeight};
        };
    };

    /**
     * Given a path extension, populates the path pointer to the path of that file and returns true if successful.
     * Assumes that there will only be one file of that type in the directory, else will return the first result found.
     * @param path A pointer to the path variable to set the value to if successful
     * @param ext A string representation of the extension to search for
     * @return True if successful, else false
     */
    inline bool findFileByPath(const Path& searchDir, Path* dest, const std::string& ext) {
        bool found = false;
        for (const auto &f: iter_dir(searchDir)) {
            if (f.is_regular_file()) {
                if (f.path().string().find(ext) != std::string::npos) {
                    found = true;
                    *dest = f.path();
                    break;
                }
            }
        }
        if (!found) {
            std::cerr << "Error finding" << ext << " file in" << searchDir << std::endl;
        }
        return found;
    }

    /**
     * Given a file path (to a directory containing map data), constructs the map into SDL Textures and returns a vector of containing objects
     * The folder is expected to have exactly one of: .png, .tsj, .tmj files
     */
    inline RawMapData* loadRawMapData(const Path &path, SDL_Renderer* renderer) {
        std::vector<TileLayer> layers; // used for parsing
        std::vector<RawMapLayerData> data; // used for final data

        std::string dirName = path.filename().string();

        // Load tileset data first
        Path setPath;
        if (!findFileByPath(path, &setPath, ".tsj")) {
            return nullptr;
        }
        std::fstream set_fStream(setPath);
        json setData = json::parse(set_fStream);
        set_fStream.close();

        TileSetData tileSet{};
        try {
            tileSet.tileWidth = setData["tilewidth"];
            tileSet.tileHeight = setData["tileheight"];
            tileSet.imageWidth = setData["imagewidth"];
            tileSet.imageHeight = setData["imageheight"];
            tileSet.numTiles = setData["tilecount"];
            tileSet.mapWidth = setData["columns"];
            tileSet.mapHeight = tileSet.numTiles / tileSet.mapWidth;
        }
        catch (json::exception &e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }

        // Read and parse layers

        Path mapPath;
        if (!findFileByPath(path, &mapPath, ".tmj")) {
            return nullptr;
        }

        std::ifstream map_fStream(mapPath);
        json mapData = json::parse(map_fStream);
        map_fStream.close();

        // Actual parsing here:
        try {
            for (const auto& l : mapData["layers"]) {
                TileLayer layer {
                        .start_x = l["startx"],
                        .start_y = l["starty"],
                        .width = l["width"],
                        .height = l["height"],
                };
                std::string name = l["name"];
                layer.layer_id = std::stoi(name);

                for (auto &c : l["chunks"]) {
                    Chunk chunk {
                        .x = c["x"],
                        .y = c["y"],
                        .width = c["width"],
                        .height = c["height"]
                    };
                    for (const auto& tile : c["data"]) {
                        chunk.tileData.push_back(tile);
                    }
                    layer.chunks.push_back(chunk);
                }
                layers.push_back(layer);
            }
        }
        catch (json::exception &e) {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }

        // Image load
        Path imgPath;
        // this assumes all images are png
        if (!findFileByPath(path, &imgPath, "png")) {
            return nullptr;
        }

        // Hacky way of dealing with weird image conversion errors... oops
        SDL_Texture* text_ = IMG_LoadTexture(renderer, imgPath.c_str());
        if (text_ == nullptr) {
            std::cerr << "Error loading texture!" << std::endl;
            return nullptr;
        }
        SDL_Texture* text = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, tileSet.mapWidth * tileSet.tileWidth, tileSet.mapHeight * tileSet.tileHeight);
        if (text == nullptr) {
            std::cerr << "Error loading texture!" << std::endl;
            return nullptr;
        }
        auto old_target = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, text);
        SDL_RenderCopy(renderer, text_, nullptr, nullptr);
        SDL_SetRenderTarget(renderer, old_target);
        SDL_DestroyTexture(text_);

        // end weird dumb hack lol

        std::string img_name = imgPath.filename().string();
        img_name = img_name.substr(0, img_name.find('.'));

        // Here's the magic
        int w = -1, h = -1;

        for (const auto &layer : layers) {

            int w_ = layer.width * tileSet.tileWidth;
            int h_ = layer.height * tileSet.tileHeight;

            if (w == -1)
                w = w_;
            if (h == -1)
                h = h_;

            SDL_Texture* layer_texture = SDL_CreateTexture(renderer, PIXEL_FORMAT, SDL_TEXTUREACCESS_TARGET, w_, h_);
            // std::vector<SDL_Rect> src_rects; // only for use in certain situations
            std::vector<Tile> tile_overrides;

            // TODO: this will assume uniform chunk sizes... double check that
            int num_chunks_x = layer.width / layer.chunks.front().width;

            for (int i = 0; i < layer.chunks.size(); i++) {
                // i is chunk offset
                for (int y = 0; y < layer.chunks[i].height; y++) {

                    for (int x = 0; x < layer.chunks[i].width; x++) {

                        int real_x = ((i % num_chunks_x) * layer.chunks[i].width * tileSet.tileWidth) + (x * tileSet.tileWidth);
                        int real_y = ((i / num_chunks_x) * layer.chunks[i].height * tileSet.tileHeight) + (y * tileSet.tileHeight); // !! possible bug source
                        int tile_id = layer.chunks[i].tileData[(y * layer.chunks[i].width) + x];

                        if (tile_id == 0)
                            continue;

                        SDL_Rect src = tileSet.getSrcRectFromId(tile_id); // !! possible bug source
                        SDL_Rect dest = {
                                .x = real_x,
                                .y = real_y,
                                .w = src.w,
                                .h = src.h
                        };
                        if (layer.layer_id >= 3 && layer.layer_id <= 5) {
                            SDL_FRect dest_override = {
                                    .x = static_cast<float>(real_x),
                                    .y = static_cast<float>(real_y),
                                    .w = static_cast<float>(src.w),
                                    .h = static_cast<float>(src.h)
                            };
                            tile_overrides.push_back({src, dest_override});
                        }
                        SDL_SetRenderTarget(renderer, layer_texture);
                        SDL_RenderCopy(renderer, text, &src, &dest);
                    }
                }
            }
            RawMapLayerData rmld{dirName, layer.layer_id, layer_texture};
            if (layer.layer_id >= 3 && layer.layer_id <= 5) {
                // rmld.srcRects = std::move(src_rects);
                rmld.tileOverrides = std::move(tile_overrides);
            }
            data.push_back(rmld);
        }
        SDL_SetRenderTarget(renderer, nullptr);

        return new RawMapData{std::move(data), text, img_name, w, h};
    }
}

#endif //RPG_MAPLOADER_HPP
