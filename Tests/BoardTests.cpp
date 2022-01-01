#include <gtest/gtest.h>

#include "Board.h"

#include <functional>

namespace fifteenPuzzle {

	TEST(Board, get__first_element) {
		Board b;
		
		ASSERT_EQ(1, b.get({ 0, 0 }));
	}

	TEST(Board, get__last_element) {
		Board b;

		ASSERT_EQ(Board::HOLE, b.get({ 3, 3 }));
	}

	TEST(Board, set) {
		Board b;
		Cell position = { 1, 1 };

		b.unsafeSet(position, 128);

		ASSERT_EQ(128, b.get(position));
	}
	
	TEST(Board, moveIntoHole_impossible) {
		Board b;

		const Cell notNextToHole{ 0, 0 };
		const BoardValue before = b.get(notNextToHole);

		ASSERT_DEATH({
			b.moveIntoHole(notNextToHole);
			},
			"Assertion failed: canBeMoved.cell."
			);
	}

	TEST(Board, moveIntoHole_fromLeft) {
		Board b;

		const Cell nextToHole{ 3, 2 };
		const Cell hole{ 3, 3 };

		const BoardValue before = b.get(nextToHole);

		ASSERT_EQ(hole, b.moveIntoHole(nextToHole));

		ASSERT_EQ(before, b.get(hole));
		ASSERT_EQ(Board::HOLE, b.get(nextToHole));
	}

	TEST(Board, moveIntoHole_fromRight) {
		Board b;

		const Cell left{ 3, 2 };
		const Cell right{ 3, 3 };

		b.unsafeSet(left, Board::HOLE);
		b.unsafeSet(right, 15u);

		ASSERT_EQ(left, b.moveIntoHole(right));

		ASSERT_EQ(15u, b.get(left));
		ASSERT_EQ(Board::HOLE, b.get(right));
	}

	TEST(Board, moveIntoHole_fromTop) {
		Board b;

		const Cell top{ 2, 3 };
		const Cell bottom{ 3, 3 };

		b.unsafeSet(bottom, Board::HOLE);
		b.unsafeSet(top, 15u);

		ASSERT_EQ(bottom, b.moveIntoHole(top));

		ASSERT_EQ(15u, b.get(bottom));
		ASSERT_EQ(Board::HOLE, b.get(top));
	}

	TEST(Board, moveIntoHole_fromBottom) {
		Board b;

		const Cell top{ 2, 3 };
		const Cell bottom{ 3, 3 };

		b.unsafeSet(top, Board::HOLE);
		b.unsafeSet(bottom, 15u);

		ASSERT_EQ(top, b.moveIntoHole(bottom));

		ASSERT_EQ(15u, b.get(top));
		ASSERT_EQ(Board::HOLE, b.get(bottom));
	}

	TEST(Board, canBeMoved_no) {
		Board b;

		ASSERT_FALSE(b.canBeMoved({0, 0}));
	}

	TEST(Board, canBeMoved_hole) {
		Board b;

		ASSERT_DEATH({
			b.canBeMoved({ 3, 3 });
		}, 
		"Assertion failed: !isHole.cell.");
	}

	TEST(Board, canBeMoved_yes) {
		Board b;

		ASSERT_TRUE(b.canBeMoved({ 3, 2 }));
	}

	TEST(Board, isSolvable__actually_solved) {
		Board b;

		ASSERT_TRUE(b.isSolvable());
	}

	TEST(Board, isSolvable__14_15_problem) {
		Board b;

		b.unsafeSet({ 3,2 }, 14);
		b.unsafeSet({ 3,1 }, 15);

		ASSERT_FALSE(b.isSolvable());
	}

	TEST(Board, shuffle) {
		Board b;

		b.shuffle();
		
		ASSERT_EQ(Board::HOLE, b.get({3, 3}));
	}


	TEST(Board, solved_yes) {
		Board b;

		ASSERT_TRUE(b.isSolved());
	}

	TEST(Board, solved_no) {
		Board b;

		b.unsafeSet({ 0, 0 }, 2);
		b.unsafeSet({ 0, 1 }, 1);

		ASSERT_FALSE(b.isSolved());
	}

	// Reference: https://github.com/google/googletest/blob/master/docs/advanced.md#death-tests

	TEST(BoardDeathTest, get__outOfBound_row) {
		ASSERT_DEATH({
			Board b;
			b.get({ 4, 0 });
			},
			"position.row < BOARD_SIZE"
		);
	}

	TEST(BoardDeathTest, get__outOfBound_column) {
		ASSERT_DEATH({
			Board b;
			b.get({ 0, 4 });
			},
			"position.col < BOARD_SIZE"
		);
	}

	TEST(BoardDeathTest, moveIntoHole__outOfBound_column) {
		ASSERT_DEATH({
			Board b;
			b.moveIntoHole({ 0, 4 });
			},
			"position.col < BOARD_SIZE"
		);
	}

	TEST(BoardDeathTest, moveIntoHole__moveTheHoleItself) {
		ASSERT_DEATH({
			Board b;
			b.moveIntoHole({ 3, 3 });
			},
			"!isHole.cell."  // Can not have ( or ) in regexp.
		);
	}


}