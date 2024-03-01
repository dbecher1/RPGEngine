//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_OVERWORLDSCENE_H
#define RPG_OVERWORLDSCENE_H

#include <vector>
#include "../world/Maps.h"
#include "Scene.h"

struct SceneBuilder {
    std::string name;
    std::string map_name;
    std::vector<std::string> entities;
    std::vector<GlobalEntity*> entities_by_ptr;
    bool is_active;
};

class ResourceManager;

class OverworldScene : public Scene {
public:
    OverworldScene(const SceneBuilder& sb, ResourceManager* rm);
    void Update(double dt) override;
    void FixedUpdate() override;
    void AddEntity(const std::string& name);
    void Draw(SpriteBatch* sb) const override;
    SDL_Point getCurrentWorldBoundaries();
private:
    //std::string sceneName;
    bool isActive;
    std::vector<GlobalEntity*> activeEntities;
    Maps* map;
};


#endif //RPG_OVERWORLDSCENE_H
