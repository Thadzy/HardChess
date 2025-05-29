#ifndef HARDCHESS_CORE_PAWN_H
#define HARDCHESS_CORE_PAWN_H

#include "HardChess/Core/Piece.h"

namespace HardChess {

    class Pawn : public Piece {
      public:
        Pawn(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Pawn"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_PAWN_H