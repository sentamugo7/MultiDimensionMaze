#include "GridMask.h"

/**
 * A GridMask is a rectangular grid of boolean values.
 *
 * @constructor
 * @param {integer} width
 * @param {integer} height
 * @param {integer} depth
 * @param {integer} a_size
 * @param {integer} b_size
 * @param {integer} c_size
 * @param {GridMaskOptions} options
 */
GridMask::GridMask(int a_size, int b_size, int c_size, int depth, int height, int width, GridMaskOptions options) {
    _a_size = a_size;
    _b_size = b_size;
    _c_size = c_size;
    _depth = depth;
    _height = height;
    _width = width;
    _interior = options.interior;
    _exterior = options.exterior;
    bool_grid.resize(_a_size);
    for (int a_loop = 0; a_loop < _a_size; a_loop++) {
        bool_grid[a_loop].resize(_b_size);
        for (int b_loop = 0; b_loop < _b_size; b_loop++) {
            bool_grid[a_loop][b_loop].resize(_c_size);
            for (int c_loop = 0; c_loop < _c_size; c_loop++) {
                bool_grid[a_loop][b_loop][c_loop].resize(_depth);
                for (int z_loop = 0; z_loop < _depth; z_loop++) {
                    bool_grid[a_loop][b_loop][c_loop][z_loop].resize(_height);
                    for (int y_loop = 0; y_loop < _height; y_loop++) {
                        bool_grid[a_loop][b_loop][c_loop][z_loop][y_loop].resize(_width);
                        for (int x_loop = 0; x_loop < _width; x_loop++) {
                            bool_grid[a_loop][b_loop][c_loop][z_loop][y_loop][x_loop] = _interior;
                        }
                    }
                }
            }
        }
    }
}

GridMask::~GridMask()
{
}

/**
 * The a_size of the GridMask
 *
 * @return {integer}
 */
int GridMask::a_size() {
    return _a_size;
};

/**
 * The b_size of the GridMask
 *
 * @return {integer}
 */
int GridMask::b_size() {
    return _b_size;
};

/**
 * The c_size of the GridMask
 *
 * @return {integer}
 */
int GridMask::c_size() {
    return _c_size;
};

/**
 * The depth of the GridMask
 *
 * @return {integer}
 */
int GridMask::depth() {
    return _depth;
};

/**
 * The height of the GridMask
 *
 * @return {integer}
 */
int GridMask::height() {
    return _height;
};

/**
 * The width of the GridMask
 *
 * @return {integer}
 */
int GridMask::width() {
    return _width;
};

/**
 * Returns the boolean value at the specified cell
 *
 * @param {Position} p
 * @return {bool}
 */
bool GridMask::get(Position pos) {
    if (pos.getA() < 0 || pos.getA() >= _a_size || pos.getB() < 0 || pos.getB() >= _b_size || pos.getC() < 0 || pos.getC() >= _c_size || pos.getZ() < 0 || pos.getZ() >= _depth || pos.getY() < 0 || pos.getY() >= _height || pos.getX() < 0 || pos.getX() >= _width) {
        return _exterior;
    }
    return bool_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()][pos.getY()][pos.getX()];
};

/**
 * Sets the boolean value at the specified cell.  Throws an
 * error for coordinates that lie outside the grid.
 *
 * @param {Position} p
 * @param {bool} [value=true]
 */
void GridMask::set(Position pos, bool value) {
    if (pos.getA() < 0 || pos.getA() >= _a_size || pos.getB() < 0 || pos.getB() >= _b_size || pos.getC() < 0 || pos.getC() >= _c_size || pos.getZ() < 0 || pos.getZ() >= _depth || pos.getY() < 0 || pos.getY() >= _height || pos.getX() < 0 || pos.getX() >= _width) {
        return;
    }
    if (pos.getA() >= bool_grid.size() || pos.getB() >= bool_grid[pos.getA()].size() || pos.getC() >= bool_grid[pos.getA()][pos.getB()].size() || pos.getZ() >= bool_grid[pos.getA()][pos.getB()][pos.getC()].size() || pos.getY() >= bool_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()].size() || pos.getX() >= bool_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()][pos.getY()].size()) {
        return;
    }

    bool_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()][pos.getY()][pos.getX()] = value;
};
