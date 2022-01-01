#pragma once

#include <string>

#include "cocos2d.h"

/** Constants for various numbers and values used in the whole UI. More or less the whole
    "look and feel" should be there. */

namespace UI {
	static const std::string FONT = "fonts/Marker Felt.ttf";
	static const std::string BLOCK_BACKGROUND = "Square.png";
	static const std::string ENDGAME_MESSAGE = "Done!";
	static const cocos2d::Color4B BLOCK_FONT_COLOR(255, 0, 0, 255);

	static constexpr uint8_t TEXT_SIZE = 85;
	static constexpr uint8_t CLOCK_TEXT_SIZE = TEXT_SIZE / 2;

	/** Z-order "levels". */
	/** @{ */
	static constexpr uint8_t BOTTOM = 0;
	static constexpr uint8_t POP_OUT = BOTTOM + 1;
	static constexpr uint8_t TOP = POP_OUT + 1;
	/** @} */

	static constexpr float WAVE_SEPARATION_S = 0.2f;

	static constexpr float POP_OUT_SCALE = 1.2f;
	static constexpr float POP_OUT_DURATION_S = 1.0f;
	static constexpr float IMMEDIATELY_S = 0.0f;

	static constexpr float MOVE_DURATION_S = 0.5f;
	
}