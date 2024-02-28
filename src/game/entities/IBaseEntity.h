//
// Created by Daniel Becher on 2/27/24.
//

#ifndef BASEENTITY_H
#define BASEENTITY_H

struct DrawCommand;

class IBaseEntity {
public:
    virtual ~IBaseEntity() = default;
    virtual void Update(double dt) = 0;
    virtual void Draw(DrawCommand* dc) = 0;
    virtual void FixedUpdate() = 0;
};

#endif //BASEENTITY_H
