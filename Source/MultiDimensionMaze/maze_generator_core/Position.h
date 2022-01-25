#pragma once

#include "dirs.h"
#include "CoreMinimal.h"


/**
 * 
 */
class MULTIDIMENSIONMAZE_API Position
{
public:
	Position();
	Position(int u, int v, int w, int z, int y, int x);
	~Position();
	int getU();
	int getV();
	int getW();
	int getZ();
	int getY();
	int getX();
	Position neighbor(Direction dir);
	Position clone();
	bool operator== (const Position& rhs) const {
		return _u == rhs._u && _v == rhs._v && _w == rhs._w && _z == rhs._z && _y == rhs._y && _x == rhs._x;
	}

private:
	int _u;
	int _v;
	int _w;
	int _z;
	int _y;
	int _x;
};
