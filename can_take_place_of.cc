#include "chess.hh"

using namespace chess;

bool chess::can_take_place_of(const Piece &from, const Piece &to)
{
	if (!from.present) {
		return false;
	}

	if (!to.present) {
		return true;
	}

	return from.color != to.color;
}
