#include "GridMask.h"

/**
 * A GridMask is a rectangular grid of boolean values.
 *
 * @constructor
 * @param {integer} width
 * @param {integer} height
 * @param {integer} depth
 * @param {integer} u_size
 * @param {integer} v_size
 * @param {integer} w_size
 * @param {GridMaskOptions} options
 */
GridMask::GridMask(int u_size, int v_size, int w_size, int depth, int height, int width, GridMaskOptions options) {
    _u_size = u_size;
    _v_size = v_size;
    _w_size = w_size;
    _depth = depth;
    _height = height;
    _width = width;
    _interior = options.interior;
    _exterior = options.exterior;
    bool_grid.resize(_u_size);
    for (int u_loop = 0; u_loop < _u_size; u_loop++) {
        bool_grid[u_loop].resize(_v_size);
        for (int v_loop = 0; v_loop < _v_size; v_loop++) {
            bool_grid[u_loop][v_loop].resize(_w_size);
            for (int w_loop = 0; w_loop < _w_size; w_loop++) {
                bool_grid[u_loop][v_loop][w_loop].resize(_depth);
                for (int z_loop = 0; z_loop < _depth; z_loop++) {
                    bool_grid[u_loop][v_loop][w_loop][z_loop].resize(_height);
                    for (int y_loop = 0; y_loop < _height; y_loop++) {
                        bool_grid[u_loop][v_loop][w_loop][z_loop][y_loop].resize(_width);
                        for (int x_loop = 0; x_loop < _width; x_loop++) {
                            bool_grid[u_loop][v_loop][w_loop][z_loop][y_loop][x_loop] = _interior;
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
 * The u_size of the GridMask
 *
 * @return {integer}
 */
int GridMask::u_size() {
    return _u_size;
};

/**
 * The v_size of the GridMask
 *
 * @return {integer}
 */
int GridMask::v_size() {
    return _v_size;
};

/**
 * The w_size of the GridMask
 *
 * @return {integer}
 */
int GridMask::w_size() {
    return _w_size;
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
    if (pos.getU() < 0 || pos.getU() >= _u_size || pos.getV() < 0 || pos.getV() >= _v_size || pos.getW() < 0 || pos.getW() >= _w_size || pos.getZ() < 0 || pos.getZ() >= _depth || pos.getY() < 0 || pos.getY() >= _height || pos.getX() < 0 || pos.getX() >= _width) {
        return _exterior;
    }
    return bool_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()][pos.getY()][pos.getX()];
};

/**
 * Sets the boolean value at the specified cell.  Throws an
 * error for coordinates that lie outside the grid.
 *
 * @param {Position} p
 * @param {bool} [value=true]
 */
void GridMask::set(Position pos, bool value) {
    if (pos.getU() < 0 || pos.getU() >= _u_size || pos.getV() < 0 || pos.getV() >= _v_size || pos.getW() < 0 || pos.getW() >= _w_size || pos.getZ() < 0 || pos.getZ() >= _depth || pos.getY() < 0 || pos.getY() >= _height || pos.getX() < 0 || pos.getX() >= _width) {
        return;
    }
    if (pos.getU() >= bool_grid.size() || pos.getV() >= bool_grid[pos.getU()].size() || pos.getW() >= bool_grid[pos.getU()][pos.getV()].size() || pos.getZ() >= bool_grid[pos.getU()][pos.getV()][pos.getW()].size() || pos.getY() >= bool_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()].size() || pos.getX() >= bool_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()][pos.getY()].size()) {
        return;
    }

   bool_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()][pos.getY()][pos.getX()] = value;
};
