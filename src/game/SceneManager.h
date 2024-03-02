//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_SCENEMANAGER_H
#define RPG_SCENEMANAGER_H

#include <deque>
#include "scenes/OverworldScene.h"
#include "ui/UIManager.h"
#include "ResourceManager.h"
#include "events/EventPoller.h"

// UI goes here??

class SceneManager {

public:
    explicit SceneManager(SDL_Window* window);
    ~SceneManager();
    void Update(double dt);
    void FixedUpdate() const;
    void Draw() const;

    void windowResizeEvent(int w, int h) const;
    void resetDefaultWindowSize(SDL_Window* window) const;
private:
    EventPoller eventPoller{};
    RPG::Event e;
    std::deque<Scene*> SceneStack;
    SpriteBatch* spriteBatch;
    ResourceManager* resourceManager;
    UIManager* uiManager;
    GlobalEntity* player;
};


#endif //RPG_SCENEMANAGER_H
