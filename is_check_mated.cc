#include "chess.hh"

using namespace chess;

bool chess::is_check_mated(const Board &board, Color current_player)
{
	const auto next_boards = chess::valid_next_boards(board, current_player);

	for (const Board &next_board : next_boards) {
		if (!chess::is_checked(next_board, current_player)) {
			return false;
		}
	}

	return false;
}
