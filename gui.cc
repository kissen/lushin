#include <cassert>
#include <cstdio>
#include <iostream>
#include <signal.h>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "assets.hh"
#include "chess.hh"
#include "gui.hh"

//
// global state of the graphical interface
//

static chess::Board mboard;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *textures[2][6];

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
	fprintf(stderr, "lushin: gui: %s: %s\n", funcname, err);
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

static SDL_Texture *texture_for(chess::Color color, chess::Kind kind)
{
	const size_t idx0 = static_cast<size_t>(color);
	const size_t idx1 = static_cast<size_t>(kind);

	return textures[idx0][idx1];
}

static SDL_Texture **texture_ref_for(chess::Color color, chess::Kind kind)
{
	const size_t idx0 = static_cast<size_t>(color);
	const size_t idx1 = static_cast<size_t>(kind);

	return &textures[idx0][idx1];
}

static void load_static_textures()
{
	using namespace assets;
	using namespace chess;

	*texture_ref_for(Color::Black, Kind::King) = load_texture(
		binary_king_black_png, binary_king_black_png_end
	);

	*texture_ref_for(Color::Black, Kind::Queen) = load_texture(
		binary_queen_black_png, binary_queen_black_png_end
	);

	*texture_ref_for(Color::Black, Kind::Rook) = load_texture(
		binary_rook_black_png, binary_rook_black_png_end
	);

	*texture_ref_for(Color::Black, Kind::Knight) = load_texture(
		binary_knight_black_png, binary_knight_black_png_end
	);

	*texture_ref_for(Color::Black, Kind::Bishop) = load_texture(
		binary_bishop_black_png, binary_bishop_black_png_end
	);

	*texture_ref_for(Color::Black, Kind::Pawn) = load_texture(
		binary_pawn_black_png, binary_pawn_black_png_end
	);

	*texture_ref_for(Color::White, Kind::King) = load_texture(
		binary_king_white_png, binary_king_white_png_end
	);

	*texture_ref_for(Color::White, Kind::Queen) = load_texture(
		binary_queen_white_png, binary_queen_white_png_end
	);

	*texture_ref_for(Color::White, Kind::Rook) = load_texture(
		binary_rook_white_png, binary_rook_white_png_end
	);

	*texture_ref_for(Color::White, Kind::Knight) = load_texture(
		binary_knight_white_png, binary_knight_white_png_end
	);

	*texture_ref_for(Color::White, Kind::Bishop) = load_texture(
		binary_bishop_white_png, binary_bishop_white_png_end
	);

	*texture_ref_for(Color::White, Kind::Pawn) = load_texture(
		binary_pawn_white_png, binary_pawn_white_png_end
	);
}

void gui::begin()
{
	assert(!window);

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

	// load in all assets into gpu memory
	load_static_textures();

	// init game state
	mboard = chess::Board::initial();
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

	for (uint8_t x = 0; x < 8; ++x) {
		for (uint8_t y = 0; y < 8; ++y) {
			const size_t idx = cellid++ % 2;
			const SDL_Color &color = background_colors[idx];
			draw_rectangle(x, y, color);
		}

		cellid += 1;
	}
}

static void draw_piece_at(uint8_t x, uint8_t y)
{
	const chess::Piece &piece = mboard.at({x, y});

	if (!piece.present) {
		return;
	}

	SDL_Texture *texture = texture_for(piece.color, piece.kind);

	const SDL_Rect dstrect = {
		x * CELL_DIM, y * CELL_DIM,
		CELL_DIM, CELL_DIM
	};

	if (SDL_RenderCopy(renderer, texture, NULL, &dstrect)) {
		fail_with_sdl_error();
	}
}

static void draw_pieces()
{
	for (uint8_t x = 0; x < 8; ++x) {
		for (uint8_t y = 0; y < 8; ++y) {
			draw_piece_at(x, y);
		}
	}
}

void gui::draw()
{
	assert(window);

	begin_drawing();

	draw_background();
	draw_pieces();

	end_drawing();
}

void gui::delay(int8_t fps)
{
	if (fps == 0) {
		return;
	}

	// todo
}

// this sucks
SDL_Renderer *gui::get_renderer()
{
	assert(renderer);
	return renderer;
}
