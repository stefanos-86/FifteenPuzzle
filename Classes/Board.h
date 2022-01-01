#pragma once

#include <array>

#include "Cell.h"

namespace fifteenPuzzle {

	/** Type of the values in the board. */
	using BoardValue = uint8_t;

	/** Represents the board of the Fifteen Puzzle.
	4x4 grid, a number (or hole) in each cell. */
	class Board {
	public:
		/** Conventional value to mark the hole in the board. 
		    By giving it a value bigger than the other numbers a completely solved puzzle is simply
			a sorted sequence. */
		static constexpr BoardValue HOLE = 16;

		/** The chances of this ever be modified to a value other than 4 are practically 0. 
		A magic number may be sufficient.*/
		static constexpr Coordinate SIZE = 4;

		Board();

		/** Returns the value at the given position. */
		BoardValue get(const Cell& cell) const noexcept;

		/** Change value in a cell. No safety checks, intended mostly to build test boards. */
		void unsafeSet(const Cell& cell, const BoardValue value);

		/** Exchanges the cell with the hole, if possible. Return the new cell where the hole was.
		Assumes the cell is next to the hole. Rerturns the same cell if the input cell can't be moved. */
		Cell moveIntoHole(const Cell& cell) noexcept;

		/** True if the cell is next to the hole. */
		bool canBeMoved(const Cell& cell) noexcept;

		/** Shuffles the board at random.
		    According to wikipedia, if the hole stays in the last place than there must be an 
			even number of inversions in the permutation between the starting position and the solution. 
			
			I am not happy with this interface. There is plenty of math involved and it can't easily be
			tested. */
		void shuffle() noexcept;

	/** Counts the number of inversion in a permutated puzzle. If they are even, then it is
		solvable.

		Not really meant for direct use, but the algorithm is complex and needs testing.

	It does it with a cubic cost algorithm. It will have to do for this exercise.
	Would it be better to try to construct a winning permutation directly? */
	bool isSolvable() const noexcept;

	/** Returns true if the puzzle is solved. */
	bool isSolved() const noexcept;

	private:
		static constexpr Coordinate BOARD_ELEMENTS = SIZE * SIZE;

		using BoardAsSequence = std::array<BoardValue, BOARD_ELEMENTS>;

		/** Simple 4x4 matrix, but put in a single dimension array. 
		    Many of the algorithms are easier to implement if the data is kept this way. */
		BoardAsSequence values;

		/** Asserts that the position is within the board. 
		   Uses assert, so... no except, but yes abort!
		   This kind of test allows to check and still have noexcept functions.
		   Also, a bad position is certainly a programming error, not an exceptional situation. But
		   this is debatable, I am not convinced myself. */
		void ensureGoodCell(const Cell& posistion) const noexcept; 

		/** True if the given position is the hole.*/
		bool isHole(const Cell& position) const noexcept;

		/** Exchanges the values in A and B. 
		    Does not check for illegal moves. */
		void swap(const Cell& A, const Cell& B) noexcept;


		/** Swap the 1 with the element immediately on the right or the left (but never with the hole). 
			It should therefore invert the parity of the permutation.
		*/
		void invertOne() noexcept;

		/** Returns the position of the cell in the unidimensional array. */
		size_t addressOf(const Cell& cell) const noexcept;

		/** Return the position of the hole, if it is abutting to the given cell.
		* Returns the cell itself if the hole in not next to it.
		* 
		* Building block for the can-move and acually-move algorithms.
		* 
		* The "return same cell" is a cheap way to handle the "not found" case without using
		* an extra boolean or a "null" cell. Inelegant, but works well enough in practice.
		*/
		Cell findHoleNextTo(const Cell& cell) noexcept;
	};
}