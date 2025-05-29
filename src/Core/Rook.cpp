#include "HardChess/Core/Rook.h"
#include "HardChess/Core/Board.h"

namespace HardChess {

    Rook::Rook(Color c, Position pos) : Piece(c, PieceType::ROOK, pos) {}

    char Rook::getSymbol() const {
        return (color == Color::WHITE) ? 'R' : 'r';
    }

    std::unique_ptr<Piece> Rook::clone() const {
        auto newPiece = std::make_unique<Rook>(this->color, this->position);
        newPiece->setHasMoved(this->hasMoved);
        return newPiece;
    }

    bool Rook::isValidMove(Position start, Position end, const Board& board) const {
        if (!end.isValid()) return false;
        if (start == end) return false;

        std::unique_ptr<Piece> targetPiece = board.getPiece(end);
        if (targetPiece && targetPiece->getColor() == this->color) {
            return false;
        }

        if (start.row == end.row || start.col == end.col) { // Horizontal or vertical move
            return board.isPathClear(start, end);
        }
        return false;
    }

} // namespace HardChess