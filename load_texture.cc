#include <cassert>
#include <climits>
#include <cstddef>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "assets.hh"
#include "gui.hh"

//
// SDL helpers
//

static void fail_with_sdl_error(const char *funcname)
{
	const char *err = SDL_GetError();
	fprintf(stderr, "lushin: sdl_image: %s: %s\n", funcname, err);
	exit(EXIT_FAILURE);
}

#define fail_with_sdl_error() fail_with_sdl_error(__func__)

static void fail_with_img_error(const char *funcname)
{
	const char *err = IMG_GetError();
	fprintf(stderr, "lushin: load_texture: sdl_image: %s: %s\n", funcname, err);
	exit(EXIT_FAILURE);
}

#define fail_with_img_error() fail_with_img_error(__func__)

//
// Implementation
//

/**
 * Return difference between start and end in bytes.
 */
static size_t diff_between(const uint8_t *start, const uint8_t *end)
{
	assert(end >= start);

	ptrdiff_t diff = end - start;
	return diff;
}

/**
 * Return difference between start and end in bytes.
 */
static int int_diff_between(const uint8_t *start, const uint8_t *end)
{
	const size_t size = diff_between(start, end);
	assert(size > 0);
	assert(size < INT_MAX);

	return static_cast<int>(size);
}

SDL_Texture *assets::load_texture(uint8_t *startptr, uint8_t *endptr)
{
	assert(startptr);
	assert(endptr);

	// init the subsystem, make sure we have PNG support
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		fail_with_img_error();
	}

	// get the size
	const int asset_size = int_diff_between(startptr, endptr);

	// mmap the in-memory asset
	SDL_RWops *rwop;
	if (!(rwop = SDL_RWFromMem(startptr, asset_size))) {
		fail_with_sdl_error();
	}

	// load as surface; IMG_Load_RW also takes care of clsoing rwop
	SDL_Surface *surface;
	if (!(surface = IMG_Load_RW(rwop, 1))) {
		fail_with_sdl_error();
	}

	// convert surface to texture
	SDL_Texture *texture;
	if (!(texture = SDL_CreateTextureFromSurface(gui::get_renderer(), surface))) {
		fail_with_sdl_error();
	}

	// we do not need the surface anymore
	SDL_FreeSurface(surface);

	return texture;
}
