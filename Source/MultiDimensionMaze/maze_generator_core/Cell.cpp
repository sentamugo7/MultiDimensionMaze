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
