//
// Created by Daniel Becher on 2/7/24.
//

#include "ResourceManager.h"
#include "../tools/extern/json.hpp"
#include "../tools/MapLoader.hpp"
#include "world/Maps.h"
#include "../tools/extern/schrift.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#include "battle/ElementalAffinity.h"
#include "ui/Text.h"
#include "ui/UIElement.h"

/// TODO: probably set an upper limit on texture packing and create a way for a multi-level hierarchy of images

#define IMAGE_PATH "../resources/images/"
#define ANIMATION_PATH "../resources/animations/"
#define ENTITY_PATH "../resources/entities/"
#define ENVIRONMENT_PATH "../resources/misc/environments.json"
#define MAP_PATH "../resources/maps/"
#define FONT_PATH "../resources/fonts/"
#define UI_PATH "../resources/ui/"
#define ENEMY_PATH "../resources/enemies/"

using json = nlohmann::json;

ResourceManager::~ResourceManager() {
    // ResourceManager has ownership over its resources, so it needs to free the SDL texture
    SDL_DestroyTexture(atlas);
}

bool ResourceManager::loadAllResources(SDL_Renderer* renderer) {
    bool flag = this->loadMaps(renderer);
    flag = flag && this->loadFonts(renderer);
    loadUIElements();
    flag = flag && this->loadTextures(renderer);
    if constexpr (DEBUG_PRINT) {
        std::cout << "\n===TEXTURE RECT INFORMATION===" << std::endl;
        for (const auto &r : this->textureRects) {
            std::cout << r.first << " - ";
            std::cout << "x: " << r.second.x << ", ";
            std::cout << "y: " << r.second.y << ", ";
            std::cout << "w: " << r.second.w << ", ";
            std::cout << "h: " << r.second.h << std::endl;
        }
        std::cout << "===END TEXTURE RECT INFORMATION===\n" << std::endl;
    }
    flag = flag && this->loadEntities() && this->loadEnvironments();
    return flag;
}

/**
 * Loads map data from TMX files.
 * This should be called before image loading (in dev) as it generates new image data for the texture loader.
 * @return True if successful
 */
bool ResourceManager::loadMaps(SDL_Renderer* renderer) {

    for (const auto &p : iter_dir(MAP_PATH)) {

        if (p.is_directory()) {

            std::string mapName = p.path().filename().string();
            MapBuilder mapBuilder{
                .map_name = mapName,
            };

            auto dataRaw = MapLoader::loadRawMapData(p.path(), renderer);

            if (dataRaw == nullptr)
                continue;

            for (auto &[mapName, layer, texture, tileOverrides] : dataRaw->layerData) {

                std::string name_noExt = mapName;
                name_noExt.append(std::to_string(layer));

                if (layer >= 3 && layer <= 5) {
                    mapBuilder.map_layers.push_back({name_noExt, layer, std::move(tileOverrides)});
                }
                else {
                    auto layerFileName = createLocalFileName(name_noExt);

                    writeTexture(texture, renderer, layerFileName);

                    SDL_DestroyTexture(texture);

                    // this is where we make the map object
                    mapBuilder.map_layers.push_back({name_noExt, layer});
                }
            }
            mapBuilder.set_name = dataRaw->tileSetName;
            mapBuilder.map_width = dataRaw->width;
            mapBuilder.map_height = dataRaw->height;
            maps.emplace(mapName, mapBuilder);

            if (std::count(TEMP_fileNames.begin(), TEMP_fileNames.end(), dataRaw->tileSetName) == 0) {
                // tileset texture has not yet been loaded
                TEMP_fileNames.push_back(dataRaw->tileSetName);
                auto fileName = createLocalFileName(dataRaw->tileSetName);
                writeTexture(dataRaw->tileSet, renderer, fileName);
            }
        }
    }
    return true;
}

/**
 * Loads a font (or fonts, in the future maybe) and rasterizes them, injecting them into the "images" subfolder for loading into the image/rect pipeline.
 * @param renderer The SDL Rendering context
 * @return True on success
 */
bool ResourceManager::loadFonts(SDL_Renderer *renderer) {

    // Characters that I manually want to be oriented toward the top of their box as opposed to the bottom
    #define SWAP_CHARS(c) (c == '"' || c == '*')

    const std::string font = "PublicPixel.ttf";

    // the characters to load
    const std::string characters{FONT_CHARACTERS};

    constexpr int x_scale = 8;
    constexpr int y_scale = 8;
    constexpr int depth = 4;

    // TODO: make this load any and all fonts (maybe), right now just going to load one
    std::string path = FONT_PATH;
    path.append(font);

    auto loaded_font = sft_loadfile(path.c_str());

    SFT sft{loaded_font, x_scale, y_scale, 0, 0, 0};

    for (const auto c : characters) {
        SFT_Glyph glyph;

        // This library loads certain characters (asterisks, quotes) not oriented correctly
        // This macro (defined above) identifies them to take care of it
        if (SWAP_CHARS(c))
            sft.flags = SFT_DOWNWARD_Y;
        else
            sft.flags = 0;

        sft_lookup(&sft, c, &glyph);
        Uint8 px[x_scale * y_scale];
        const SFT_Image img{px, x_scale, y_scale};
        sft_render(&sft, glyph, img);

        std::array<Uint8, x_scale * y_scale * depth> pixels_final{};

        for (int y = 0; y < y_scale ; y++) {
            for (int x = 0; x < x_scale; x++) {
                const auto ptr = static_cast<Uint8*>(img.pixels);
                const int idx = (y * x_scale) + x;
                const Uint8 val = *(ptr + idx);

                // Few things here: 1, making this into a 32 bit image by manually copying the pixels
                // Have to semi-manually set the alpha value based on the pixel data
                // We also have to manually flip the pixels on the y axis

                for (int i = 0; i < depth; i++) {
                    int final_idx;

                    if (SWAP_CHARS(c))
                        final_idx = i + (idx * depth);
                    else
                        final_idx = i + ((((y_scale - y - 2) * x_scale) + x) * depth);

                    pixels_final[final_idx] = val;
                }
            }
        }

        // Adjust for a 1 pixel buffer that this library creates
        const auto surf = SDL_CreateRGBSurfaceFrom(&pixels_final, img.width - 1, img.height - 1, 8 * depth, img.width * depth, 0xFF000000, 0xFF0000, 0xFF00, 0xFF);
        // rest of the game uses BGRA so it'll complain unless we convert
        const auto surf_ = SDL_ConvertSurfaceFormat(surf, PIXEL_FORMAT, 0);

        std::string fName = IMAGE_PATH;
        fName.append("font/");

        fName.append(std::string{c});
        if (c >= 'a' && c <= 'z') fName.append("_");
        fName.append(".png");

        IMG_SavePNG(surf_, fName.c_str());

        SDL_FreeSurface(surf);
        SDL_FreeSurface(surf_);
    }
    sft_freefont(loaded_font);

    return true;
}

SDL_Rect ResourceManager::getRectFromTextureName(const std::string &fileName) const {
    if (this->textureRects.count(fileName) == 0)
        return {};
    return this->textureRects.at(fileName);
}

bool ResourceManager::loadTextures(SDL_Renderer* renderer) {
    ImagePacker::loadImages(renderer, this, IMAGE_PATH);
    return true;
}

bool ResourceManager::loadEntities() {

    for (const auto& file : iter_dir(ENTITY_PATH)) {

        // Downsides to macosx development lol
        if (file.path().filename().string()[0] == '.') continue;

        std::ifstream istream(file);
        json manifest = json::parse(istream);
        istream.close();

        try {
            std::string affinity_;
            std::string name;
            float moveSpeed;
            int z;

            name = manifest["Name"];
            affinity_ = manifest["Element"];
            ElementalAffinity affinity = getTypeFromString(affinity_);
            // TODO: Attributes

            moveSpeed = manifest["Overworld"]["MoveSpeed"];
            z = manifest["Overworld"]["Z"];
            std::string dir = manifest["Directory"];

            AnimationStateMachine overworld(dir), battle(dir);

            for (auto& anim : manifest["Overworld"]["Animations"]) {
                std::string anim_name = anim["Name"];
                // The texture rect is stored with the identifier, so we need to reconstruct that
                std::string rect_name = anim_name;
                rect_name.append("_").append(dir);
                auto [_, _a, w, h] = textureRects[rect_name];
                int framesX = anim["FramesX"];
                int framesY = anim["FramesY"];
                double speed = anim["Speed"];
                overworld.AddAnimation(anim_name, Animation{framesX, framesY, speed, {static_cast<float>(w), static_cast<float>(h)}});
            }

            for (auto& anim : manifest["Battle"]["Animations"]) {
                std::string anim_name = anim["Name"];
                // The texture rect is stored with the identifier, so we need to reconstruct that
                std::string rect_name = anim_name;
                rect_name.append("_").append(dir);
                auto [_, _a, w, h] = textureRects[rect_name];
                int framesX = anim["FramesX"];
                int framesY = anim["FramesY"];
                double speed = anim["Speed"];
                battle.AddAnimation(anim_name, Animation{framesX, framesY, speed, {static_cast<float>(w), static_cast<float>(h)}});
            }

            EntityBuilder eb{
                name, moveSpeed, true, z, true, overworld, battle};
            if (manifest.contains("IsPlayer") && manifest["IsPlayer"]) {
                eb.is_player = true;
            }
            //Entities[name] = GlobalEntity(eb);
            Entities.emplace(name, eb);
        } catch (json::type_error &e) {
            std::cerr << e.what() << std::endl;
        } catch (json::parse_error &e) {
            std::cerr << e.what() << std::endl;
        }

    }
    return true;
}

bool ResourceManager::loadEnemies() {
    for (const auto& entry : iter_dir(ENEMY_PATH)) {
        if (!entry.is_regular_file()) continue;

        std::ifstream istream(entry);
        json manifest = json::parse(istream);
        istream.close();

        try {
            Enemy enemy;
            enemy.name = manifest["Name"];
            // TODO everything else lol

            enemies[enemy.name] = enemy;
        } catch (json::type_error &e) {
            std::cerr << e.what() << std::endl;
        } catch (json::parse_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return true;
}

bool ResourceManager::loadAnimations() {
    // TODO
    return false;
}

// TODO: may be deleted (probably)
std::unordered_map<std::string, GlobalEntity> *ResourceManager::getEntities() {
    return &Entities;
}

bool ResourceManager::loadEnvironments() {

    std::ifstream istream(ENVIRONMENT_PATH);
    json manifest = json::parse(istream);
    istream.close();

    for (const auto& env : manifest) {
        EnvironmentBuilder eb;
        try {
            eb.name = env["Name"];
            if (env.contains("SpriteName")) {
                eb.use_spriteName = true;
                eb.sprite_name = env["SpriteName"];
            }
            eb.z = env["Z"];
        } catch (json::type_error &e) {
            std::cerr << e.what() << std::endl;
        } catch (json::parse_error &e) {
            std::cerr << e.what() << std::endl;
        }
        environments.emplace(eb.name, eb);
    }

    return true;
}

bool ResourceManager::loadUIElements() {

    for (const auto& entry : iter_dir(UI_PATH)) {
        if (entry.is_regular_file()) {
            std::ifstream istream(entry);
            json manifest = json::parse(istream);
            istream.close();

            try {
                auto* uieb = new UIElementBuilder();

                SDL_FRect rect;
                rect.x = manifest["Location"][0];
                rect.y = manifest["Location"][1];
                rect.w = manifest["Location"][2];
                rect.h = manifest["Location"][3];
                uieb->Location = rect;

                std::string name = manifest["Name"];
                uieb->name = name;
                float curve = manifest["Curve"];
                uieb->curve = curve;

                if (manifest.contains("Text")) {
                    for (const auto& text : manifest["Text"]) {
                        TextBuilder tb;
                        tb.text_raw  = text["Text"];
                        tb.size = text["Size"];
                        tb.alignment_x = text["XAlignment"];
                        tb.alignment_y = text["YAlignment"];

                        if (text.contains("OriginOffset")) {
                            // TODO
                        }
                        uieb->text_builders.push_back(tb);
                    }
                }
                if (manifest.contains("Children")) {
                    // TODO: make this recursive
                }

                UIElements.push_back(uieb);
            } catch (json::type_error &e) {
                std::cerr << e.what() << std::endl;
            } catch (json::parse_error &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    return true;
}

Environment *ResourceManager::getEnvironment(const std::string &name) {
    return (environments.count(name) > 0) ? &environments.at(name) : nullptr;
}

// Shout out to some bloke on stackoverflow for this main idea
void ResourceManager::writeTexture(SDL_Texture *texture, SDL_Renderer *renderer, const std::string &name) {
    SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

    SDL_SetRenderTarget(renderer, texture);
    int w, h;
    Uint32 f;
    SDL_QueryTexture(texture, &f, nullptr, &w, &h);
    std::cout << name << " " << SDL_GetPixelFormatName(f) << std::endl;

    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, PIXEL_FORMAT);
    SDL_RenderReadPixels(renderer, nullptr, 0, surf->pixels, surf->pitch);

    IMG_SavePNG(surf, name.c_str());
    SDL_FreeSurface(surf);

    SDL_SetRenderTarget(renderer, prev_target);
}

Maps *ResourceManager::getMap(const std::string &name) {
    return (maps.count(name) > 0) ? &maps.at(name) : nullptr;
}

/**
 * Takes in a file name (as of now assumed to be a .png image with no extension) and returns a full path
 * If I want to make this more robust, I'll implement an enum
 * @param name The file name
 * @return A string representation of the path to save the file at
 */
std::string ResourceManager::createLocalFileName(const std::string &name) {
    std::string out = IMAGE_PATH;
    out.append(name);
    out.append(".png");
    return out;
}

SDL_Texture *ResourceManager::getAtlas() const {
    return atlas;
}

GlobalEntity *ResourceManager::getEntity(const std::string &name) {
    return &Entities.at(name);
}

Enemy ResourceManager::getEnemy(const std::string &name) {
    return enemies[name];
}





