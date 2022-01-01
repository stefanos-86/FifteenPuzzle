#include "BlockNode.h"

#include "UIConstants.h"

USING_NS_CC;

const std::string BlockNode::BLOCK_MOVED_EVENT = "end_of_movement";

BlockNode* BlockNode::create(const std::string& backgroundImage, const uint8_t number)
{
	// From the equivalent in Sprite.
	BlockNode* pRet = new BlockNode();
	
	if (pRet && pRet->initWithFile(backgroundImage))
	{
		// I do not date tamper with the Init* functions nor do anything before a call to init.
		pRet->blockNodeInit(number);

		pRet->autorelease();
		return pRet;
	}

	delete pRet;
	return nullptr;
}

BlockNode::BlockNode():
	popOutAnimationSequence(nullptr),
	endMove(nullptr),
	number(nullptr)
{
}

BlockNode::~BlockNode()
{
	popOutAnimationSequence->release();
	endMove->release();
}

void BlockNode::blockNodeInit(const uint8_t number)
{
	prepareAnimations();
	writeNumber(number);
}

void BlockNode::popOutAnimation(const float delay)
{
	assert(getNumberOfRunningActions() == 0);

	auto waitBeforeStarting = DelayTime::create(delay);
	auto waitAndPulse = Sequence::create(waitBeforeStarting, popOutAnimationSequence, nullptr);

	runAction(waitAndPulse);
}

void BlockNode::moveAnimation(const Vec2& destination)
{
	assert(getNumberOfRunningActions() == 0);

	// Can not cache. Destination may be anything.
	auto move = MoveTo::create(UI::MOVE_DURATION_S, destination);
	auto moveWithSignal = Sequence::create(move, endMove, nullptr);

	runAction(moveWithSignal);
}

void BlockNode::prepareAnimations()
{
	assert(popOutAnimationSequence == nullptr && "Cached animation prepared twice.");

	auto bring_up = CallFuncN::create([](Node* n) {n->setZOrder(UI::POP_OUT); });
	auto bring_down = CallFuncN::create([](Node* n) {n->setZOrder(UI::BOTTOM); });


	constexpr float stepDuration = UI::POP_OUT_DURATION_S / 5;

	auto inflate = ScaleBy::create(stepDuration * 2, UI::POP_OUT_SCALE);
	auto deflate = ScaleBy::create(stepDuration * 2, 1 / UI::POP_OUT_SCALE);

	auto delay = DelayTime::create(stepDuration);

	popOutAnimationSequence = Sequence::create(bring_up, inflate, delay, deflate, bring_down, nullptr);
	popOutAnimationSequence->retain();

	endMove = CallFunc::create([&]() {
		EventCustom event(BlockNode::BLOCK_MOVED_EVENT);
		_eventDispatcher->dispatchEvent(&event);
		});
	endMove->retain();
}


void BlockNode::writeNumber(const uint8_t number)
{
	assert(this->number == nullptr); // It makes no sense to write it twice.
	assert(number < 16);

	const auto blockSize = getContentSize().width; // Assume a square block.
	const auto fontSize = 0.9f * blockSize;

	this->number = Label::createWithTTF(std::to_string(number), UI::FONT, fontSize);

	// Center the text "box" in the background "box".
	const auto textSize = this->number->getContentSize();
	const auto padToCenter_h = (blockSize - textSize.width) / 2;
	const auto padToCenter_v = (blockSize - textSize.height) / 2;

	assert(padToCenter_h > 0 && padToCenter_v > 0);

	this->number->setPosition(padToCenter_h, padToCenter_v);
	this->number->setAnchorPoint(Vec2(0, 0));
	this->number->setTextColor(UI::BLOCK_FONT_COLOR);
	addChild(this->number);
}
