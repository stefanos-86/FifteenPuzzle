#pragma once

#include <cstdint>

namespace fifteenPuzzle {

	/** Type to express rows and columns in the board. */
	using Coordinate = uint8_t;

	/** Type to "point at" a cell in the board. */
	class Cell {
	public:
		Coordinate row;
		Coordinate col;

		Cell& operator=(const Cell& other);
	};
	
	bool operator==(const Cell& lhs, const Cell& rhs);
	bool operator!=(const Cell& lhs, const Cell& rhs);

}