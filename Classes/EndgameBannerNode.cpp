#include "EndgameBannerNode.h"

#include "UIConstants.h"

USING_NS_CC;

EndgameBannerNode* EndgameBannerNode::create(const std::string& backgroundImage,
	const std::string& victoryMessage,
	const const float fallingDistance)
{
	// From the equivalent in Sprite and BlockNode. So much code duplication... TODO: see if avoidable. Cache animations in another way?
	EndgameBannerNode* pRet = new EndgameBannerNode();
	
	if (pRet && pRet->initWithFile(backgroundImage))
	{
		// I do not date tamper with the Init* functions nor do anything before a call to init.
		pRet->prepareAnimations(fallingDistance);
		pRet->writeVictoryMessage(victoryMessage);

		pRet->autorelease();
		return pRet;
	}

	delete pRet;
	return nullptr;
}

EndgameBannerNode::EndgameBannerNode():
	fallDownAnimationSequence(nullptr),
	message(nullptr)
{
}

EndgameBannerNode::~EndgameBannerNode()
{
	fallDownAnimationSequence->release();
}



void EndgameBannerNode::fallDownAnimation()
{
	assert(getNumberOfRunningActions() == 0);

	runAction(fallDownAnimationSequence);
}


void EndgameBannerNode::prepareAnimations(const float fallingDistance)
{
	assert(fallDownAnimationSequence == nullptr && "Animation already there.");

	auto fall = MoveBy::create(1, Vec2(0, -fallingDistance));
	auto bouncyFall = EaseBounceOut::create(fall->clone());

	fallDownAnimationSequence = Sequence::create(bouncyFall, nullptr);
	fallDownAnimationSequence->retain();
}

void EndgameBannerNode::writeVictoryMessage(const std::string& message)
{
	assert(this->message == nullptr);

	this->message = Label::createWithTTF(message, UI::FONT, UI::TEXT_SIZE);
	this->message->setPosition(0, 0);
	this->message->setAnchorPoint(Vec2(0, 0));
	this->message->setTextColor(UI::BLOCK_FONT_COLOR);
	addChild(this->message);

	float stretchToFit_h = this->message->getContentSize().width / getContentSize().width;
	float stretchToFit_v = this->message->getContentSize().height / getContentSize().height;

	// Make room for the text. Counter-shrink the label (It may have been more intelligent
	// to derive this widget from Label and add the background as a Sprite).
	this->setScale(stretchToFit_h, stretchToFit_v);
	this->message->setScale(1 / stretchToFit_h, 1 / stretchToFit_v);
}


