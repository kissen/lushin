#include <cstdio>

#include "gui.hh"

int main()
{
	puts("hello, world");

	gui::begin();

	while (true) {
		gui::update();
		gui::draw();
	}
}
