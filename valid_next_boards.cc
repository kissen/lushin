#include "chess.hh"

using namespace chess;

std::vector<Board> chess::valid_next_boards(const Board &board, Color current_player)
{
	std::vector<Board> next_boards;

	board.for_each([&] (const Pos &pos, const Piece &piece) {
		if (!piece.present) {
			return;
		}

		if (piece.color != current_player) {
			return;
		}

		const auto nexts = chess::valid_next_positions(board, pos);

		for (const Pos &to : nexts) {
			const Pos &from = pos;

			Board next_board = board;
			next_board.move(from, to);

			next_boards.push_back(next_board);
		}
	});

	return next_boards;
}
