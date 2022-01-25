// maze algorithm from:
// https://github.com/sbj42/maze-generator

#pragma once

#include <vector>
#include "Position.h"
#include "CoreMinimal.h"
struct GridMaskOptions { bool interior; bool exterior; };
/**
 * 
 */
class MULTIDIMENSIONMAZE_API GridMask
{
public:
	GridMask(int u_size, int v_size, int w_size, int depth, int height, int width, GridMaskOptions options);
	~GridMask();

    int u_size();
    int v_size();
    int w_size();
    int depth();
    int height();
    int width();
    bool get(Position pos);
    void set(Position pos, bool value);
 
private:
    int _u_size;
    int _v_size;
    int _w_size;
    int _depth;
    int _height;
    int _width;
    bool _interior;
    bool _exterior;

    //  note the original grid array is replaced by a multi dimensional vector of bool objects
    std::vector<std::vector<std::vector<std::vector<std::vector<std::vector<bool>>>>>> bool_grid;
};
