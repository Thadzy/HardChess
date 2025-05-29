#include "HardChess/Core/Piece.h"
#include "HardChess/Core/Board.h" // Required for Board reference in getPossibleMoves

namespace HardChess {

    std::vector<Position> Piece::getPossibleMoves(Position start, const Board& board) const {
        std::vector<Position> moves;
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position end(r, c);
                if (isValidMove(start, end, board)) {
                    moves.push_back(end);
                }
            }
        }
        return moves;
    }

} // namespace HardChess