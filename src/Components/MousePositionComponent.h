#ifndef MOUSEPOSITIONCOMPONENT_H
#define MOUSEPOSITIONCOMPONENT_H

#include <glm/glm.hpp>

struct MousePositionComponent {
    glm::vec2 position;
    glm::vec2 lastPosition;

    MousePositionComponent(glm::vec2 position = glm::vec2(0.0, 0.0), glm::vec2 lastPosition = glm::vec2(0.0, 0.0)) {
        this->position = position;
        this->lastPosition = lastPosition;
    }
};

#endif