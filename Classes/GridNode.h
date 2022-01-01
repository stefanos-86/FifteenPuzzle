#pragma once

#include "cocos2d.h"

#include <cstdint>

namespace fifteenPuzzle { class Cell; }

class GridNode : public cocos2d::Node {
public:
	static GridNode* create(const float side, const uint8_t subdivisions);

	/** This constructor only does some math and saves its results. it should be safe to use
	    before calling init. */
	GridNode(const float side, const uint8_t subdivisions);

	/** Auto-scale the child to fit in the grid cell and auto-translate it to the right place. */
	void addChild(cocos2d::Node* widget, const uint8_t row, const uint8_t column);

	/** Returns the node at the given position. 
	    Woult it be more convenient to use the physics system collision detection for this?*/
	cocos2d::Node* blockAt(const cocos2d::Vec2& locationInView) const;

	cocos2d::Node* blockAtLocal(const cocos2d::Vec2& locationInLocalSpace) const;

	/** Returns the cell coordinates of the given location. */
	const fifteenPuzzle::Cell cellAt(const cocos2d::Vec2& locationInView) const;

	/** Returns the center of the given cell in local coordinates of this node. */
	cocos2d::Vec2 positionOf(const fifteenPuzzle::Cell& cell) const;

private:
	const float side;
	const uint8_t subdivisions;
	const float elementSide;
};