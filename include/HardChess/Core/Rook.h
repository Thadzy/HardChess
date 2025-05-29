#ifndef HARDCHESS_CORE_ROOK_H
#define HARDCHESS_CORE_ROOK_H

#include "HardChess/Core/Piece.h"

namespace HardChess {

    class Rook : public Piece {
      public:
        Rook(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Rook"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_ROOK_H