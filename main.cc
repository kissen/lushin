#include <cstdio>
#include <iostream>

#include "assets.hh"
#include "gui.hh"

int main()
{
	gui::begin();

	while (true) {
		gui::update();
		gui::draw();
		gui::delay(60);
	}
}
