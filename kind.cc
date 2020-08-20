#include "chess.hh"

#include <stdexcept>

using namespace chess;

std::ostream &operator<<(std::ostream &os, const chess::Kind &kind)
{
	switch (kind) {
	case Kind::King:
		return os << "King";
	case Kind::Queen:
		return os << "Queen";
	case Kind::Rook:
		return os << "Rook";
	case Kind::Bishop:
		return os << "Bishop";
	case Kind::Knight:
		return os << "Knight";
	case Kind::Pawn:
		return os << "Pawn";
	default:
		throw std::invalid_argument("bad value for Kind enum");
	}
}
