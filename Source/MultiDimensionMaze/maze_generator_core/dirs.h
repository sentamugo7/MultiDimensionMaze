// maze algorithm from:
// https://github.com/sbj42/maze-generator

#pragma once

#include "CoreMinimal.h"

enum Direction { U_PLUS, U_MINUS, V_PLUS, V_MINUS, W_PLUS, W_MINUS, UP_ /* z increase */, DOWN_ /* z decrease */, SOUTH /* y increase */, NORTH /* y decrease */, EAST /* x increase */, WEST /* x decrease */, NONE, END };
static const char* DIRECTION_TEXT[] =
{ "U_PLUS", "U_MINUS", "V_PLUS", "V_MINUS", "W_PLUS", "W_MINUS", "UP_", "DOWN_", "SOUTH", "NORTH", "EAST", "WEST", "NONE", "END" };

static const int DIRECTION_COUNT = NONE;

/**
 * 
 */
class MULTIDIMENSIONMAZE_API dirs
{
public:
	dirs();
	~dirs();
	static Direction opposite(Direction dir);
	static int du(Direction dir);
	static int dv(Direction dir);
	static int dw(Direction dir);
	static int dz(Direction dir);
	static int dy(Direction dir);
	static int dx(Direction dir);
	static int bitmask(Direction dir);
};
