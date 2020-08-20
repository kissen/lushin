#pragma once

#include <cstdint>
#include <optional>
#include <ostream>
#include <vector>

namespace chess
{
	/**
	 * The color or team association of a piece, that is
	 * either black or white.
	 */
	enum class Color
	{
		Black,
		White
	};

	/**
	 * The kind of family of a piece, e.g. King or Rook.
	 */
	enum class Kind
	{
		King,
		Queen,
		Rook,
		Bishop,
		Knight,
		Pawn,
	};

	/**
	 * A piece is the combination of a color and a kind of piece.
	 * As a special case, the Piece can not be present. A piece
	 * that is not present in fact does not exist on the board.
	 */
	struct Piece
	{
		Color color;
		Kind kind;
		bool present;

		/**
		 * Construct a new piece with given color, kind and
		 * present flag.
		 */
		Piece(Color color, Kind kind, bool present=true);

		/**
		 * Return a piece that is not present.
		 */
		static Piece that_is_not_present();
	};

	/**
	 * Represents a position on the board in programming
	 * coordinates.
	 */
	class Pos
	{
	public:
		int8_t x;
		int8_t y;

		/**
		 * Create a new Pos pointing at (0, 0).
		 */
		Pos();

		/**
		 * Create a new Pos. It is your responsiblity that
		 * x and y are in [0, 7].
		 */
		Pos(int8_t x, int8_t y);

		/**
		 * Create a new Pos. It is your responsiblity that
		 * x and y are in [0, 7].
		 */
		Pos(int x, int y);

		/**
		 * Pairwise addition of two Pos.
		 */
		Pos operator+(const Pos &other) const;

		/**
		 * Pairwise addition to this Pos.
		 */
		Pos &operator+=(const Pos &other);

		/**
		 * Multiplication of scalar k.
		 */
		Pos operator*(int8_t k) const;

		/**
		 * Return whether this Pos is within the bounds of
		 * the chess board, that is whether both x and y
		 * are in [0, 7].
		 */
		bool on_board() const;
	};

	/**
	 * Represents a chess board.
	 */
	class Board
	{
	public:
		/**
		 * Construct a new empty board.
		 */
		Board();

		/**
		 * Return a pointer to the piece at pos.
		 */
		Piece &at(const Pos &pos);

		/**
		 * Return a reference to the piece at pos.
		 */
		const Piece &at(const Pos &pos) const;

		/**
		 * Move piece from -> to. If a piece was captured, return
		 * that piece.
		 */
		std::optional<Piece> move(const Pos &from, const Pos &to);

		/**
		 * Create a new board with the inital game set. Black is
		 * on top, White on the bottom.
		 */
		static Board initial();

	private:
		// 8 x 8 board
		std::vector<Piece> mboard;
	};
};

//
// define << operators for easier printing
//

std::ostream &operator<<(std::ostream &os, const chess::Color &color);
std::ostream &operator<<(std::ostream &os, const chess::Kind &kind);
std::ostream &operator<<(std::ostream &os, const chess::Piece &piece);
std::ostream &operator<<(std::ostream &os, const chess::Pos &pos);
std::ostream &operator<<(std::ostream &os, const chess::Board &board);
