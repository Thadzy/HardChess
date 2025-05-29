#include "HardChess/Core/King.h"
#include "HardChess/Core/Board.h"
#include <cmath> // For abs

namespace HardChess {

    King::King(Color c, Position pos) : Piece(c, PieceType::KING, pos) {}

    char King::getSymbol() const {
        return (color == Color::WHITE) ? 'K' : 'k';
    }

    std::unique_ptr<Piece> King::clone() const {
        auto newPiece = std::make_unique<King>(this->color, this->position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool King::isValidMove(Position start, Position end, const Board& board) const {
        if (!end.isValid()) return false;
        if (start == end) return false;

        std::unique_ptr<Piece> targetPiece = board.getPiece(end);
        if (targetPiece && targetPiece->getColor() == this->color) {
            return false;
        }

        int dr = std::abs(start.row - end.row);
        int dc = std::abs(start.col - end.col);

        if (dr <= 1 && dc <= 1) { // Moves one square in any direction
            // The check for whether the king moves into check is typically handled
            // by the Game logic calling Board::isSquareAttacked or similar.
            // For this piece-specific isValidMove, we assume the square isn't attacked by self-check rule.
            return true;
        }
        
        // Castling logic would be here if implemented
        // Example basic castling (needs Board methods like isSquareAttacked, hasRookMoved etc.):
        // if (!this->hasMoved && dr == 0 && std::abs(dc) == 2) {
        //     // Check path clear, not in/through/into check, rook hasn't moved
        // }

        return false;
    }

} // namespace HardChess