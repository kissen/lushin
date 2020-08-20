#pragma once

#include <cstdint>
#include <SDL2/SDL.h>

namespace assets
{
	extern "C"
	{
		extern void *binary_bishop_black_png_size asm("_binary_bishop_black_png_size");
		extern void *binary_bishop_black_png asm("_binary_bishop_black_png_start");
		extern void *binary_bishop_black_png_end asm("_binary_bishop_black_png_end");

		extern void *binary_bishop_white_png_size asm("_binary_bishop_white_png_size");
		extern void *binary_bishop_white_png asm("_binary_bishop_white_png_start");

		extern void *binary_king_black_png_size asm("_binary_king_black_png_size");
		extern void *binary_king_black_png asm("_binary_king_black_png_start");

		extern void *binary_king_white_png_size asm("_binary_king_white_png_size");
		extern void *binary_king_white_png asm("_binary_king_white_png_start");

		extern void *binary_knight_black_png_size asm("_binary_knight_black_png_size");
		extern void *binary_knight_black_png asm("_binary_knight_black_png_start");

		extern void *binary_knight_white_png_size asm("_binary_knight_white_png_size");
		extern void *binary_knight_white_png asm("_binary_knight_white_png_start");

		extern void *binary_pawn_black_png_size asm("_binary_pawn_black_png_size");
		extern void *binary_pawn_black_png asm("_binary_pawn_black_png_start");

		extern void *binary_pawn_white_png_size asm("_binary_pawn_white_png_size");
		extern void *binary_pawn_white_png asm("_binary_pawn_white_png_start");

		extern void *binary_queen_black_png_size asm("_binary_queen_black_png_size");
		extern void *binary_queen_black_png asm("_binary_queen_black_png_start");

		extern void *binary_queen_white_png_size asm("_binary_queen_white_png_size");
		extern void *binary_queen_white_png asm("_binary_queen_white_png_start");

		extern void *binary_rook_black_png_size asm("_binary_rook_black_png_size");
		extern void *binary_rook_black_png asm("_binary_rook_black_png_start");

		extern void *binary_rook_white_png_size asm("_binary_rook_white_png_size");
		extern void *binary_rook_white_png asm("_binary_rook_white_png_start");

	}

	/**
	 * Convert one of the staticly compiled PNG assets into an SDL
	 * texture.
	 */
	SDL_Texture *load_texture(void *startptr, void *sizeptr);
}
