#include <cstdio>

#include "assets.hh"
#include "gui.hh"

int main()
{
	printf("size=%p\n", assets::binary_bishop_black_png_size);
	printf("start=%p\n", assets::binary_bishop_black_png);
	printf("end=%p\n", assets::binary_bishop_black_png_end);

	gui::begin();
	void *ptr = assets::load_texture(assets::binary_bishop_black_png, assets::binary_bishop_black_png_size);
	printf("asset@%p\n", ptr);

	while (true) {
		gui::update();
		gui::draw();
	}
}
