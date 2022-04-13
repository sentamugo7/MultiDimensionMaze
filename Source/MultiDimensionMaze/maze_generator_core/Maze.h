// maze algorithm from:
// https://github.com/sbj42/maze-generator

#pragma once

#include <vector>
#include "Cell.h"
#include "dirs.h"
#include "Position.h"
#include "CoreMinimal.h"

static const Position START(0, 0, 0, 0, 0, 0);

/**
 * 
 */
class MULTIDIMENSIONMAZE_API Maze
{
public:
	Maze();
	Maze(int u_size, int v_size, int w_size, int depth, int height, int width);
	~Maze();
	int getUSize();
	int getVSize();
	int getWSize();
	int getDepth();
	int getHeight();
	int getWidth();
	int getDimensionCount();
	bool isPosInMaze(Position pos);
	bool getPassage(Position pos, Direction dir);
	void setPassage(Position pos, Direction dir, bool value);
	Position getEnd();
	Direction getHintAt(Position pos);
	void solve();
	int solutionCount();
	int basisCount();


private:
	int _u_size;
	int _v_size;
	int _w_size;
	int _depth;
	int _height;
	int _width;
	//  note the original grid array is replaced by a multi dimensional vector of Cell objects
	std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<Cell>>>>>> cell_grid;
	Cell* getCellAt(Position pos);
	void solveAt(Position pos);
};
