#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
    float distance; // Distance field for sprite sorting

    TransformComponent(glm::vec2 position = glm::vec2(30.0, 20.0), glm::vec2 scale = glm::vec2(1.0, 1.0), double rotation = 0.0) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
        this->distance = 0.0;
    }
};


#endif