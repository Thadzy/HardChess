#ifndef HARDCHESS_CORE_BOARD_H
#define HARDCHESS_CORE_BOARD_H

#include "HardChess/Core/CommonTypes.h"
#include "HardChess/Core/Piece.h" // For Piece, not just forward declaration
#include <vector>
#include <memory> // For std::unique_ptr

// Forward declare specific piece types to avoid full includes here if only creating them
namespace HardChess {
    class Pawn; class Rook; class Knight; class Bishop; class Queen; class King;

    class Board {
      private:
        std::vector<std::vector<std::unique_ptr<Piece>>> grid;
        Position whiteKingPos;
        Position blackKingPos;

        // For move validation state (temporary move for check detection)
        struct ValidationMoveState {
            Position start, end;
            std::unique_ptr<Piece> capturedPiece;
            bool pieceAtStartOriginalHasMoved;
            bool pieceAtEndOriginalHasMoved; // If a piece was captured, its hasMoved doesn't matter for undo
        };
        ValidationMoveState currentValidationState;

      public:
        Board();
        Board(const Board& other); // Deep copy constructor
        Board& operator=(const Board& other); // Deep copy assignment

        void initializeBoard();

        std::unique_ptr<Piece> getPiece(Position pos) const;
        Piece* getPiecePtr(Position pos) const;

        void setPiece(Position pos, std::unique_ptr<Piece> piece);
        std::unique_ptr<Piece> removePiece(Position pos);

        // Returns captured piece
        std::unique_ptr<Piece> movePiece(Position start, Position end); 
        
        bool isPathClear(Position start, Position end) const;
        Position findKing(Color kingColor) const;
        bool isSquareAttacked(Position square, Color attackerColor) const;
        bool isKingInCheck(Color kingColor) const;

        // Methods for move validation (simpler than full board copy)
        void recordPieceStatesForValidation(Position start, Position end);
        std::unique_ptr<Piece> tryMoveForValidation(Position start, Position end);
        void revertValidationMove(Position start, Position end, std::unique_ptr<Piece> originalPieceAtEnd);

        bool promotePawn(Position pawnPos, PieceType promotionType);
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_BOARD_H