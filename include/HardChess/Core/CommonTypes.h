#ifndef HARDCHESS_CORE_COMMONTYPES_H
#define HARDCHESS_CORE_COMMONTYPES_H

#include <string>
#include <vector>
#include <memory> // For std::unique_ptr

namespace HardChess {

    enum class Color { NONE, WHITE, BLACK };
    enum class PieceType { NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

    struct Position {
        int row;
        int col;

        Position(int r = -1, int c = -1) : row(r), col(c) {}

        bool isValid() const {
            return row >= 0 && row < 8 && col >= 0 && col < 8;
        }

        bool operator==(const Position& other) const {
            return row == other.row && col == other.col;
        }
        bool operator!=(const Position& other) const {
            return !(*this == other);
        }
    };

    // Forward declaration
    class Piece;
    class Board; // Forward declare Board
    class ConsoleUI; // Forward declare ConsoleUI

} // namespace HardChess

#endif // HARDCHESS_CORE_COMMONTYPES_H