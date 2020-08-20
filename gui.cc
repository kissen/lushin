#include <cassert>
#include <iostream>
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
	0,
	0,
	0,
	255
};

static const SDL_Color SDL_WHITE = {
	255,
	255,
	255,
	255
};

static void fail_with_sdl_error(const char *funcname)
{
	const char *err = SDL_GetError();
	fprintf(stderr, "lushin: gfx: %s: %s\n", funcname, err);
	exit(EXIT_FAILURE);
}

#define fail_with_sdl_error() fail_with_sdl_error(__func__)

static void set_render_color(const SDL_Color &color)
{
	if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
		fail_with_sdl_error();
	}
}

static void begin_drawing()
{
	set_render_color(SDL_BLACK);

	if (SDL_RenderClear(renderer)) {
		fail_with_sdl_error();
	}
}

static void end_drawing()
{
	SDL_RenderPresent(renderer);
}

static void draw_rectangle(uint8_t x, uint8_t y, const SDL_Color &color)
{
	const SDL_Rect rectangle = {
		x * CELL_DIM, y * CELL_DIM,
		CELL_DIM, CELL_DIM
	};

	set_render_color(color);

	if (SDL_RenderFillRect(renderer, &rectangle)) {
		fail_with_sdl_error();
	}
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

static void draw_background()
{
	static const SDL_Color background_colors[] = {
		SDL_WHITE, SDL_BLACK
	};

	size_t cellid = 0;

	for (uint8_t y = 0; y < 8; ++y) {
		for (uint8_t x = 0; x < 8; ++x) {
			const size_t idx = cellid++ % 2;
			const SDL_Color &color = background_colors[idx];
			draw_rectangle(x, y, color);
		}

		cellid += 1;
	}
}

void gui::draw()
{
	assert(window);

	begin_drawing();

	draw_background();

	end_drawing();
}

void gui::delay(int8_t fps)
{
	if (fps == 0) {
		return;
	}

	// todo
}
