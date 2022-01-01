#pragma once

#include <cstdint>

#include "cocos2d.h"


/** Tricked-out label that keeps and displays a clock in HH:MM:SS format. 
    It does not return the time. It is for display only, no other part of the code needs to access
	the time. */
class ClockNode : public cocos2d::Label {
public:
	static ClockNode * CreateWithTTF(const std::string& font, const float size);

	ClockNode();

	/** Starts the clock. */ 
	void start();

	/** Stops the clock. Forever. You are not supposed to restart it.
	    Notice that the user can not pause the game. If he/she could, it could stop the clock and think. */
	void stop();

private:
	/** Not atomic. Only this object accesses it, once per second. The likelyhood of two updates
	"piling up" because one of them took a second is minimal. The same holds for the label text. */
	uint32_t secondsSinceStart;
	
	/** Callback every second. */
	void oneSecondTick(float dt);
	
	/** Prints the current time as the label text. */
	void updateLabel();
	
};