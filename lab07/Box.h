#ifndef BOX_H
#define BOX_H

#include <glm/glm.hpp>

class Drawable;

/**
 * Represents the bounding box
 */
class Box {
public:
    Drawable* cube;
    glm::mat4 modelMatrix;
    glm::vec3 scaleFactor=glm::vec3(5.0f, 0.3f, 20.0f);
    glm::vec3 p;
    float size;
    Box(glm::vec3 position,float s);
    ~Box();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif
