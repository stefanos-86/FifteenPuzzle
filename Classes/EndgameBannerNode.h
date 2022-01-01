#pragma once

#include <string>

#include "cocos2d.h"


class EndgameBannerNode : public cocos2d::Sprite {
public:
	static EndgameBannerNode* create(const std::string& backgroundImage,
									 const std::string& victoryMessage,
									 const const float fallingDistance);

	EndgameBannerNode();
	~EndgameBannerNode();

	void fallDownAnimation();

private:
	cocos2d::Sequence* fallDownAnimationSequence;
	cocos2d::Label* message;

	void prepareAnimations(const float fallingDistance);

	void writeVictoryMessage(const std::string& message);


	/** Can not use rule of 0, cache some pointers. But do not need any of those. */
	/** @{*/
	EndgameBannerNode(const EndgameBannerNode& other) = delete;
	EndgameBannerNode(EndgameBannerNode&& other) = delete;
	EndgameBannerNode& operator=(const EndgameBannerNode& other) = delete;
	EndgameBannerNode& operator=(EndgameBannerNode&& other) = delete;
	/**@}*/
};