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
	Position(int a, int b, int c, int z, int y, int x);
	~Position();
	int getA();
	int getB();
	int getC();
	int getZ();
	int getY();
	int getX();
	Position neighbor(Direction dir);
	Position clone();
	bool operator== (const Position& rhs) const {
		return _a == rhs._a && _b == rhs._b && _c == rhs._c && _z == rhs._z && _y == rhs._y && _x == rhs._x;
	}

private:
	int _a;
	int _b;
	int _c;
	int _z;
	int _y;
	int _x;
};
