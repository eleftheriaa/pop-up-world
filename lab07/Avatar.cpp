#include "Avatar.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>
#include <glfw3.h>
#include <common/camera.h>
//#include "RigidBody.h"


using namespace std;
using namespace glm;

// JUMP STATE
float velocity = 0.0f;    // Vertical velocity of the avatar
float gravity = -9.8f;    // Gravity force pulling the avatar down
float jumpStrength =5.0f; // Strength of the jump
bool isJumping = false;   // To track if the avatar is currently jumping
bool onGround = true;
bool jumpKeyPressed = false;

float avatarGroundLevel = 0.29f; 

Avatar::Avatar(vec3 pos, vec3 vel, float radius, float mass)
    : RigidBody() {
    //avatar = new Drawable("models/pose_cat.obj");

    // Load both poses
    avatar = new Drawable("models/cat_armature.obj");
    //pose2 = new Drawable("models/cat_armature2.obj");
    //currentPose = pose1;

    r = radius;
    m = mass;
    x = pos;
    v = vel;
    P = m * v;

    if (radius == 0) throw std::logic_error("Avatar: radius != 0");
    mat3 I = mat3(
        2.0f / 5 * mass * radius * radius, 0, 0,
        0, 2.0f / 5 * mass * radius * radius, 0,
        0, 0, 2.0f / 5 * mass * radius * radius);
    I_inv = inverse(I);
}

Avatar::~Avatar() {
    delete avatar;  
    //delete pose2;  // Deleting pose2, which was created with new
}

void Avatar::draw(unsigned int drawable) {
    avatar->bind();
    float groundlevel = 0.0f;
    avatar->draw();
}

/*
bool isOnGround() {
    return (x.y <= 0.0f);
}*/
void Avatar::update(float t, float dt) {
    //integration
    advanceState(t, dt);
    // compute model matrix
    
    //mat4 scale = glm::scale(mat4(), vec3(0.07f, 0.07f, 0.07f));
    mat4 translation = translate(mat4(), vec3(x.x, x.y, x.z));
#ifdef USE_QUATERNIONS

    //mat4 rotation = rotate(mat4(1.0f), 3.14f, vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), 3 * 3.14f / 2, vec3(1.0f, 0.0f, 0.0f));
    mat4 rotation_POSE = rotate(mat4(1.0f), 3.14f, vec3(0.0f, 1.0f, 0.0f));
#else
    mat4 rotation = rotate(mat4(1.0f), 3.14f, vec3(0.0f, 1.0f, 0.0f)) * rotate(mat4(1.0f), 3 * 3.14f / 2, vec3(1.0f, 0.0f, 0.0f));
#endif
    modelMatrix = translation * rotation_POSE;
    //----------------ADJUST_PLAYER_POSITION--------------------------

    extern GLFWwindow* window;
    extern Camera* camera;
    if (!isJumping) {
        // Start the jump
        if (glfwGetKey(camera->window, GLFW_KEY_SPACE) == GLFW_PRESS ||(glfwGetKey(camera->window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(camera->window, GLFW_KEY_SPACE) == GLFW_PRESS) ) {
            if (!jumpKeyPressed && onGround) {
                velocity = jumpStrength;
                isJumping = true;
                onGround = false;
            }
            jumpKeyPressed = true; // Mark the key as being pressed
        }
        else {
            jumpKeyPressed = false; // Reset the key press state when released
        }

    }
    if (isJumping) {
        velocity += gravity * dt; // Adjust velocity based on gravity
        x.y += velocity * dt; // Update position based on velocity

        // Check if avatar reaches the ground
        // assume m is the height of the ground
        if (x.y  < avatarGroundLevel) {
            x.y = avatarGroundLevel;   // Set to ground level
            velocity = 0.0f;     // Stop downward velocity
            isJumping = false;   // Stop the jump
            onGround = true;
        }
    }
    
}