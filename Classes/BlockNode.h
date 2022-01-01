#pragma once

#include <string>

#include "cocos2d.h"


class BlockNode : public cocos2d::Sprite {
public:
	static const std::string BLOCK_MOVED_EVENT;

	static BlockNode* create(const std::string& backgroundImage, const uint8_t number);

	BlockNode();
	~BlockNode();

	void blockNodeInit(const uint8_t number);

	void popOutAnimation(const float delay);
	void moveAnimation(const cocos2d::Vec2& destination);


private:

	/** What to do when there is a call to idle animation.
	Autoreleased, but cached once and forever. Must be retain'ed release'd because it is not a child
	of anything...*/
	cocos2d::Sequence* popOutAnimationSequence;

	/** Action to raise the end of movement event.

	This callback is node-independant. Could it be "more cached"? As is, there is a copy
	in every node object, giving 14 useless copies.
	*/
	cocos2d::CallFunc* endMove;

	cocos2d::Label* number;

	void prepareAnimations();

	void writeNumber(const uint8_t number);

	/** Can not use rule of 0, cache some pointers. But do not need any of those. */
	/** @{*/
	BlockNode(const BlockNode& other) = delete;
	BlockNode(BlockNode&& other) = delete;
	BlockNode& operator=(const BlockNode& other) = delete;
	BlockNode& operator=(BlockNode&& other) = delete;
	/**@}*/
};