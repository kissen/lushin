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
	enum class Color : uint8_t
	{
		Black = 0,
		White = 1
	};

	/**
	 * The kind of family of a piece, e.g. King or Rook.
	 */
	enum class Kind : uint8_t
	{
		King = 0,
		Queen = 1,
		Rook = 2,
		Bishop = 3,
		Knight = 4,
		Pawn = 5,
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
		 * Copy constructor.
		 */
		Piece(const Piece &other);

		/**
		 * Construct a new piece with given color, kind and
		 * present flag.
		 */
		Piece(Color color, Kind kind, bool present=true);

		/**
		 * Destructor for rule of three.
		 */
		~Piece();

		/**
		 * Assignment operator.
		 */
		Piece &operator=(const Piece &other);

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
		 * Copy constructor.
		 */
		Pos(const Pos &other);

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
		 * Destructore for rule of three.
		 */
		~Pos();

		/**
		 * Assignment.
		 */
		Pos &operator=(const Pos &other);

		/**
		 * Equality check.
		 */
		bool operator==(const Pos &other) const;

		/**
		 * Unequality check.
		 */
		bool operator!=(const Pos &other) const;

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

	/**
	 * Return whether from can take the place of to.
	 */
	bool can_take_place_of(const Piece &from, const Piece &to);

	/**
	 * Return all positions the piece on position from can reach
	 * within one turn. If from is not present, this method
	 * returns an empty vector.
	 */
	std::vector<Pos> valid_next_positions(const Board &board, const Pos &from);

	/**
	 * Return wheter board is in a state of "checked" when it is
	 * current_players turn.
	 *
	 * If this function returns true, then current_player has to
	 * make a move to protect their king.
	 */
	bool is_checked(const Board &board, Color current_player);

	/**
	 * Return wheter board is in a state of "checkmate" when it is
	 * current_players turn.
	 *
	 * If this function returns true, then current_player has lost
	 * the game.
	 */
	bool is_check_mated(const Board &board, Color current_player);
};

//
// define << operators for easier printing
//

std::ostream &operator<<(std::ostream &os, const chess::Color &color);
std::ostream &operator<<(std::ostream &os, const chess::Kind &kind);
std::ostream &operator<<(std::ostream &os, const chess::Piece &piece);
std::ostream &operator<<(std::ostream &os, const chess::Pos &pos);
std::ostream &operator<<(std::ostream &os, const chess::Board &board);
