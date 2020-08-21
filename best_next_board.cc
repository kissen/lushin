#include <stdexcept>

#include "chess.hh"
#include "choice.hh"

using namespace chess;

Board chess::best_next_board(const Board &board, Color current_player)
{
	const auto choices = chess::valid_next_boards(board, current_player);
	const auto chosen = choice::make(choices);

	if (!chosen) {
		throw std::logic_error("no valid next states");
	}

	return *chosen;
}
