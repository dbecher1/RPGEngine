//
// Created by Daniel Becher on 2/7/24.
//

#include "SceneManager.h"
#include "state/GlobalState.h"

double idk = 0;

SceneManager::SceneManager(SDL_Window* window) {

    resourceManager = new ResourceManager();

    SpriteBatchBuilder sb {window, resourceManager};
    spriteBatch = new SpriteBatch(sb);

    resourceManager->loadAllResources(spriteBatch->renderer);
    spriteBatch->atlas = resourceManager->getAtlas();

    SceneBuilder sceneBuilder{"demo", "demo"};
    //sceneBuilder.entities.emplace_back("Character");
    player = resourceManager->getEntity("Celes");
    sceneBuilder.entities_by_ptr.push_back(player);
    OverworldScene s{sceneBuilder, resourceManager};
    SceneStack.push_back(new OverworldScene(s));

    uiManager = new UIManager();
    // gameState = GlobalState::GetGlobalState();

    spriteBatch->setCameraBoundaries(s.getCurrentWorldBoundaries());
}

SceneManager::~SceneManager() {
    for (auto s : SceneStack) {
        delete s;
    }
    delete uiManager;
    delete resourceManager;
    delete spriteBatch;
}

void SceneManager::Update(double dt) {

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

    idk += dt;
    if (idk > 1) {
        idk = 0;
        uiManager->ToggleState("pause");
        uiManager->ToggleState("dialog");
    }
}

void SceneManager::Draw() {
    for (const auto &scene : SceneStack) {
        scene->Draw(spriteBatch);
    }
    //uiManager->Draw(spriteBatch);
    spriteBatch->SubmitDraw();
}

void SceneManager::windowResizeEvent(int w, int h) {
    spriteBatch->windowResizeEvent(w, h);
}

void SceneManager::resetDefaultWindowSize(SDL_Window *window) {
    spriteBatch->resetDefaultWindowSize(window);
}

void SceneManager::FixedUpdate() {
    SceneStack.back()->FixedUpdate();
}

