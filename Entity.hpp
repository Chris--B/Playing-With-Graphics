#pragma once

#include "CommonDefs.hpp"

class btCollisionObject;

class Entity {
public:
    void render() const;

    // TODO: I'm lazy and don't want to write a bunch of boilerplate yet
    // private:
public:
    // TODO: Some sort of graphics object with mesh data?
    std::unique_ptr<btCollisionObject> phys_obj;
};
