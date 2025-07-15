#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H


struct PlayerComponent {
    bool isPlayer;

    PlayerComponent() {
        this->isPlayer = true;
    }
};

#endif