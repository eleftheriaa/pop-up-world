#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "RigidBody.h"

class Drawable;

class Mushroom : public RigidBody {
public:
    Drawable* mushroom;
    float l;
    glm::mat4 modelMatrix;

    Mushroom(glm::vec3 pos, glm::vec3 vel, glm::vec3 omega, float length, float mass);
    ~Mushroom();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif