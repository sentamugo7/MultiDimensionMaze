#include "Maze.h"

Maze::Maze() : Maze(0, 0, 0, 0, 0, 0) {}

/**
 * A Maze is a rectangular grid of cells, where each cell
 * may have passages in each of the cardinal directions.
 * The maze is initialized with each cell having no passages.
 *
 * @constructor
 * @param {integer} u_size
 * @param {integer} v_size
 * @param {integer} w_size
 * @param {integer} depth
 * @param {integer} height
 * @param {integer} width
 */
Maze::Maze(int u_size, int v_size, int w_size, int depth, int height, int width) {
    _u_size = u_size;
    _v_size = v_size;
    _w_size = w_size;
    _depth = depth;
    _height = height;
    _width = width;

    cell_grid.clear();
    cell_grid.resize(getUSize());
    for (int u_loop = 0; u_loop < getUSize(); u_loop++) {
        cell_grid[u_loop].resize(getVSize());
        for (int v_loop = 0; v_loop < getVSize(); v_loop++) {
            cell_grid[u_loop][v_loop].resize(getWSize());
            for (int w_loop = 0; w_loop < getWSize(); w_loop++) {
                cell_grid[u_loop][v_loop][w_loop].resize(getDepth());
                for (int z_loop = 0; z_loop < getDepth(); z_loop++) {
                    cell_grid[u_loop][v_loop][w_loop][z_loop].resize(getHeight());
                    for (int y_loop = 0; y_loop < getHeight(); y_loop++) {
                        cell_grid[u_loop][v_loop][w_loop][z_loop][y_loop].resize(getWidth());
                    }
                }
            }
        }
    }
}

Maze::~Maze()
{
}
/**
 * The u_size of the Maze
 *
 * @return {integer}
 */

int Maze::getUSize() {
    return _u_size;
}
/**
 * The v_size of the Maze
 *
 * @return {integer}
 */
int Maze::getVSize() {
    return _v_size;
}
/**
 * The w_size of the Maze
 *
 * @return {integer}
 */
int Maze::getWSize() {
    return _w_size;
}
/**
 * The depth of the Maze
 *
 * @return {integer}
 */
int Maze::getDepth() {
    return _depth;
}
/**
 * The height of the Maze
 *
 * @return {integer}
 */
int Maze::getHeight() {
    return _height;
}
/**
 * The width of the Maze
 *
 * @return {integer}
 */
int Maze::getWidth() {
    return _width;
}

/**
 * The number of (non collapsed) dimensions of the Maze
 *
 * @return {integer}
 */
int Maze::getDimensionCount() {
    if (getUSize() > 1) {
        return 6;
    }
    else if (getVSize() > 1) {
        return 5;
    }
    else if (getWSize() > 1) {
        return 4;
    }
    else if (getDepth() > 1) {
        return 3;
    }
    else {
        return 2;
    }
}

bool Maze::isPosInMaze(Position pos) {
    if (pos.getU() < 0 || pos.getU() >= getUSize() || pos.getV() < 0 || pos.getV() >= getVSize() || pos.getW() < 0 || pos.getW() >= getWSize() || pos.getZ() < 0 || pos.getZ() >= getDepth() || pos.getY() < 0 || pos.getY() >= getHeight() || pos.getX() < 0 || pos.getX() >= getWidth()) {
        return false;
    }
    return true;
}
/**
 * Returns whether there is a passage at the given position and
 * direction
 *
 * @param {Position} pos
 * @param {Direction} dir
 * @return {bool}
 */

bool Maze::getPassage(Position pos, Direction dir) {
    if (!isPosInMaze(pos)) {
        return false;
    }
    return cell_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()][pos.getY()][pos.getX()].is_open(dir);
};

/**
 * Creates or removes a passage at the given position and
 * direction.  Note that this also creates the corresponding
 * passage in the neighboring cell.
 *
 * @param {Position} pos
 * @param {Direction} dir
 * @param {bool} value
 */
void Maze::setPassage(Position pos, Direction dir, bool value) {
    if (!isPosInMaze(pos)) {
        return;
    }
    cell_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()][pos.getY()][pos.getX()].set_open(dir, value);
    if (!isPosInMaze(pos.neighbor(dir))) {
        return;
    }
    cell_grid[pos.getU() + dirs::du(dir)][pos.getV() + dirs::dv(dir)][pos.getW() + dirs::dw(dir)][pos.getZ() + dirs::dz(dir)][pos.getY() + dirs::dy(dir)][pos.getX() + dirs::dx(dir)].set_open(dirs::opposite(dir), value);
};

/**
 * Gets the end position of the maze
 *
 * @return {Position}
 */
Position Maze::getEnd() {
    return Position(getUSize() - 1, getVSize() - 1, getWSize() - 1, getDepth() - 1, getHeight() - 1, getWidth() - 1);
}

/**
 * Gets a pointer to the Cell at the specified Position
 *
 * @param {Position} pos
 * @return {Cell*}
 */
Cell* Maze::getCellAt(Position pos) {
    if (!isPosInMaze(pos)) {
        return NULL;
    }
    return &cell_grid[pos.getU()][pos.getV()][pos.getW()][pos.getZ()][pos.getY()][pos.getX()];
}

/**
 * Gets the hint/solution direction at the specified Cell
 *
 * @param {Position} pos
 * @return {Direction}
 */
Direction Maze::getHintAt(Position pos) {
    return getCellAt(pos)->getSolution();
}

/**
 * Solve the Maze from the specified Position
 *
 * @param {Position} pos
 */
void Maze::solveAt(Position pos) {
    std::vector<Direction> noneDirs;
    for (int dirLoop = 0; dirLoop < DIRECTION_COUNT; dirLoop++) {
        Direction dir = static_cast<Direction>(dirLoop);
        if (getPassage(pos, dir)) {
            Position neighborPos = pos.neighbor(dir);
            //if (getCellAt(neighborPos) != NULL && getCellAt(neighborPos)->getSolution() == NONE) {
            if (getCellAt(neighborPos)->getSolution() == NONE) {
            getCellAt(neighborPos)->setSolution(dirs::opposite(dir));
                noneDirs.push_back(dir);
            }
        }
    }
    for (int noneLoop = 0; noneLoop < noneDirs.size(); noneLoop++) {
        Direction dir = noneDirs[noneLoop];
        Position neighborPos = pos.neighbor(dir);
        solveAt(neighborPos);
    }
}

/**
 * Solve the Maze (get solution/hint Direction at every Cell)
 *
 */
void Maze::solve() {
    getCellAt(getEnd())->setSolution(Direction::END);
    solveAt(getEnd());
}

int Maze::solutionCount() {
    int count = 0;
    Position posLoop = START;
    while (!(posLoop == getEnd())) {
        count++;
        posLoop = posLoop.neighbor(getCellAt(posLoop)->getSolution());
    }
    return count;
}

int Maze::basisCount() {
    return _u_size + _v_size + _w_size + _depth + _height + _width;
}

