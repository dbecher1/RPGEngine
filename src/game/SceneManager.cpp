//
// Created by Daniel Becher on 2/7/24.
//

#include "SceneManager.h"
#include "state/GlobalState.h"

double idk = 0;

SceneManager::SceneManager(SDL_Window* window) {

    resourceManager = new ResourceManager();

    spriteBatch = new SpriteBatch({window, resourceManager});

    resourceManager->loadAllResources(spriteBatch->renderer);
    spriteBatch->atlas = resourceManager->getAtlas();

    SceneBuilder sceneBuilder{"demo", "demo"};
    player = resourceManager->getEntity("Celes");
    sceneBuilder.entities_by_ptr.push_back(player);
    const OverworldScene s{sceneBuilder, resourceManager};
    SceneStack.push_back(new OverworldScene(s));

    uiManager = new UIManager();

    spriteBatch->setCameraBoundaries(s.getCurrentWorldBoundaries());
}

SceneManager::~SceneManager() {
    for (const auto &s : SceneStack) {
        delete s;
    }
    delete uiManager;
    delete resourceManager;
    delete spriteBatch;
}

void SceneManager::Update(const double dt) {

    while (eventPoller.Poll(&e)) {
        switch (e.type) {
            default: break;
            case RPG::BATTLE_START: {
                // SceneStack.push_back({});
                break;
            }
        }
    }

    GlobalState::GameTime_Tick(dt);
    SceneStack.back()->Update(dt);
    // spriteBatch->Update(player->getDrawOffset()); // TODO

}

void SceneManager::Draw() const {
    for (const auto &scene : SceneStack) {
        scene->Draw(spriteBatch);
    }
    // uiManager->Draw(spriteBatch);
    spriteBatch->SubmitDraw();
}

void SceneManager::windowResizeEvent(const int w, const int h) const {
    spriteBatch->windowResizeEvent(w, h);
}

void SceneManager::resetDefaultWindowSize(SDL_Window *window) const {
    spriteBatch->resetDefaultWindowSize(window);
}

void SceneManager::FixedUpdate() const {
    SceneStack.back()->FixedUpdate();
}

