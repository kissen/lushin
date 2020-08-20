#include "chess.hh"

#include <stdexcept>

using namespace chess;

static std::vector<Pos> moves_from_diff(const Board &board, const Piece &piece, const Pos &at, const std::vector<Pos> &diffs)
{
	std::vector<Pos> output;

	for (const Pos &diff : diffs) {
		const Pos actual = at + diff;

		if (!actual.on_board()) {
			continue;
		}

		const Piece &target = board.at(actual);

		if (can_take_place_of(piece, target)) {
			output.push_back(actual);
		}
	}

	return output;
}

static std::vector<Pos> valid_next_king_positions(const Board &board, const Piece &piece, const Pos &from)
{
	static const std::vector<Pos> diff = {
		{1, 0}, {0, 1}, {-1, 0}, {0, -1},
		{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
	};

	return moves_from_diff(board, piece, from, diff);
}

static std::vector<Pos> reachable_by_travel(const Board &board, const Piece &piece, const Pos &start, const Pos& direction)
{
	std::vector<Pos> output;

	Pos current = start + direction;

	while (current.on_board()) {
		const Piece &piece_on_current = board.at(current);

		if (piece_on_current.present) {
			if (can_take_place_of(piece, piece_on_current)) {
				output.push_back(current);
			}

			break;
		}

		output.push_back(current);
		current += direction;
	}

	return output;
}

static std::vector<Pos> valid_next_rook_positions(const Board &board, const Piece &piece, const Pos &from)
{
	static const std::vector<Pos> directions = {
		{1, 0}, {0, 1}, {-1, 0}, {0, -1}
	};

	std::vector<Pos> output;

	for (const Pos &direction : directions) {
		const std::vector<Pos> reachable = reachable_by_travel(board, piece, from, direction);
		output.insert(end(output), begin(reachable), end(reachable));
	}

	return output;
}

static std::vector<Pos> valid_next_bishop_positions(const Board &board, const Piece &piece, const Pos &from)
{
	static const std::vector<Pos> directions = {
		{1, 1}, {1, -1}, {-1, -1}, {-1, 1}
	};

	std::vector<Pos> output;

	for (const Pos &direction : directions) {
		const std::vector<Pos> reachable = reachable_by_travel(board, piece, from, direction);
		output.insert(end(output), begin(reachable), end(reachable));
	}

	return output;
}

static std::vector<Pos> valid_next_queen_positions(const Board &board, const Piece &piece, const Pos &from)
{
	std::vector<Pos> output;

	const std::vector<Pos> rook_moves = valid_next_rook_positions(board, piece, from);
	output.insert(end(output), begin(rook_moves), end(rook_moves));

	const std::vector<Pos> bishop_moves = valid_next_bishop_positions(board, piece, from);
	output.insert(end(output), begin(bishop_moves), end(bishop_moves));

	return output;
}

static std::vector<Pos> valid_next_knight_positions(const Board &board, const Piece &piece, const Pos &from)
{
	static const std::vector<Pos> diff = {
		{1, 2}, {2, 1}, {2, -1}, {1, -2},
		{-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
	};

	return moves_from_diff(board, piece, from, diff);
}

static bool is_initial_pawn(const Piece &piece, const Pos &at)
{
	if (piece.kind != Kind::Pawn) {
		return false;
	}

	const Color &color = piece.color;

	if (color == Color::White && at.y == 6) {
		return true;
	}

	if (color == Color::Black && at.y == 1) {
		return true;
	}

	return false;
}

static bool is_empty(const Board &b, const Pos &at)
{
	const Piece &piece = b.at(at);
	return !piece.present;
}

static std::vector<Pos> valid_next_pawn_positions(const Board &board, const Piece &piece, const Pos &from)
{
	std::vector<Pos> pawn_moves;

	int sign = piece.color == Color::White ? -1 : 1;

	const Pos regular_move = Pos(0, sign) + from;
	const Pos double_move  = Pos(0, 2 * sign) + from;
	const Pos capture_left  = Pos(-1, sign) + from;
	const Pos capture_right = Pos(1, sign) + from;

	if (regular_move.on_board() && is_empty(board, regular_move)) {
		pawn_moves.push_back(regular_move);
	}

	if (double_move.on_board() && is_initial_pawn(piece, from) && is_empty(board, double_move)) {
		pawn_moves.push_back(double_move);
	}

	if (capture_left.on_board() && !is_empty(board, capture_left) && can_take_place_of(piece, board.at(capture_left))) {
		pawn_moves.push_back(capture_left);
	}

	if (capture_right.on_board() && !is_empty(board, capture_right) && can_take_place_of(piece, board.at(capture_right))) {
		pawn_moves.push_back(capture_right);
	}

	return pawn_moves;
}

std::vector<Pos> chess::valid_next_positions(const Board &board, const Pos &from)
{
	const Piece &piece = board.at(from);

	if (!piece.present) {
		return {};
	}

	switch (piece.kind) {
	case Kind::King:
		return valid_next_king_positions(board, piece, from);
	case Kind::Queen:
		return valid_next_queen_positions(board, piece, from);
	case Kind::Rook:
		return valid_next_rook_positions(board, piece, from);
	case Kind::Bishop:
		return valid_next_bishop_positions(board, piece, from);
	case Kind::Knight:
		return valid_next_knight_positions(board, piece, from);
	case Kind::Pawn:
		return valid_next_pawn_positions(board, piece, from);
	default:
		throw std::invalid_argument("bad value for Kind enum");
	}
}
