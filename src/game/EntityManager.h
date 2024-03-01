//
// Created by Daniel Becher on 2/27/24.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <string>

class IBaseEntity;
class SpriteBatch;

class EntityManager {
public:
    EntityManager() = default;
    ~EntityManager();

    void AddEntity(const std::string& name, IBaseEntity* entity);
    void DestroyEntity(const std::string& name);
    void ActivateEntity(const std::string& name);
    void DeactivateEntity(const std::string& name);

    void Update(double dt);
    void Draw(SpriteBatch* sb);
private:
    std::map<std::string, IBaseEntity*> Entities;
};



#endif //ENTITYMANAGER_H
