#include "HardChess/Core/Pawn.h"
#include "HardChess/Core/Board.h" // For board.getPiece()

namespace HardChess {

    Pawn::Pawn(Color c, Position pos) : Piece(c, PieceType::PAWN, pos) {}

    char Pawn::getSymbol() const {
        return (color == Color::WHITE) ? 'P' : 'p';
    }
    
    std::unique_ptr<Piece> Pawn::clone() const {
        auto newPiece = std::make_unique<Pawn>(this->color, this->position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Pawn::isValidMove(Position start, Position end, const Board& board) const {
        if (!end.isValid()) return false;
        if (start == end) return false;

        std::unique_ptr<Piece> targetPiece = board.getPiece(end);
        if (targetPiece && targetPiece->getColor() == this->color) {
            return false; // Cannot capture own piece
        }

        int forwardDirection = (this->color == Color::WHITE) ? -1 : 1; // White moves up, Black moves down

        // Standard one-step forward
        if (end.col == start.col && end.row == start.row + forwardDirection && !targetPiece) {
            return true;
        }

        // Initial two-step forward
        if (!this->hasMoved && end.col == start.col && end.row == start.row + 2 * forwardDirection && !targetPiece) {
            // Check if path is clear
            Position intermediate(start.row + forwardDirection, start.col);
            if (!board.getPiece(intermediate)) {
                return true;
            }
        }

        // Capture
        if (abs(end.col - start.col) == 1 && end.row == start.row + forwardDirection) {
            if (targetPiece && targetPiece->getColor() != this->color) {
                return true; // Diagonal capture of opponent's piece
            }
            // En Passant would be checked here if implemented
        }
        return false;
    }

} // namespace HardChess