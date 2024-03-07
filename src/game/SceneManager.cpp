//
// Created by Daniel Becher on 2/7/24.
//

#include "SceneManager.h"

#include "scenes/BattleScene.h"
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

    /*
    SceneStack.back()->Deactivate();
    BattleBuilder bb;
    bb.party[0] = player;
    bb.party_size++;
    bb.background = *resourceManager->getEnvironment("Forest");

   SceneStack.push_back(new BattleScene(bb));
    */
    uiManager = new UIManager(resourceManager);
    spriteBatch->setCameraBoundaries(s.getCurrentWorldBoundaries());
    // GlobalState::CurrentGlobalState = BATTLE;
    GlobalState::CurrentGlobalState = OVERWORLD;
    RPG::Event e;
    e.type = RPG::BATTLE_START;
    eventPoller.Push(e);
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
                GlobalState::PreviousGlobalState = GlobalState::CurrentGlobalState;
                GlobalState::CurrentGlobalState =  BATTLE;

                BattleBuilder bb;
                bb.party[0] = resourceManager->getEntity("Celes");
                bb.party_size++;
                bb.background = *resourceManager->getEnvironment("Forest");

                SceneStack.back()->Deactivate();
                SceneStack.push_back(new BattleScene(bb));

                auto mid = uiManager->getElementByName("BattleMid");
                TextBuilder tb;

                // this is where we'd get the entity info
                tb.text_raw = "Morbol";
                tb.font = uiManager->getDefaultFont();
                tb.alignment_x = X_ALIGN_CENTER;
                tb.alignment_y = Y_ALIGN_TOP;
                tb.size = 16;
                mid->addText(tb);

                break;
            }
            case RPG::BATTLE_END: {
                GlobalState::CurrentGlobalState = GlobalState::PreviousGlobalState;
                GlobalState::PreviousGlobalState =  BATTLE;

                auto ptr = SceneStack.back();
                SceneStack.pop_back();
                SceneStack.back()->Activate();
                delete ptr;
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
        if (scene->isActive()) scene->Draw(spriteBatch);
    }
    uiManager->Draw(spriteBatch);
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

