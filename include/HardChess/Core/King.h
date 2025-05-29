#ifndef HARDCHESS_CORE_KING_H
#define HARDCHESS_CORE_KING_H

#include "HardChess/Core/Piece.h"

namespace HardChess {

    class King : public Piece {
      public:
        King(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "King"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
        // Note: King's isValidMove should also check if 'end' is attacked by opponent
        // This check is usually done at game level to prevent illegal moves.
        // For piece-level check, it ensures the move is structurally king-like.
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_KING_H