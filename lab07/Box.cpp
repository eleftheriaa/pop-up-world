#include "Box.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>

using namespace glm;

Box::Box(vec3 position,float s) {
    p = position;
    size = s;
    cube = new Drawable("models/cube.obj");
    
}

Box::~Box() {
    delete cube;
}

void Box::draw(unsigned int drawable) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    cube->bind();
    cube->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_CULL_FACE);
}

void Box::update(float t, float dt) {
    mat4 translation = translate(mat4(), vec3(p.x, p.y, p.z));
    //mat4 scaling = glm::scale(mat4(), vec3(size, size, size));
    vec3 scaleFactor = vec3(5.0f, 0.3f, 20.0f);
    mat4 scaling = scale(mat4(), scaleFactor);
    modelMatrix = translation*scaling;
}