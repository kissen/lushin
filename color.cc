#include "chess.hh"

#include <stdexcept>

using namespace chess;

std::ostream &operator<<(std::ostream &os, const chess::Color &color)
{
	switch (color) {
	case Color::Black:
		return os << "Black";
	case Color::White:
		return os << "White";
	default:
		throw std::invalid_argument("bad value for Color enum");
	}
}
