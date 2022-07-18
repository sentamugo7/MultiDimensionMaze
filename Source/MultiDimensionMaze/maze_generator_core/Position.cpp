#include "Position.h"

/**
 * Position is a six dimensional location - scalar at each dimension
 *
 * @constructor
 * @param {int} a
 * @param {int} b
 * @param {int} c
 * @param {int} z
 * @param {int} y
 * @param {int} x
 */
Position::Position(int a, int b, int c, int z, int y, int x) {
	_a = a;
	_b = b;
	_c = c;
	_z = z;
	_y = y;
	_x = x;
}

/**
 * Position is a six dimensional location - scalar at each dimension
 *
 * @constructor
 */
Position::Position() {
	_a = 0;
	_b = 0;
	_c = 0;
	_z = 0;
	_y = 0;
	_x = 0;
}

/**
 * Position destructor
 *
 */
Position::~Position()
{
}

/**
 * Returns a dimension value
 *
 * @return {int}
 */
int Position::getA() {
	return _a;
}

/**
 * Returns b dimension value
 *
 * @return {int}
 */
int Position::getB() {
	return _b;
}

/**
 * Returns c dimension value
 *
 * @return {int}
 */
int Position::getC() {
	return _c;
}

/**
 * Returns z dimension value
 *
 * @return {int}
 */
int Position::getZ() {
	return _z;
}

/**
 * Returns y dimension value
 *
 * @return {int}
 */
int Position::getY() {
	return _y;
}

/**
 * Returns x dimension value
 *
 * @return {int}
 */
int Position::getX() {
	return _x;
}

/**
 * Returns the Position one in the Direction specified 
 *
 * @param {Direction} dir
 * @return {Position}
 */
Position Position::neighbor(Direction dir) {
	return Position(_a + dirs::da(dir), _b + dirs::db(dir), _c + dirs::dc(dir), _z + dirs::dz(dir), _y + dirs::dy(dir), _x + dirs::dx(dir));
}

/**
 * clone (copy) this Position
 *
 * @return {Position}
 */
Position Position::clone() {
	return Position(getA(), getB(), getC(), getZ(), getY(), getX());
}
