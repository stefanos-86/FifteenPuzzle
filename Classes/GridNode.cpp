#include "GridNode.h"

#include "Cell.h"

USING_NS_CC;

GridNode* GridNode::create(const float width, const uint8_t subdivisions)
{
	//assert non-zero params etcetera.

	GridNode* pRet = new GridNode(width, subdivisions);
	
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}

	delete pRet;
	return nullptr;
}

GridNode::GridNode(const float side, const uint8_t subdivisions) :
	Node(),
	side(side),
	subdivisions(subdivisions),
	elementSide(side / subdivisions)
{
}

void GridNode::addChild(cocos2d::Node* widget, const uint8_t row, const uint8_t column)
{
	assert(widget != nullptr);
	assert(row < subdivisions);
	assert(column < subdivisions);

	// Scale the widget to fit the space, and square it off.
	const auto widgetExtent = widget->getContentSize();
	const float scale_x = elementSide / widgetExtent.width;
	const float scale_y = elementSide / widgetExtent.height;
	widget->setScale(scale_x, scale_y);

	// Translate to its spot. Notice that the grid grows downward. Y = 0 must be at the top.
	// Could position easily with 
	//		widget->setAnchorPoint(Vec2(0, 1));
	// but then the animation would be anchored there and changing the anchor point also changes the
	// object position. Tweak by an half side to compensate (see code of positionOf).
	const Vec2 position = positionOf({row, column});
	widget->setPosition(position);

	Node::addChild(widget);
}

cocos2d::Node* GridNode::blockAt(const cocos2d::Vec2& locationInView) const
{
	const auto locationInLocalSpace = convertToNodeSpace(locationInView);
	return blockAtLocal(locationInLocalSpace);
}

cocos2d::Node* GridNode::blockAtLocal(const cocos2d::Vec2& locationInLocalSpace) const
{
	for (auto* cell : getChildren()) {
		const auto extent = cell->getBoundingBox();
		if (extent.containsPoint(locationInLocalSpace))
			return cell;
	}

	return nullptr;
}

const fifteenPuzzle::Cell GridNode::cellAt(const cocos2d::Vec2& locationInView) const
{
	const auto localCoordinates = convertToNodeSpace(locationInView);

	const fifteenPuzzle::Coordinate column = std::floor(localCoordinates.x / elementSide);
	const fifteenPuzzle::Coordinate row = std::floor((side - localCoordinates.y) / elementSide);

	return { row, column };
}

cocos2d::Vec2 GridNode::positionOf(const fifteenPuzzle::Cell& cell) const
{
	const float x =        elementSide * cell.col + elementSide / 2;
	const float y = side - elementSide * cell.row - elementSide / 2;
	return cocos2d::Vec2(x, y);
}
