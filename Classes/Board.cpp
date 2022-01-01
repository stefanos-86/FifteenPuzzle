#include "Board.h"

#include <algorithm>
#include <iterator>
#include <cassert>
#include <string>

namespace fifteenPuzzle {
	Board::Board()
	{
		for (BoardValue value = 1; value <= BOARD_ELEMENTS; ++value)
			values[value - 1] = value;
		values[BOARD_ELEMENTS - 1] = HOLE;
	}

	BoardValue Board::get(const Cell& cell) const noexcept
	{
		ensureGoodCell(cell);
		return values[addressOf(cell)];
	}

	void Board::unsafeSet(const Cell& cell, const BoardValue value)
	{
		values[addressOf(cell)] = value;
	}

	Cell Board::moveIntoHole(const Cell& cell) noexcept
	{
		ensureGoodCell(cell);
		assert(!isHole(cell));  // It is not legal to move the hole into itself!
		assert(canBeMoved(cell)); // It makes no sense to move otherwise.

		Cell hole = findHoleNextTo(cell);
		swap(cell, hole);
		return hole;  // By now, ex-hole, filled by the moved tile.
	}

	bool Board::canBeMoved(const Cell& cell) noexcept
	{
		ensureGoodCell(cell);
		assert(!isHole(cell));  // Logical error, why are you asking to move the hole?

		// Found the hole next to it? Then can move.
		return cell != findHoleNextTo(cell);
	}

	void Board::shuffle() noexcept
	{
		assert(get({ 3, 3 }) == HOLE);  // If not, the shuffle has to account for the hole.

		auto upToHole = values.begin(); // ... or shuffle everything, the put the hole back at the end?
		std::advance(upToHole, values.size() - 1);  // Notice this assumes the hole is at the end.
		std::random_shuffle(values.begin(), upToHole);

		if (!isSolvable())  // Should pass the linear sequence here and avoid yet another copy of the values.
			invertOne();

		assert(isSolvable());
	}

	bool Board::isHole(const Cell& position) const noexcept
	{
		return Board::HOLE == get(position);
	}

	void Board::swap(const Cell& A, const Cell& B) noexcept
	{
		const BoardValue temp = values[addressOf(A)];
		values[addressOf(A)] = values[addressOf(B)];
		values[addressOf(B)] = temp;
	}

	bool Board::isSolvable() const noexcept
	{
		// Max 105 inversion (as per https://en.wikipedia.org/wiki/Inversion_(discrete_mathematics))
		uint8_t inversions = 0;

		// For each i < j... (and i == j, but should fail to see it an inversion).
		for (BoardValue i = 1; i <= 15; ++i)
			for (BoardValue j = i; j <= 15; ++j) {
				const auto pi = std::find(values.begin(), values.end(), i);
				const auto pj = std::find(values.begin(), values.end(), j);

				if (std::distance(values.begin(), pi) > std::distance(values.begin(), pj))
					++inversions;
			}


		return (inversions % 2 == 0);
	}

	bool Board::isSolved() const noexcept
	{
		return std::is_sorted(values.begin(), values.end());
	}

	void Board::invertOne() noexcept
	{
		for (Coordinate row = 0; row < SIZE; ++row)
			for (Coordinate col = 0; col < SIZE; ++col)
				while (values[addressOf({ row, col })] != 1) {
					if (col > 0)
						swap({ row, col }, { row, col - 1u });
					else // Col == 0, therefore I can move the value to the right.
						swap({ row, col }, { row, col + 1u });

					return; // Do not visit the matrix after finding the 1.
				}
	}

	size_t Board::addressOf(const Cell& cell) const noexcept
	{
		ensureGoodCell(cell);

		return cell.row * SIZE + cell.col;
	}

	Cell Board::findHoleNextTo(const Cell& cell) noexcept
	{
		ensureGoodCell(cell);

		// There are only 4 places where the hole could be...
		// Notice that the neighbors' coordinates may underflow or be out of the board, but if
		// that happen, they are not used (and I save one level of IF nesting).
		const Cell onTheRight{ cell.row, cell.col + 1u };
		if (cell.col < SIZE - 1 && isHole(onTheRight)) 
			return onTheRight;

		const Cell onTheLeft{ cell.row, cell.col - 1u };
		if (cell.col > 0 && isHole(onTheLeft)) 
			return onTheLeft;

		const Cell below{ cell.row + 1u, cell.col };
		if (cell.row < SIZE - 1 && isHole(below)) 
			return below;

		const Cell above{ cell.row - 1u, cell.col };
		if (cell.row > 0 && isHole(above)) 
			return above;

		// No near hole, return the same block.
		return cell;
	}


	void Board::ensureGoodCell(const Cell& position) const noexcept
	{
		// Pity those are macroes, so we can't print the wrong value
		// not even with this nice trick: https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert
		assert(position.row < SIZE);
		assert(position.col < SIZE);
	}
	
}