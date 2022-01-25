#include "Cell.h"

/**
 * A Cell is a wrapper object that makes it easier to
 * ask for passage information by name. No argument constructor
 *
 * @constructor
 */
Cell::Cell() {
	_passages = 0;
	_solution = NONE;
}

/**
 * Cell destructor
 *
 */
Cell::~Cell()
{
}

/**
 * Returns if the Cell has a passage in the u plus direction
 *
 * @return {bool}
 */
bool Cell::u_plus() {
	return is_open(U_PLUS);
}

/**
 * Returns if the Cell has a passage in the u minus direction
 *
 * @return {bool}
 */
bool Cell::u_minus() {
	return is_open(U_MINUS);
}

/**
 * Returns if the Cell has a passage in the v plus direction
 *
 * @return {bool}
 */
bool Cell::v_plus() {
	return is_open(V_PLUS);
}

/**
 * Returns if the Cell has a passage in the v minus direction
 *
 * @return {bool}
 */
bool Cell::v_minus() {
	return is_open(V_MINUS);
}

/**
 * Returns if the Cell has a passage in the w plus direction
 *
 * @return {bool}
 */
bool Cell::w_plus() {
	return is_open(W_PLUS);
}

/**
 * Returns if the Cell has a passage in the w minus direction
 *
 * @return {bool}
 */
bool Cell::w_minus() {
	return is_open(W_MINUS);
}

/**
 * Returns if the Cell has a passage in the up (z plus) direction
 *
 * @return {bool}
 */
bool Cell::up_() {
	return is_open(UP_);
}

/**
 * Returns if the Cell has a passage in the down (z minus) direction
 *
 * @return {bool}
 */
bool Cell::down_() {
	return is_open(DOWN_);
}

/**
 * Returns if the Cell has a passage in the south (y plus) direction
 *
 * @return {bool}
 */
bool Cell::south() {
	return is_open(SOUTH);
}

/**
 * Returns if the Cell has a passage in the north (y minus) direction
 *
 * @return {bool}
 */

bool Cell::north() {
	return is_open(NORTH);
}

/**
 * Returns if the Cell has a passage in the east (x plus) direction
 *
 * @return {bool}
 */
bool Cell::east() {
	return is_open(EAST);
}


/**
 * Returns if the Cell has a passage in the west (x minus) direction
 *
 * @return {bool}
 */
bool Cell::west() {
	return is_open(WEST);
}

/**
 * Returns the boolean if the Cell has a passage in the specified direction
 *
 * @param {Direction} dir
 * @return {bool}
 */
bool Cell::is_open(Direction dir) {
	int exp = static_cast<int>(dir);
	return (_passages >> exp) & 1U;
}

/**
 * sets the boolean for the Cell to have a passage in the specified direction
 *
 * @param {Direction} dir
 * @param {bool} value
 */
void Cell::set_open(Direction dir, bool value) {
	int exp = static_cast<int>(dir);
	int mask = 1UL << exp;
	if (value) {
		_passages |= mask;
	}
	else {
		_passages &= ~(1UL << exp);
	}
}

/**
 * gets the solution (hint) direction for the Cell
 *
 * @return {Direction}
 */
Direction Cell::getSolution() {
	return _solution;
}

/**
 * sets the solution (hint) direction for the Cell
 *
 * @param {Direction} solution
 */
void Cell::setSolution(Direction solution) {
	_solution = solution;
}
