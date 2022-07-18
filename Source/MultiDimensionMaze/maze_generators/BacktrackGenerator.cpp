#include "BacktrackGenerator.h"

/**
 * BacktrackGenerator generates a Maze with the backtrack algorithm
 *
 * @constructor
 */
BacktrackGenerator::BacktrackGenerator()
{
    //////////    set for consistent testing
    //////////srand(1);
    /////    for (int i = 0; i < 3; i++) rand();
}

/**
 * BacktrackGenerator destructor
 *
 */
BacktrackGenerator::~BacktrackGenerator()
{
}

void BacktrackGenerator::generate(Maze& maze) {
    MGOptions mgOptions = { };
    backtrack(maze, mgOptions);
}

/**
 * Chooses a random integer between 0 (inclusive) and max (exclusive)
 *
 * @param {MGOptions} options
 * @param {integer} max
 */
int BacktrackGenerator::randomInt(MGOptions options, int max) {
    int rr = rand();
    return floor(((double)rr / (double)RAND_MAX) * (double)max);
}

/**
 * Chooses a random element from an array
 *
 * @param {MGOptions} options
 * @param {Array} array
 */
Direction BacktrackGenerator::randomChoice(MGOptions options, std::vector<Direction> array) {
    if (array.size() == 1)
        return array[0];
    int rr = randomInt(options, array.size());
    if (rr >= array.size()) {
        return Direction::EAST;
    }
    return array[rr];
}

/**
 * Returns an array of directions toward neighboring
 * cells that have not been visited (i.e. cells for
 * which visited.get returns false).
 *
 * @param {MGOptions} options
 * @param {GridMask} visited
 * @param {integer[]} pos
 */
std::vector<Direction> BacktrackGenerator::getUnvisitedDirections(MGOptions options, GridMask visited, Position pos) {
    std::vector<Direction> ret;
    for (int loop = WEST; loop >= 0; loop--) {
        Direction dir = static_cast<Direction>(loop);
        if (!visited.get(pos.neighbor(dir))) {
            ret.push_back(dir);
        }
    }
    return ret;
}

/**
 * Recursive-backtracking maze generator.
 *
 * @param {Maze} maze
 * @param {MGOptions} options
 */
void BacktrackGenerator::backtrack(Maze& maze, MGOptions mgOptions) {
    int a_size = maze.getASize();
    int b_size = maze.getBSize();
    int c_size = maze.getCSize();
    int depth = maze.getDepth();
    int height = maze.getHeight();
    int width = maze.getWidth();

    // The 'visited' mask marks cells that have been added to the maze
    // We use this to see which neighbors are available for connecting
    // with a passage.  To avoid making a passage to the outside of
    // the grid, we mark the exterior cells as if they were already
    // included.
    GridMaskOptions gmOptions = { /*.interior=*/ false, /*.exterior=*/ true };

    GridMask visited = GridMask(a_size, b_size, c_size, depth, height, width, gmOptions);

    Position save_cur(0, 0, 0, 0, 0, 0);
    Position cur(0, 0, 0, 0, 0, 0);
    // Mark the first cell as visited
    visited.set(cur, true);
    // This array will hold the cells that we've moved through,
    // to which we may return later to create a new branch
    std::vector<Position> stack;
    MGOptions options;
    // Loop until the maze is complete
    for (;;) {

        // Which directions can we go from this cell,
        // that lead to cells that aren't yet in the maze?
        std::vector<Direction> neighbors = getUnvisitedDirections(options, visited, cur);

        if (neighbors.size() > 0) {
            // If there is at least one such direction, pick
            // one at random
            Direction dir = randomChoice(options, neighbors);
            // If there were other choices, then push the
            // current cell onto the stack.
            if (neighbors.size() > 1)
                stack.push_back(cur);
            // Dig a passage from the curent cell to the next cell
            maze.setPassage(cur, dir, true);
            // Move on to the next cell
            save_cur = cur.clone();
            cur = cur.neighbor(dir);
            // Which is now in the maze
            visited.set(cur, true);
        }
        else if (stack.size() > 0) {
            // If there are no available directions here, but
            // there are still cells in the stack, then pop
            // the last cell off the stack ("backtracking"),
            // and continue from there ("recusrive")
            cur = stack.back();
            stack.pop_back();
        }
        else {
            // If there are no available directions and the
            // stack is empty, then the maze is complete
            break;
        }
    }
}
