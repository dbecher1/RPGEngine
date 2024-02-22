//
// Created by Daniel Becher on 2/7/24.
//

#include "SceneManager.h"

SceneManager::SceneManager(SDL_Window* window) {

    resourceManager = new ResourceManager();

    SpriteBatchBuilder sb {window, resourceManager};
    spriteBatch = new SpriteBatch(sb);

    resourceManager->loadAllResources(spriteBatch->renderer);
    spriteBatch->atlas = resourceManager->getAtlas();

    camera = {spriteBatch->renderer, INITIAL_WIDTH, INITIAL_HEIGHT};

    // TODO might want to... gulp... make entities a vector of...
    // ..pointers..
    SceneBuilder sceneBuilder{"demo", "demo"};
    sceneBuilder.entities.emplace_back("Character");
    Scene s{sceneBuilder, resourceManager};
    SceneStack.push_back(s);
}

SceneManager::~SceneManager() {
    delete resourceManager;
    delete spriteBatch;
}

void SceneManager::Update(double dt) {
    SceneStack.back().Update(dt);
}

void SceneManager::Draw() {
    for (const auto &scene : SceneStack) {
        scene.Draw(spriteBatch);
    }
    spriteBatch->SubmitDraw();
}

void SceneManager::windowResizeEvent(int w, int h) {
    spriteBatch->windowResizeEvent(w, h);
}

void SceneManager::resetDefaultWindowSize(SDL_Window *window) {
    spriteBatch->resetDefaultWindowSize(window);
}

void SceneManager::FixedUpdate() {
    SceneStack.back().FixedUpdate();
}

