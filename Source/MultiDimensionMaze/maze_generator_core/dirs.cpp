#include "dirs.h"

dirs::dirs()
{
}

dirs::~dirs()
{
}

/**
 * Returns the opposite direction
 *
 * @param {Direction} dir
 * @return {Direction}
 */
Direction dirs::opposite(Direction dir) {
    switch (dir) {
    case A_PLUS: return A_MINUS;
    case A_MINUS: return A_PLUS;
    case B_PLUS: return B_MINUS;
    case B_MINUS: return B_PLUS;
    case C_PLUS: return C_MINUS;
    case C_MINUS: return C_PLUS;
    case UP_: return DOWN_;
    case DOWN_: return UP_;
    case NORTH: return SOUTH;
    case EAST: return WEST;
    case SOUTH: return NORTH;
    case WEST: return EAST;
    case NONE: return NONE;
    default: return DOWN_; // just to get compile to work, should never happen
    }
};

/**
 * Returns the a component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::da(Direction dir) {
    switch (dir) {
    case A_PLUS: return 1;
    case A_MINUS: return -1;
    default: return 0;
    }
};

/**
 * Returns the b component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::db(Direction dir) {
    switch (dir) {
    case B_PLUS: return 1;
    case B_MINUS: return -1;
    default: return 0;
    }
};

/**
 * Returns the c component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::dc(Direction dir) {
    switch (dir) {
    case C_PLUS: return 1;
    case C_MINUS: return -1;
    default: return 0;
    }
};

/**
 * Returns the z component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::dz(Direction dir) {
    switch (dir) {
    case UP_: return 1;
    case DOWN_: return -1;
    default: return 0;
    }
};

/**
 * Returns the y component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::dy(Direction dir) {
    switch (dir) {
    case NORTH: return -1;
    case SOUTH: return 1;
    default: return 0;
    }
};

/**
 * Returns the x component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::dx(Direction dir) {
    switch (dir) {
    case EAST: return 1;
    case WEST: return -1;
    default: return 0;
    }
};

/**
 * Returns a bitmask for the direction, useful for
 * storing a bunch of direction boolean values in
 * a single integer.
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::bitmask(Direction dir) {
    return 1 << dir;
};
