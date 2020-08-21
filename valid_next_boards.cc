#include "chess.hh"

using namespace chess;

std::vector<Board> chess::valid_next_boards(const Board &board, Color current_player)
{
	std::vector<Board> next_boards;

	for (uint8_t x = 0; x < 8; ++x) {
		for (uint8_t y = 0; y < 8; ++y) {
			const Pos pos = {x, y};
			const Piece &piece = board.at(pos);

			if (!piece.present) {
				continue;
			}

			if (piece.color != current_player) {
				continue;
			}

			const auto nexts = chess::valid_next_positions(board, pos);

			for (const Pos &to : nexts) {
				const Pos &from = pos;

				Board next_board = board;
				next_board.move(from, to);

				next_boards.push_back(next_board);
			}
		}
	}

	return next_boards;
}
