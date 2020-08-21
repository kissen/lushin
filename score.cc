#include <stdexcept>

#include "chess.hh"

using namespace chess;

static int absolute_score_piece(const Piece &piece)
{
	switch (piece.kind) {
	case Kind::King:
		return 18;
	case Kind::Queen:
		return 9;
	case Kind::Rook:
		return 5;
	case Kind::Bishop:
		return 3;
	case Kind::Knight:
		return 3;
	case Kind::Pawn:
		return 1;
	default:
		return 0;
	}
}

static int score_piece(const Piece &piece, Color current_player)
{
	const int absolute = absolute_score_piece(piece);

	if (piece.color == current_player) {
		return absolute;
	} else {
		return (-1) * absolute;
	}
}

int chess::score(const Board &board, Color current_player)
{
	int accumulated = 0;

	board.for_each([&] (const Pos &pos, const Piece &piece) {
		(void) pos;
		accumulated += score_piece(piece, current_player);
	});

	return accumulated;
}
