#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
public:
    GLFWwindow* window;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    // Initial position : on +Z
    glm::vec3 position;
    // Initial horizontal angle : toward -Z
    float horizontalAngle;
    // Initial vertical angle : none
    float verticalAngle;
    // Field of View
    float FoV;
    float speed; // units / second
    float mouseSpeed;
    float fovSpeed;
    float deltaTime;
    bool cartoonLike;
    bool Tpressed;
    bool Onepressed;

    glm::vec3 direction;


    Camera(GLFWwindow* window);
    void update();
    void startTransition();
    void FirstPerson();
    void switchProjection(bool);
};

#endif
