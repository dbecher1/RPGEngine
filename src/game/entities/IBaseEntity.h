//
// Created by Daniel Becher on 2/27/24.
//

#ifndef BASEENTITY_H
#define BASEENTITY_H

struct DrawCommand;

// Not a true interface any more but that's ok
class IBaseEntity {

protected:
    bool active{};

public:
    virtual ~IBaseEntity() = default;
    virtual void Update(double dt) = 0;
    virtual void Draw(DrawCommand* dc) = 0;
    virtual void FixedUpdate() = 0;

    void Activate() {
        active = true;
    }

    void Deactivate() {
        active = false;
    }

    [[nodiscard]] bool isActive() const {
        return active;
    }
};

#endif //BASEENTITY_H
