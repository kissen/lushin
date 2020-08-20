#include "chess.hh"

using namespace chess;

Pos::Pos() : x(0), y(0)
{
}

Pos::Pos(int8_t x, int8_t y) : x(x), y(y)
{
}

Pos::Pos(int x, int y) : x(static_cast<int8_t>(x)), y(static_cast<int8_t>(y))
{
}

Pos Pos::operator+(const Pos &other) const
{
	const int8_t x = this->x + other.x;
	const int8_t y = this->y + other.y;
	return Pos(x, y);
}

Pos &Pos::operator+=(const Pos &other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Pos Pos::operator*(int8_t k) const
{
	const int8_t x = k * this->x;
	const int8_t y = k * this->y;
	return Pos(x, y);
}

static bool in_range(int8_t x)
{
	return x >= 0 && x <= 7;
}

bool Pos::on_board() const
{
	return in_range(this->x) && in_range(this->y);
}

std::ostream &operator<<(std::ostream &os, const chess::Pos &pos)
{
	const int x = static_cast<int>(pos.x);
	const int y = static_cast<int>(pos.y);

	return os << "(" << x << ", " << y << ")";
}
