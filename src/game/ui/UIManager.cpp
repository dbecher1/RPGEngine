//
// Created by Daniel Becher on 2/24/24.
//

#include "UIManager.h"
#include "../SpriteBatch.h"

UIManager::UIManager(const ResourceManager* resourceManager) {
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
    default_font = Font(resourceManager);
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
    auto demo_text = default_font.GenerateText("Hello!", 32, {20, 100});
    auto demo_text2 = default_font.GenerateText("Hello?", 32, {500, 500});
    sb->Add(demo_text);
    sb->Add(demo_text2);
}

/**
 * This will eventually be fleshed out into a more robust state changing method.
 * Right now I'll just be using it to set states as active or not.
 * @param name The name of UI element to toggle.
 */
void UIManager::ToggleState(const std::string &name) {
    elements.at(name).setActiveState(!elements.at(name).isActive());
}
