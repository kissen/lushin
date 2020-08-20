#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdio>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <vector>

#include <SDL2/SDL.h>

#include "assets.hh"
#include "chess.hh"
#include "gui.hh"

//
// global state of the graphical interface
//

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *textures[2][6];

//
// state that gets written in update() and read in draw()
//

static chess::Board mboard;

static uint32_t mticks;

static struct {
	int x;
	int y;
	bool left_down;
	bool right_down;
	bool left_clicked;
	bool right_clicked;
} mmouse;

/* Currently selected position in board coordinates. Might be nullptr. */
static const chess::Pos *m_selected_pos;

/* Currently selected piece. Might be nullptr. */
static const chess::Piece *m_selected_piece;

/* Supported next moves for m_selected_piece. Might be nulltpr. */
static const std::vector<chess::Pos> *m_valid_next_moves_for_selected;

//
// SDL helpers (constants and functions)
//

#define CELL_DIM 64

static const SDL_Color SDL_BLACK = {
	0x00, 0x00, 0x00, 0xff
};

static const SDL_Color SDL_WHITE = {
	0xff, 0xff, 0xff, 0xff
};

static const SDL_Color SDL_HIGHLIGHT = {
	0xff, 0xf7, 0x7a, 0xff
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

static SDL_Color blend_average(const SDL_Color &color0, const SDL_Color &color1)
{
	const uint16_t r = static_cast<uint16_t>(color0.r) + static_cast<uint16_t>(color1.r);
	const uint16_t g = static_cast<uint16_t>(color0.g) + static_cast<uint16_t>(color1.g);
	const uint16_t b = static_cast<uint16_t>(color0.b) + static_cast<uint16_t>(color1.b);
	const uint16_t a = static_cast<uint16_t>(color0.a) + static_cast<uint16_t>(color1.a);

	const uint8_t rfixed = static_cast<uint8_t>(r / 2);
	const uint8_t gfixed = static_cast<uint8_t>(g / 2);
	const uint8_t bfixed = static_cast<uint8_t>(b / 2);
	const uint8_t afixed = static_cast<uint8_t>(a / 2);

	return SDL_Color {
		rfixed, gfixed, bfixed, afixed
	};
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

static void update_time()
{
	mticks = SDL_GetTicks();
}

static void update_mouse_position()
{
	const uint32_t mstate = SDL_GetMouseState(&mmouse.x, &mmouse.y);

	const bool left_was_down = mmouse.left_down;
	const bool right_was_down = mmouse.right_down;

	mmouse.left_down = mstate & SDL_BUTTON(1);
	mmouse.right_down = mstate & SDL_BUTTON(3);

	mmouse.left_clicked = left_was_down && (! mmouse.left_down);
	mmouse.right_clicked = right_was_down && (! mmouse.right_down);
}

static chess::Pos mouse_selection()
{
	const uint8_t xscaled = mmouse.x / CELL_DIM;
	const uint8_t yscaled = mmouse.y / CELL_DIM;

	return {xscaled, yscaled};
}

static void update_selection()
{
	static chess::Pos current_selection;

	current_selection = mouse_selection();
	m_selected_pos = &current_selection;

	const chess::Piece piece = mboard.at(current_selection);

	// if piece not present, no suggestions

	if (!piece.present) {
		m_selected_piece = nullptr;
		m_valid_next_moves_for_selected = nullptr;
		return;
	}

	// if an actual piece is suggested, highlight suggestions

	static std::vector<chess::Pos> next_moves;

	m_selected_piece = &piece;
	next_moves = chess::valid_next_positions(mboard, current_selection);
	m_valid_next_moves_for_selected = &next_moves;
}

void gui::update()
{
	SDL_PumpEvents();

	update_time();
	update_mouse_position();
	update_selection();
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
			SDL_Color color = background_colors[idx];

			const chess::Pos xy = {x, y};
			bool requires_highlight = false;

			if (m_selected_pos && *m_selected_pos == xy) {
				requires_highlight = true;
			}

			if (m_valid_next_moves_for_selected) {
				const auto &moves = *m_valid_next_moves_for_selected;
				if (std::find(begin(moves), end(moves), xy) != end(moves)) {
					requires_highlight =  true;
				}
			}

			if (requires_highlight) {
				color = blend_average(color, SDL_HIGHLIGHT);
			}

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

	const int64_t ideal_delay = int64_t(1000) / static_cast<int64_t>(fps);
	const int64_t actual_delay = ideal_delay - (static_cast<int64_t>(SDL_GetTicks()) - static_cast<int64_t>(mticks));

	if (actual_delay > 0 && actual_delay < UINT32_MAX) {
		const uint32_t actual_delay32 = static_cast<uint32_t>(actual_delay);
		SDL_Delay(actual_delay32);
	}
}

// this sucks
SDL_Renderer *gui::get_renderer()
{
	assert(renderer);
	return renderer;
}
