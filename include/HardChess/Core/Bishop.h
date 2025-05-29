#ifndef HARDCHESS_CORE_BISHOP_H
#define HARDCHESS_CORE_BISHOP_H

#include "HardChess/Core/Piece.h"

namespace HardChess {

class Bishop : public Piece {
public:
    Bishop(Color c, Position pos);
    char getSymbol() const override;
    std::string getName() const override { return "Bishop"; }
    std::unique_ptr<Piece> clone() const override;
    bool isValidMove(Position start, Position end, const Board& board) const override;
};

} // namespace HardChess

#endif // HARDCHESS_CORE_BISHOP_H