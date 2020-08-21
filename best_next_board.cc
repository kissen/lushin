#include <algorithm>
#include <cassert>
#include <climits>
#include <stdexcept>

#include "chess.hh"
#include "choice.hh"

using namespace chess;

static std::vector<Board> boards_with_best_scores(const std::vector<Board> &boards, Color current_player)
{
	assert(!boards.empty());

	std::vector<Board> best_boards;
	int best_score = INT_MIN;

	for (const Board &board : boards) {
		const int board_score = chess::score(board, current_player);

		if (board_score > best_score) {
			best_score = board_score;
			best_boards.clear();
			best_boards.push_back(board);
		} else if (board_score == best_score) {
			best_boards.push_back(board);
		}
	}

	return best_boards;
}

Board chess::best_next_board(const Board &board, Color current_player)
{
	const auto choices = chess::valid_next_boards(board, current_player);
	assert(!choices.empty());

	const auto besties = boards_with_best_scores(choices, current_player);
	assert(!choices.empty());

	const auto chosen = choice::make(besties);
	assert(chosen);

	return *chosen;
}
