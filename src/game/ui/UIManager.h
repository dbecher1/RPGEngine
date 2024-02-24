//
// Created by Daniel Becher on 2/24/24.
//

#ifndef RPG_UIMANAGER_H
#define RPG_UIMANAGER_H

#include "UIElement.h"
#include <map>

class SpriteBatch;

/**
 * Class that will accomplish the following two goals:
 * 1. Be an abstraction for a collection of UI elements
 * 2. Be a state manager for those elements
 */
class UIManager {
public:
    UIManager();
    void Update(double dt);
    void ToggleState(const std::string& name);
    void Draw(SpriteBatch* sb);
private:
    std::map<std::string, UIElement> elements;
};


#endif //RPG_UIMANAGER_H
