#include <cassert>
#include <cstdio>
#include <signal.h>

#include <SDL2/SDL.h>

#include "chess.hh"
#include "gui.hh"

using namespace chess;

//
// global state of the graphical interface
//

static Board mboard;
static SDL_Window *window;
static SDL_Renderer *renderer;

//
// SDL helpers (constants and functions)
//

#define CELL_DIM 64

static const SDL_Color SDL_BLACK = {
	32,
	194,
	14,
	255
};

static const SDL_Color SDL_WHITE = {
	0,
	0,
	0,
	255
};

static void fail_with_sdl_error(const char *funcname)
{
	const char *err = SDL_GetError();
	fprintf(stderr, "lushin: gfx: %s: %s\n", funcname, err);
	exit(EXIT_FAILURE);
}

#define fail_with_sdl_error() fail_with_sdl_error(__func__)

static void set_render_color(const SDL_Color *color)
{
	if (SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a)) {
		fail_with_sdl_error();
	}
}

static void begin_drawing(void)
{
	set_render_color(&SDL_BLACK);

	if (SDL_RenderClear(renderer)) {
		fail_with_sdl_error();
	}
}

static void end_drawing(void)
{
	SDL_RenderPresent(renderer);
}
//
// implementation of gui.hh functions
//

void gui::begin()
{
	if (window) {
		return;
	}

	// init sdl

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
		fail_with_sdl_error();
	}

	// reset the Ctrl+C signal handler; don't let
	// sdl capture it

	signal(SIGINT, SIG_DFL);

	// create the window

	window = SDL_CreateWindow(
		"lushin",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		8 * CELL_DIM, 8 * CELL_DIM, 0
	);

	if (!window) {
		fail_with_sdl_error();
	}

	// create the renderer

	if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
		fail_with_sdl_error();
	}
}

void gui::update()
{
}

void gui::draw()
{
	assert(window);

	begin_drawing();
	end_drawing();
}

void gui::delay(int8_t fps)
{
	if (fps == 0) {
		return;
	}

	// todo
}
