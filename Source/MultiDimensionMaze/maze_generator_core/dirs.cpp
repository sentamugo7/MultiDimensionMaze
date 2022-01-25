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
        case U_PLUS: return U_MINUS;
        case U_MINUS: return U_PLUS;
        case V_PLUS: return V_MINUS;
        case V_MINUS: return V_PLUS;
        case W_PLUS: return W_MINUS;
        case W_MINUS: return W_PLUS;
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
 * Returns the u component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::du(Direction dir) {
    switch (dir) {
    case U_PLUS: return 1;
    case U_MINUS: return -1;
    default: return 0;
    }
};

/**
 * Returns the v component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::dv(Direction dir) {
    switch (dir) {
    case V_PLUS: return 1;
    case V_MINUS: return -1;
    default: return 0;
    }
};

/**
 * Returns the w component of the direction vector
 *
 * @param {Direction} dir
 * @return {integer}
 */
int dirs::dw(Direction dir) {
    switch (dir) {
    case W_PLUS: return 1;
    case W_MINUS: return -1;
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
