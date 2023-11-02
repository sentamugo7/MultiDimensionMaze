// maze algorithm from:
// https://github.com/sbj42/maze-generator

#pragma once

#include "dirs.h"
#include "CoreMinimal.h"

/**
 *
 */
class MULTIDIMENSIONMAZE_API Cell
{
public:
    Cell();
    ~Cell();
    bool is_open(Direction dir);
    void set_open(Direction dir, bool value);
    Direction getSolution();
    void setSolution(Direction solution);

private:
    int _passages;
    Direction _solution;
};
