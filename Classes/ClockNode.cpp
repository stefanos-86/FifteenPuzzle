#include "ClockNode.h"

#include <cstdlib>
#include <limits>

USING_NS_CC;

ClockNode* ClockNode::CreateWithTTF(const std::string& fontFile, const float fontSize)
{
	auto ret = new (std::nothrow) ClockNode();

	if (ret && ret->initWithTTF("CANARY", fontFile, fontSize))
	{
		ret->updateLabel();
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

ClockNode::ClockNode() : secondsSinceStart(0)
{
}

void ClockNode::start()
{
	assert(secondsSinceStart == 0 && "Clock started twice.");

	schedule(CC_SCHEDULE_SELECTOR(ClockNode::oneSecondTick), 1.0f);
}

void ClockNode::stop()
{
	unschedule(CC_SCHEDULE_SELECTOR(ClockNode::oneSecondTick));
}

void ClockNode::oneSecondTick(float dt)
{
	assert(0.95 < dt && dt < 1.05); // Give it some room to account for float rounding. Enough to account for big coding errors.
	assert(secondsSinceStart < (uint32_t) std::numeric_limits<uint32_t>::max);  // It may take a while, but it could overflow.

	secondsSinceStart += 1;
	updateLabel();
}

void ClockNode::updateLabel()
{
	const auto minutesAndSeconds = std::div(secondsSinceStart, 60);
	const auto hoursAndMinutes = std::div(minutesAndSeconds.quot, 60);

	// I have a particular aversion for stream output modifiers. This turned out to be just as bad.
	// Also, it seems I don't have the _s version of the function.
	// Maybe I can set a string to HH:MM:SS and overwrite the letters. Needs a little study. There may be a nicer way.
	const size_t bufferSize = 8    // HH:MM:SS
							  + 1; // /0
	char timeBuffer[bufferSize];
	const int writtenChars = std::snprintf(timeBuffer, bufferSize,
		"%.2i:%.2i:%.2i", hoursAndMinutes.quot, hoursAndMinutes.rem, minutesAndSeconds.rem);

	assert(writtenChars == std::strlen("HH:MM:SS"));  // Happy with unsafe strlen. Is in assert, input is hardcoded.

	setString(timeBuffer);
}
