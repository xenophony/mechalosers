#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

// #include <SDL2/SDL.h>
#include <vector>

struct GridComponent {
    std::vector<std::vector<int>> grid;

    GridComponent(std::vector<std::vector<int>> grid) {
        this->grid = grid;
    }
};

#endif