#include "chess.hh"

#include <stdexcept>

using namespace chess;

Color chess::swap_color(Color color)
{
	switch (color) {
	case Color::Black:
		return Color::White;
	case Color::White:
		return Color::Black;
	default:
		throw std::invalid_argument("bad value for Color enum");
	}
}

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
