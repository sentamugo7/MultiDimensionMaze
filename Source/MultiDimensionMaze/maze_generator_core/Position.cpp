#include "Position.h"

/**
 * Position is a six dimensional location - scalar at each dimension
 *
 * @constructor
 * @param {int} u
 * @param {int} v
 * @param {int} w
 * @param {int} z
 * @param {int} y
 * @param {int} x
 */
Position::Position(int u, int v, int w, int z, int y, int x) {
	_u = u;
	_v = v;
	_w = w;
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
	_u = 0;
	_v = 0;
	_w = 0;
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
 * Returns u dimension value
 *
 * @return {int}
 */
int Position::getU() {
	return _u;
}

/**
 * Returns v dimension value
 *
 * @return {int}
 */
int Position::getV() {
	return _v;
}

/**
 * Returns w dimension value
 *
 * @return {int}
 */
int Position::getW() {
	return _w;
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
	return Position(_u + dirs::du(dir), _v + dirs::dv(dir), _w + dirs::dw(dir), _z + dirs::dz(dir), _y + dirs::dy(dir), _x + dirs::dx(dir));
}

/**
 * clone (copy) this Position
 *
 * @return {Position}
 */
Position Position::clone() {
	return Position(getU(), getV(), getW(), getZ(), getY(), getX());
}
