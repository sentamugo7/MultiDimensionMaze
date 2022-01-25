// maze algorithm from:
// https://github.com/sbj42/maze-generator

#pragma once

#include <vector>
#include <array>
#include "../maze_generator_core/dirs.h"
#include "../maze_generator_core/Position.h"
#include "../maze_generator_core/Maze.h"
#include "../maze_generator_core/GridMask.h"
#include <cmath>
#include "CoreMinimal.h"
struct MGOptions { };

/**
 * 
 */
class MULTIDIMENSIONMAZE_API BacktrackGenerator
{
public:
	BacktrackGenerator();
	~BacktrackGenerator();
	void generate(Maze& maze);
	int randomInt(MGOptions options, int max);
	Direction randomChoice(MGOptions options, std::vector<Direction> array);
    std::vector<Direction> getUnvisitedDirections(MGOptions options, GridMask visited, Position pos);
	void backtrack(Maze& maze, MGOptions mgOptions);
private:
};
