#pragma once

#include <cstdint>

namespace gui
{
	/**
	 * Start showing the graphical interface.
	 */
	void begin();

	/**
	 * Update the graphical interface.
	 */
	void update();

	/**
	 * Draw the current state of the graphical user interface.
	 */
	void draw();

	/**
	 * Delay execution with the aim of reaching fps. If fps is 0,
	 * execution returns right away without any delay.
	 */
	void delay(int8_t fps);
}
