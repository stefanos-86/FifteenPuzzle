#include <gtest/gtest.h>

#include "Cell.h"

namespace fifteenPuzzle {

	TEST(Cell, assign_self) {
		Cell c{ 1, 2 };
		
		c = c;

		// No damage done.
		Cell d{ 1, 2 };
		ASSERT_EQ(c, d);
	}

	TEST(Cell, assign) {
		Cell c{ 1, 2 };
		Cell d{ 3, 4 };
		
		d = c;

		ASSERT_EQ(c, d);
	}

	TEST(Cell, compare_row) {
		Cell c{ 0, 0 };
		Cell d{ 1, 0 };

		ASSERT_NE(c, d);
	}

	TEST(Cell, compare_col) {
		Cell c{ 0, 0 };
		Cell d{ 0, 1 };

		ASSERT_NE(c, d);
	}

}