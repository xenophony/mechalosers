#ifndef KEYBOARD_CONTROLLEDCOMPONENT_H
#define KEYBOARD_CONTROLLEDCOMPONENT_H

#include <glm/glm.hpp>



struct KeyboardControlledComponent {
    glm::vec2 upVelocity = glm::vec2(0.0);
    glm::vec2 rightVelocity = glm::vec2(0.0);
    glm::vec2 downVelocity = glm::vec2(0.0);
    glm::vec2 leftVelocity = glm::vec2(0.0);
    // KeyboardControlledComponent(
    //     glm::vec2 upVelocity,
    //     glm::vec2 rightVelocity,
    //     glm::vec2 downVelocity,
    //     glm::vec2 leftVelocity,
    //     ) : upVelocity(upVelocity), rightVelocity(rightVelocity),
    //     downVelocity(downVelocity), leftVelocity(leftVelocity) {
    // }

    KeyboardControlledComponent(glm::vec2 upVelocity = glm::vec2(0, 0), glm::vec2 rightVelocity = glm::vec2(0, 0), glm::vec2 downVelocity = glm::vec2(0, 0), glm::vec2 leftVelocity = glm::vec2(0, 0))
    {
        this->upVelocity = upVelocity;
        this->rightVelocity = rightVelocity;
        this->downVelocity = downVelocity;
        this->leftVelocity = leftVelocity;
    }

    // KeyboardControlledComponent(
    //     int upX, int upY,
    //     int rightX, int rightY,
    //     int downX, int downY,
    //     int leftX, int leftY
    // ) {
    //     this->upVelocity = glm::vec2(upX, upY);
    //     this->rightVelocity = glm::vec2(rightX, rightY);
    //     this->downVelocity = glm::vec2(downX, downY);
    //     this->leftVelocity = glm::vec2(leftX, leftY);
    // }

    // KeyboardControlledComponent(
    //     glm::vec2 upVelocity = glm::vec2(0),
    //     glm::vec2 rightVelocity = glm::vec2(0),
    //     glm::vec2 downVelocity = glm::vec2(0),
    //     glm::vec2 leftVelocity = glm::vec2(0),
    //     bool isExplicit = false;
    // ) {
    //     this->upVelocity = upVelocity;
    //     this->rightVelocity = rightVelocity;
    //     this->downVelocity = downVelocity;
    //     this->leftVelocity = leftVelocity;
    // }
};

#endif