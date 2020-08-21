#include "chess.hh"

using namespace chess;

static bool contains_king(const Board &board, Color king_color)
{
	bool contains = false;

	board.for_each([&] (const Pos &pos, const Piece &piece) {
		(void) pos;
		if (piece.color == king_color && piece.kind == Kind::King) {
			contains = true;
		}
	});

	return contains;
}

bool chess::is_checked(const Board &board, Color current_player)
{
	const Color opponent_player = chess::swap_color(current_player);
	const auto next_boards = chess::valid_next_boards(board, opponent_player);

	for (const Board &next_board : next_boards) {
		if (!contains_king(next_board, current_player)) {
			return true;
		}
	}

	return false;
}
