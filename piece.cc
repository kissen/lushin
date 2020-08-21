#include "chess.hh"

using namespace chess;

Piece::Piece(const Piece &other) : color(other.color), kind(other.kind), present(other.present)
{
}

Piece::Piece(Color color, Kind kind, bool present) : color(color), kind(kind), present(present)
{
}

Piece::~Piece()
{
}

Piece &Piece::operator=(const Piece &other)
{
	this->color = other.color;
	this->kind = other.kind;
	this->present = other.present;
	return *this;
}

Piece Piece::that_is_not_present()
{
	return Piece(Color::White, Kind::King, false);
}

std::ostream &operator<<(std::ostream &os, const chess::Piece &piece)
{
	if (piece.present) {
		return os << "(" << piece.color << ", " << piece.kind << ", present)";
	} else {
		return os << "(" << piece.color << ", " << piece.kind << ", not present)";
	}
}
