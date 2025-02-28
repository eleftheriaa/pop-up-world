#ifndef AVATAR_H
#define AVATAR_H

#include "RigidBody.h"


class Drawable;


class Avatar : public RigidBody {
public:
    Drawable* avatar;
    //Drawable* pose2;
    float r;
    glm::mat4 modelMatrix;
    Avatar(glm::vec3 pos, glm::vec3 vel, float radius, float mass);
    ~Avatar();

    //void updatePose();
    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
    

};



#endif#pragma once
