#include <algorithm>
#include <iostream>
#include <cassert>

#include "chess.hh"

static constexpr uint8_t ROWS = 8;
static constexpr uint8_t COLUMNS = 8;

using namespace chess;

Board::Board()
{
	const size_t num_cells = ROWS * COLUMNS;

	for (size_t i = 0; i < num_cells; ++i) {
		Piece not_present = Piece::that_is_not_present();
		this->mboard.push_back(not_present);
	}
}

static size_t get_idx_for(uint8_t x, uint8_t y)
{
	assert(x < 8 && y < 8);

	const size_t idx = x + y * 8;
	return idx;
}

Piece &Board::at(const Pos &pos)
{
	const size_t idx = get_idx_for(pos.x, pos.y);
	return this->mboard.at(idx);
}

const Piece &Board::at(const Pos &pos) const
{
	const size_t idx = get_idx_for(pos.x, pos.y);
	return this->mboard.at(idx);
}

std::optional<Piece> Board::move(const Pos &from, const Pos &to)
{
	const Piece capturer = this->at(from);
	assert(capturer.present);

	const Piece captured = this->at(to);
	assert(!captured.present || can_take_place_of(capturer, captured));

	this->at(from) = Piece::that_is_not_present();
	this->at(to) = capturer;

	if (captured.present) {
		return std::make_optional(captured);
	} else {
		return std::nullopt;
	}
}

Board Board::initial()
{
	Board b;

	b.at({0, 0}) = Piece{Color::Black, Kind::Rook};
	b.at({1, 0}) = Piece{Color::Black, Kind::Knight};
	b.at({2, 0}) = Piece{Color::Black, Kind::Bishop};
	b.at({3, 0}) = Piece{Color::Black, Kind::Queen};
	b.at({4, 0}) = Piece{Color::Black, Kind::King};
	b.at({5, 0}) = Piece{Color::Black, Kind::Bishop};
	b.at({6, 0}) = Piece{Color::Black, Kind::Knight};
	b.at({7, 0}) = Piece{Color::Black, Kind::Rook};

	b.at({0, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({1, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({2, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({3, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({4, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({5, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({6, 1}) = Piece{Color::Black, Kind::Pawn};
	b.at({7, 1}) = Piece{Color::Black, Kind::Pawn};

	b.at({0, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({1, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({2, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({3, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({4, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({5, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({6, 6}) = Piece{Color::White, Kind::Pawn};
	b.at({7, 6}) = Piece{Color::White, Kind::Pawn};

	b.at({0, 7}) = Piece{Color::White, Kind::Rook};
	b.at({1, 7}) = Piece{Color::White, Kind::Knight};
	b.at({2, 7}) = Piece{Color::White, Kind::Bishop};
	b.at({3, 7}) = Piece{Color::White, Kind::Queen};
	b.at({4, 7}) = Piece{Color::White, Kind::King};
	b.at({5, 7}) = Piece{Color::White, Kind::Bishop};
	b.at({6, 7}) = Piece{Color::White, Kind::Knight};
	b.at({7, 7}) = Piece{Color::White, Kind::Rook};

	return b;
}


std::ostream &operator<<(std::ostream &os, const chess::Board &board)
{
	for (uint8_t y = 0; y < COLUMNS; ++y) {
		for (uint8_t x = 0; x < ROWS; ++x) {
			const Piece &piece = board.at({x, y});

			os << piece;

			if (x != ROWS - 1) {
				os << ' ';
			}
		}

		if (y != COLUMNS - 1) {
			os << '\n';
		}
	}

	return os;
}
