#include "HardChess/Core/Knight.h"
#include "HardChess/Core/Board.h"
#include <cmath> // For abs

namespace HardChess {

    Knight::Knight(Color c, Position pos) : Piece(c, PieceType::KNIGHT, pos) {}

    char Knight::getSymbol() const {
        return (color == Color::WHITE) ? 'N' : 'n';
    }

    std::unique_ptr<Piece> Knight::clone() const {
        auto newPiece = std::make_unique<Knight>(this->color, this->position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Knight::isValidMove(Position start, Position end, const Board& board) const {
        if (!end.isValid()) return false;
        if (start == end) return false;

        std::unique_ptr<Piece> targetPiece = board.getPiece(end);
        if (targetPiece && targetPiece->getColor() == this->color) {
            return false;
        }

        int dr = std::abs(start.row - end.row);
        int dc = std::abs(start.col - end.col);

        // Standard knight move: 2 by 1 or 1 by 2
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }

} // namespace HardChess