//
// Created by Daniel Becher on 2/7/24.
//

#ifndef RPG_SCENEMANAGER_H
#define RPG_SCENEMANAGER_H

#include <deque>
#include "scenes/OverworldScene.h"
#include "ui/UIManager.h"
#include "ResourceManager.h"
#include "SpriteBatch.h"
#include "GlobalState.h"

// UI goes here??

class SceneManager {

public:
    explicit SceneManager(SDL_Window* window);
    ~SceneManager();
    void Update(double dt);
    void FixedUpdate();
    void Draw();

    void windowResizeEvent(int w, int h);
    void resetDefaultWindowSize(SDL_Window* window);
private:
    std::deque<Scene*> SceneStack;
    SpriteBatch* spriteBatch;
    ResourceManager* resourceManager;
    UIManager* uiManager;
    Entity* player;
    GlobalState* gameState;
};


#endif //RPG_SCENEMANAGER_H
