#include "chess.hh"

using namespace chess;

Piece::Piece(Color color, Kind kind, bool present) : color(color), kind(kind), present(present)
{
}

Piece Piece::that_is_not_present()
{
	return Piece(Color::White, Kind::King, false);
}

std::ostream &operator<<(std::ostream &os, const chess::Piece &piece)
{
	return os << "(" << piece.color << ", " << piece.kind << ")";
}
