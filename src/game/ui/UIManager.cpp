//
// Created by Daniel Becher on 2/24/24.
//

#include "UIManager.h"
#include "../SpriteBatch.h"

UIManager::UIManager() {
    SDL_FRect r{0.0125f, 0.0125f, 0.25f, 0.5f};
    UIElementBuilder eb = {r, "pause"};
    eb.color = {125, 125, 150, 255};
    eb.is_active = true;
    elements.emplace(eb.name, eb);

    UIElementBuilder eb2 = {
            {0.1f, 0.6, 0.8f, 0.37f},
            "dialog",
            0.01f,
    };
    eb2.is_active = true;
    elements.emplace(eb2.name, eb2);
}

void UIManager::Update(double dt) {
    // TODO
}

void UIManager::Draw(SpriteBatch* sb) {
    for (auto&[_, snd] : elements) {
        if (snd.isActive()) {
            sb->Add(&snd);
        }
    }
}

/**
 * This will eventually be fleshed out into a more robust state changing method.
 * Right now I'll just be using it to set states as active or not.
 * @param name The name of UI element to toggle.
 */
void UIManager::ToggleState(const std::string &name) {
    elements.at(name).setActiveState(!elements.at(name).isActive());
}
