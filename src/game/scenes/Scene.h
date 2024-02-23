//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_SCENE_H
#define RPG_SCENE_H

#include <vector>
#include "../entities/Entity.h"
#include "../world/Maps.h"

struct SceneBuilder {
    std::string name;
    std::string map_name;
    std::vector<std::string> entities;
    std::vector<Entity*> entities_by_ptr;
    bool is_active;
};

class ResourceManager;

// TODO: Make scene into an abstract class with inheritance

class Scene {
public:
    Scene(const SceneBuilder& sb, ResourceManager* rm);
    void Update(double dt);
    void FixedUpdate();
    void AddEntity(const std::string& name);
    void Draw(SpriteBatch* sb) const;
private:
    std::string sceneName;
    bool isActive;
    std::vector<Entity*> activeEntities;
    Maps* map;
};


#endif //RPG_SCENE_H
