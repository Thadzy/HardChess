#ifndef HARDCHESS_CORE_KNIGHT_H
#define HARDCHESS_CORE_KNIGHT_H

#include "HardChess/Core/Piece.h"

namespace HardChess {

    class Knight : public Piece {
      public:
        Knight(Color c, Position pos);
        char getSymbol() const override;
        std::string getName() const override { return "Knight"; }
        std::unique_ptr<Piece> clone() const override;
        bool isValidMove(Position start, Position end, const Board& board) const override;
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_KNIGHT_H