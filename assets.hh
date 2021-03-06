#pragma once

#include <cstdint>
#include <SDL2/SDL.h>

namespace assets
{
	extern "C"
	{
		extern uint8_t binary_bishop_black_png[] asm("_binary___assets_bishop_black_png_start");
		extern uint8_t binary_bishop_black_png_end[] asm("_binary___assets_bishop_black_png_end");

		extern uint8_t binary_bishop_white_png[] asm("_binary___assets_bishop_white_png_start");
		extern uint8_t binary_bishop_white_png_end[] asm("_binary___assets_bishop_white_png_end");

		extern uint8_t binary_king_black_png[] asm("_binary___assets_king_black_png_start");
		extern uint8_t binary_king_black_png_end[] asm("_binary___assets_king_black_png_end");

		extern uint8_t binary_king_white_png[] asm("_binary___assets_king_white_png_start");
		extern uint8_t binary_king_white_png_end[] asm("_binary___assets_king_white_png_end");

		extern uint8_t binary_knight_black_png[] asm("_binary___assets_knight_black_png_start");
		extern uint8_t binary_knight_black_png_end[] asm("_binary___assets_knight_black_png_end");

		extern uint8_t binary_knight_white_png[] asm("_binary___assets_knight_white_png_start");
		extern uint8_t binary_knight_white_png_end[] asm("_binary___assets_knight_white_png_end");

		extern uint8_t binary_pawn_black_png[] asm("_binary___assets_pawn_black_png_start");
		extern uint8_t binary_pawn_black_png_end[] asm("_binary___assets_pawn_black_png_end");

		extern uint8_t binary_pawn_white_png[] asm("_binary___assets_pawn_white_png_start");
		extern uint8_t binary_pawn_white_png_end[] asm("_binary___assets_pawn_white_png_end");

		extern uint8_t binary_queen_black_png[] asm("_binary___assets_queen_black_png_start");
		extern uint8_t binary_queen_black_png_end[] asm("_binary___assets_queen_black_png_end");

		extern uint8_t binary_queen_white_png[] asm("_binary___assets_queen_white_png_start");
		extern uint8_t binary_queen_white_png_end[] asm("_binary___assets_queen_white_png_end");

		extern uint8_t binary_rook_black_png[] asm("_binary___assets_rook_black_png_start");
		extern uint8_t binary_rook_black_png_end[] asm("_binary___assets_rook_black_png_end");

		extern uint8_t binary_rook_white_png[] asm("_binary___assets_rook_white_png_start");
		extern uint8_t binary_rook_white_png_end[] asm("_binary___assets_rook_white_png_end");
	}

	/**
	 * Convert one of the staticly compiled PNG assets into an SDL
	 * texture.
	 *
	 * Quits the program on errors.
	 */
	SDL_Texture *load_texture(uint8_t *startptr, uint8_t *endptr);
}
