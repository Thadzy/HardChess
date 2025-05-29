#include "HardChess/Core/Queen.h"
#include "HardChess/Core/Board.h"
#include <cmath> // For abs

namespace HardChess {

    Queen::Queen(Color c, Position pos) : Piece(c, PieceType::QUEEN, pos) {}

    char Queen::getSymbol() const {
        return (color == Color::WHITE) ? 'Q' : 'q';
    }
    
    std::unique_ptr<Piece> Queen::clone() const {
        auto newPiece = std::make_unique<Queen>(this->color, this->position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Queen::isValidMove(Position start, Position end, const Board& board) const {
        if (!end.isValid()) return false;
        if (start == end) return false;

        std::unique_ptr<Piece> targetPiece = board.getPiece(end);
        if (targetPiece && targetPiece->getColor() == this->color) {
            return false;
        }

        // Check if it's a rook-like move OR a bishop-like move
        bool isRookMove = (start.row == end.row || start.col == end.col);
        bool isBishopMove = (std::abs(start.row - end.row) == std::abs(start.col - end.col));

        if (isRookMove || isBishopMove) {
            return board.isPathClear(start, end);
        }
        return false;
    }

} // namespace HardChess