#include "Cell.h"

namespace fifteenPuzzle {

	Cell& Cell::operator=(const Cell& other) {
		if (this == &other)
			return *this;

		this->row = other.row;
		this->col = other.col;

		return *this;
	}

	bool operator==(const Cell& lhs, const Cell& rhs) {
		return lhs.row == rhs.row && lhs.col == rhs.col; 
	}

	bool operator!=(const Cell& lhs, const Cell& rhs) { 
		return !(lhs == rhs); 
	}
}