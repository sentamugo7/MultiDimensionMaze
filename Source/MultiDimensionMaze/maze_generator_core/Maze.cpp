#include "Maze.h"

Maze::Maze() : Maze(0, 0, 0, 0, 0, 0) {}

/**
 * A Maze is a rectangular grid of cells, where each cell
 * may have passages in each of the cardinal directions.
 * The maze is initialized with each cell having no passages.
 *
 * @constructor
 * @param {integer} a_size
 * @param {integer} b_size
 * @param {integer} c_size
 * @param {integer} depth
 * @param {integer} height
 * @param {integer} width
 */
Maze::Maze(int a_size, int b_size, int c_size, int depth, int height, int width) {
    _a_size = a_size;
    _b_size = b_size;
    _c_size = c_size;
    _depth = depth;
    _height = height;
    _width = width;

    cell_grid.clear();
    cell_grid.resize(getASize());
    for (int a_loop = 0; a_loop < getASize(); a_loop++) {
        cell_grid[a_loop].resize(getBSize());
        for (int b_loop = 0; b_loop < getBSize(); b_loop++) {
            cell_grid[a_loop][b_loop].resize(getCSize());
            for (int c_loop = 0; c_loop < getCSize(); c_loop++) {
                cell_grid[a_loop][b_loop][c_loop].resize(getDepth());
                for (int z_loop = 0; z_loop < getDepth(); z_loop++) {
                    cell_grid[a_loop][b_loop][c_loop][z_loop].resize(getHeight());
                    for (int y_loop = 0; y_loop < getHeight(); y_loop++) {
                        cell_grid[a_loop][b_loop][c_loop][z_loop][y_loop].resize(getWidth());
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
 * The a_size of the Maze
 *
 * @return {integer}
 */

int Maze::getASize() {
    return _a_size;
}
/**
 * The b_size of the Maze
 *
 * @return {integer}
 */
int Maze::getBSize() {
    return _b_size;
}
/**
 * The c_size of the Maze
 *
 * @return {integer}
 */
int Maze::getCSize() {
    return _c_size;
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
    if (getASize() > 1) {
        return 6;
    }
    else if (getBSize() > 1) {
        return 5;
    }
    else if (getCSize() > 1) {
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
    if (pos.getA() < 0 || pos.getA() >= getASize() || pos.getB() < 0 || pos.getB() >= getBSize() || pos.getC() < 0 || pos.getC() >= getCSize() || pos.getZ() < 0 || pos.getZ() >= getDepth() || pos.getY() < 0 || pos.getY() >= getHeight() || pos.getX() < 0 || pos.getX() >= getWidth()) {
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
    return cell_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()][pos.getY()][pos.getX()].is_open(dir);
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
    cell_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()][pos.getY()][pos.getX()].set_open(dir, value);
    if (!isPosInMaze(pos.neighbor(dir))) {
        return;
    }
    cell_grid[pos.getA() + dirs::da(dir)][pos.getB() + dirs::db(dir)][pos.getC() + dirs::dc(dir)][pos.getZ() + dirs::dz(dir)][pos.getY() + dirs::dy(dir)][pos.getX() + dirs::dx(dir)].set_open(dirs::opposite(dir), value);
};

/**
 * Gets the end position of the maze
 *
 * @return {Position}
 */
Position Maze::getEnd() {
    return Position(getASize() - 1, getBSize() - 1, getCSize() - 1, getDepth() - 1, getHeight() - 1, getWidth() - 1);
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
    return &cell_grid[pos.getA()][pos.getB()][pos.getC()][pos.getZ()][pos.getY()][pos.getX()];
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
    return _a_size + _b_size + _c_size + _depth + _height + _width;
}

Direction Maze::getLastSolution() {
    Direction lastSolution = NONE;
    Position posLoop = START;
    while (!(posLoop == getEnd())) {
        lastSolution = getCellAt(posLoop)->getSolution();
        posLoop = posLoop.neighbor(getCellAt(posLoop)->getSolution());
    }
    return lastSolution;
}
