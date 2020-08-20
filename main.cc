#include <cstdio>
#include <iostream>

#include "assets.hh"
#include "gui.hh"

int main()
{
	gui::begin();
	void *ptr = assets::load_texture(assets::binary_bishop_black_png, assets::binary_bishop_black_png_end);
	printf("asset@%p\n", ptr);

	while (true) {
		gui::update();
		gui::draw();
	}
}
