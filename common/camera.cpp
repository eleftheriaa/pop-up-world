#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;


// ------ values for 3d - 2d transition ------ 
bool transitioning = false;
float theta = 0.0f; // Current rotation angle for the transition
float T = 1.0f; // time theta takes to go from 0 to pi/2
float transitionSpeed =3.14f/2*T; // Speed of rotation (radians per second)
int radious = 15;
float a = 2.0f;
// go back
bool reverseTransitioning = false;
bool tKeyPressed = false;

// number of times the key is pressed
int sevenPressed = 1;
bool sevenKeyPressed = false;
// ------ values for first Person View ------
bool firstPersonview = false;
float h = 2.2f + 0.29f; // distance between the ground & cat's eyes and the ground from 0
float phi = 0.0f;
float deltaz = 0.0f;
float flag = 0.0f;




void Camera::startTransition() {
    if (!transitioning) {
        transitioning = true;
        //theta = 0.0f; // Reset the rotation angle   
    }
}
Camera::Camera(GLFWwindow* window) : window(window) {
    position = vec3(0,1, 60);
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;
    FoV = 45.0f;
    speed = 5.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 0.1f;
    cartoonLike = false;
    Tpressed = false;
    Onepressed = false;
}
void Camera::FirstPerson() {
    if (!firstPersonview) {
        firstPersonview = true;
        
    }
}
void Camera::update() {
    

    // Compute time difference
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
    ////Get mouse position
    //double xPos, yPos;
    //glfwGetCursorPos(window, &xPos, &yPos);

    //int width, height;
    //glfwGetWindowSize(window, &width, &height);

    //// Reset mouse position for next frame
    //glfwSetCursorPos(window, width / 2, height / 2);

    //horizontalAngle += mouseSpeed * float(width / 2 - xPos);
    //verticalAngle += mouseSpeed * float(height / 2 - yPos);

    // 
    vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    
    vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );
    
    vec3 up = cross(right, direction);

    //// Move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&  Tpressed == false ) {
        position += direction * deltaTime * speed;
    }
    //// Move backward
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //    position -= direction * deltaTime * speed;
    //}
    //// Strafe right
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //    position += right * deltaTime * speed;
    //}
    //// Strafe left
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //    position -= right * deltaTime * speed;
    //}
    // Task 5.6: handle zoom in/out effects
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        FoV -= fovSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        FoV += fovSpeed;
    }

    viewMatrix = lookAt(
        position,
        position + direction,
        up
    );
    // projectionmatrix without the key press
    float aspectRatio = 4.0f / 3.0f;
    if (flag == 0) {
        projectionMatrix = perspective(radians(FoV), aspectRatio, 0.1f, 1000.0f);
    }
    else if (flag == 1) {
        projectionMatrix = ortho(-15.0f * aspectRatio, 15.0f * aspectRatio, -15.0f, 15.0f, -position.z -250.0f , position.z + 250.0f);
    }

    else if(flag == 2)  {
        projectionMatrix = ortho(-8.0f * aspectRatio, 8.0f * aspectRatio, -8.0f, 8.0f, -0.2f + position.z, 250.0f );
       
}
    
    // 3d -2d
    if (transitioning) {
        Tpressed = true;
        //cartoonLike = true;
        // Increment rotation angle based on transition speed
        theta +=  transitionSpeed * deltaTime;
        std::cout << "rotationAngle in rad: " << theta << endl;

        vec3 position_new = vec3(position.x + a * radious * sin(theta), position.y - (position.y) * theta / (3.14f/2.0f), position.z - radious + radious * cos(theta));
        /*
        vec4 vec4right = rotate(mat4(), theta, vec3(0.0f, 1.0f, 0.0f)) * vec4(right, 1.0f);
        right = vec3(vec4right.x, vec4right.y, vec4right.z);
*/
        direction = -position_new + vec3(position.x, position.y - (position.y) * theta / (3.14f / 2.0f), position.z - radious);
        right = cross(direction, up);
        viewMatrix = lookAt(
            position_new,
            position_new + direction,
            up
        );

        // Check if the rotation is complete
        if (theta >= 3.14f/2.0f) {
            theta = 3.14f / 2.0f;
            position = position_new;
            horizontalAngle =- 3.14f / 2.0f ;
            transitioning = false;
            cartoonLike = true;
            flag = 1.0f;
            
        }
    
    }
    // fix motion of camera after the 2d transition
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && Tpressed == true) {
        position += right * deltaTime * speed;
    }
    // 2D - 3D Transition
    if (reverseTransitioning) {
        flag = 0.0f;
        //Tpressed = false;
        //
        // cartoonLike = false;

        // Decrement rotation angle based on transition speed
        theta -= transitionSpeed * deltaTime;

        std::cout << "rotationAngle in rad (reverse): " << theta << std::endl;
        std::cout << "X_camra " << position.x << std::endl;
        std::cout << "Y_camra" << position.y << std::endl;
        std::cout << "Z_camra " << position.z << std::endl;
        vec3 position_back = vec3(position.x - a * radious *(1 - sin(theta)),
            position.y + (1 - theta / (3.14f / 2.0f)) * 5,
            position.z + radious * cos(theta));

        direction = -position_back + vec3(position.x - a*radious,
            position.y + (1 - theta / (3.14f / 2.0f)) * 5,
            position.z );
        right = cross(direction, up);

        viewMatrix = lookAt(position_back, position_back + direction, up);
        std::cout << "X_camra_back " << position_back.x << std::endl;
        std::cout << "Y_camra_back" << position_back.y << std::endl;
        std::cout << "Z_camra_back " << position_back.z << std::endl;

        // Stop transition at 3D mode
        if (theta <= 0.0f) {
            theta = 0.0f;
            position = position_back;
           
            horizontalAngle = 3.14f;
            //verticalAngle = 0.0f;

            reverseTransitioning = false;
        }
    }
   
    // TASK 7 

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        if (!sevenKeyPressed) { // Only increment when key is first pressed
            sevenPressed++;
            cartoonLike = (sevenPressed % 2 == 0); // True for even, false for odd
        }
        sevenKeyPressed = true;  // Mark key as held
    }
    else {
        sevenKeyPressed = false; // Reset flag when key is released
    }
    /*if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        sevenPressed += 1;
        if (sevenPressed % 2 != 0) {
            cartoonLike = true;
        }
        else{
            cartoonLike = false;
        }
    }*/
    // TASK 4 (3D -> 2D)
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        startTransition();
    }
    // TASK 4 (2D -> 3D)
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        Tpressed = false;

        if (!reverseTransitioning) {
            reverseTransitioning = true;
        }
    }
    
    // TASK 8 (3D - 1 PERSON VIEW / ORTHO PROJ)
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        Onepressed = true;
        if (!firstPersonview) {
            firstPersonview = true;

        }

        //FirstPerson();


    }


    // Store current time for the next frame
    lastTime = currentTime;
}
