//
// Created by Daniel Becher on 2/24/24.
//

#include "UIManager.h"
#include "../SpriteBatch.h"

UIManager::UIManager(const ResourceManager* resourceManager) {

    default_font = Font(resourceManager);

    SDL_FRect r{0.0125f, 0.0125f, 0.25f, 0.5f};
    UIElementBuilder eb = {r, "pause"};
    eb.is_active = true;
    //elements.emplace(eb.name, eb);

    UIElementBuilder eb2 = {
            {0.1f, 0.6, 0.8f, 0.37f},
            "dialog",
            0.01f,
    };
    eb2.is_active = true;
    TextBuilder tb {&default_font, "Hi? Is this thing on? Test!", 24};
    tb.alignment_x = X_ALIGN_LEFT;
    tb.alignment_y = Y_ALIGN_TOP;
    tb.offset.y = 20;
    eb2.text_builders.push_back(tb);

    UIElementBuilder titleBox = {
    {0, -0.25f, 0.2f, 0.25f},
        "dialog_name",
        0.01f
    };
    titleBox.is_active = true;
    TextBuilder tb2 = {&default_font, "Jill", 16, X_ALIGN_CENTER, Y_ALIGN_MIDDLE};
    titleBox.text_builders.push_back(tb2);
    eb2.nodes.push_back(titleBox);
    elements.emplace(eb2.name, eb2);

}

void UIManager::Update(double dt) {
    // TODO
}

void UIManager::Draw(SpriteBatch* sb) {
    for (auto&[_, snd] : elements) {
        if (snd.isActive()) {
            //sb->Add(&snd);
            snd.Draw(sb);
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
