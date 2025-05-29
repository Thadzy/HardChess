#include "HardChess/Core/Bishop.h"
#include "HardChess/Core/Board.h"
#include <cmath>

namespace HardChess {

Bishop::Bishop(Color c, Position pos)
    : Piece(c, PieceType::BISHOP, pos) {}

char Bishop::getSymbol() const {
    return (color == Color::WHITE) ? 'B' : 'b';
}

std::unique_ptr<Piece> Bishop::clone() const {
    auto newPiece = std::make_unique<Bishop>(this->color, this->position);
    newPiece->setHasMoved(this->hasMoved);
    return newPiece;
}

bool Bishop::isValidMove(Position start, Position end, const Board& board) const {
    if (!end.isValid()) return false;
    if (start == end) return false;

    int dr = std::abs(start.row - end.row);
    int dc = std::abs(start.col - end.col);

    if (dr != dc) return false; // Must move diagonally

    // Check path clear
    if (!board.isPathClear(start, end)) return false;

    std::unique_ptr<Piece> targetPiece = board.getPiece(end);
    if (targetPiece && targetPiece->getColor() == this->color) {
        return false; // Can't capture own piece
    }

    return true;
}

} // namespace HardChess