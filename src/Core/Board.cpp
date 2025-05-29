#include "HardChess/Core/Board.h"
#include "HardChess/UI/ConsoleUI.h"
#include "HardChess/Core/Pawn.h"
#include "HardChess/Core/Rook.h"
#include "HardChess/Core/Knight.h"
#include "HardChess/Core/Bishop.h"
#include "HardChess/Core/Queen.h"
#include "HardChess/Core/King.h"
#include <iostream>
#include <algorithm>

namespace HardChess
{

    Board::Board() : grid(8)
    {
        for (int i = 0; i < 8; ++i)
        {
            grid.clear();
            grid.resize(8);
            for (auto &row : grid)
                row.resize(8);
        }
        initializeBoard();
    }

    // Deep copy constructor
    Board::Board(const Board &other) : grid(8)
    {
        for (int i = 0; i < 8; ++i)
        {
            grid.clear();
            grid.resize(8);
            for (auto &row : grid)
                row.resize(8);
            for (int j = 0; j < 8; ++j)
            {
                if (other.grid[i][j])
                {
                    grid[i][j] = other.grid[i][j]->clone();
                }
                else
                {
                    grid[i][j] = nullptr;
                }
            }
        }
        whiteKingPos = other.whiteKingPos;
        blackKingPos = other.blackKingPos;
    }

    // Deep copy assignment
    Board &Board::operator=(const Board &other)
    {
        if (this == &other)
        {
            return *this;
        }
        grid.clear();
        grid.resize(8);
        for (auto &row : grid)
            row.resize(8);
        for (int i = 0; i < 8; ++i)
        {
            grid.clear();
            grid.resize(8);
            for (auto &row : grid)
                row.resize(8);
            for (int j = 0; j < 8; ++j)
            {
                if (other.grid[i][j])
                {
                    grid[i][j] = other.grid[i][j]->clone();
                }
                else
                {
                    grid[i][j] = nullptr;
                }
            }
        }
        whiteKingPos = other.whiteKingPos;
        blackKingPos = other.blackKingPos;
        return *this;
    }

    void Board::initializeBoard()
    {
        // Clear board first (important if re-initializing)
        for (auto &row : grid)
        {
            for (auto &cell : row)
            {
                cell.reset();
            }
        }

        // Place Pawns
        for (int c = 0; c < 8; ++c)
        {
            grid[1][c] = std::make_unique<Pawn>(Color::BLACK, Position(1, c));
            grid[6][c] = std::make_unique<Pawn>(Color::WHITE, Position(6, c));
        }

        // Place Rooks
        grid[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
        grid[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));
        grid[7][0] = std::make_unique<Rook>(Color::WHITE, Position(7, 0));
        grid[7][7] = std::make_unique<Rook>(Color::WHITE, Position(7, 7));

        // Place Knights
        grid[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
        grid[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
        grid[7][1] = std::make_unique<Knight>(Color::WHITE, Position(7, 1));
        grid[7][6] = std::make_unique<Knight>(Color::WHITE, Position(7, 6));

        // Place Bishops
        grid[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
        grid[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
        grid[7][2] = std::make_unique<Bishop>(Color::WHITE, Position(7, 2));
        grid[7][5] = std::make_unique<Bishop>(Color::WHITE, Position(7, 5));

        // Place Queens
        grid[0][3] = std::make_unique<Queen>(Color::BLACK, Position(0, 3));
        grid[7][3] = std::make_unique<Queen>(Color::WHITE, Position(7, 3));

        // Place Kings
        grid[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
        blackKingPos = Position(0, 4);
        grid[7][4] = std::make_unique<King>(Color::WHITE, Position(7, 4));
        whiteKingPos = Position(7, 4);
    }

    Piece *Board::getPiecePtr(Position pos) const
    {
        if (!pos.isValid() || !grid[pos.row][pos.col])
        {
            return nullptr;
        }
        return grid[pos.row][pos.col].get();
    }

    std::unique_ptr<Piece> Board::getPiece(Position pos) const
    {
        if (!pos.isValid() || !grid[pos.row][pos.col])
        {
            return nullptr;
        }
        return grid[pos.row][pos.col]->clone();
    }

    void Board::setPiece(Position pos, std::unique_ptr<Piece> piece)
    {
        if (!pos.isValid())
            return;
        grid[pos.row][pos.col] = std::move(piece);
        if (grid[pos.row][pos.col])
        {
            grid[pos.row][pos.col]->setPosition(pos);
            if (grid[pos.row][pos.col]->getType() == PieceType::KING)
            {
                if (grid[pos.row][pos.col]->getColor() == Color::WHITE)
                    whiteKingPos = pos;
                else
                    blackKingPos = pos;
            }
        }
    }

    std::unique_ptr<Piece> Board::removePiece(Position pos)
    {
        if (!pos.isValid() || !grid[pos.row][pos.col])
        {
            return nullptr;
        }
        return std::move(grid[pos.row][pos.col]);
    }

    std::unique_ptr<Piece> Board::movePiece(Position start, Position end)
    {
        if (!start.isValid() || !end.isValid() || !grid[start.row][start.col])
        {
            return nullptr;
        }

        std::unique_ptr<Piece> capturedPiece = std::move(grid[end.row][end.col]);
        grid[end.row][end.col] = std::move(grid[start.row][start.col]);
        // grid[start.row][start.col] is now nullptr implicitly by move

        if (grid[end.row][end.col])
        {
            grid[end.row][end.col]->setPosition(end);
            grid[end.row][end.col]->setHasMoved(true);

            if (grid[end.row][end.col]->getType() == PieceType::KING)
            {
                if (grid[end.row][end.col]->getColor() == Color::WHITE)
                {
                    whiteKingPos = end;
                }
                else
                {
                    blackKingPos = end;
                }
            }
        }
        return capturedPiece;
    }

    bool Board::isPathClear(Position start, Position end) const
    {
        int dr = (end.row > start.row) ? 1 : ((end.row < start.row) ? -1 : 0);
        int dc = (end.col > start.col) ? 1 : ((end.col < start.col) ? -1 : 0);

        int currentRow = start.row + dr;
        int currentCol = start.col + dc;

        while (currentRow != end.row || currentCol != end.col)
        {
            if (!Position(currentRow, currentCol).isValid())
                return false;
            if (grid[currentRow][currentCol])
            {
                return false;
            }
            currentRow += dr;
            currentCol += dc;
        }
        return true;
    }

    Position Board::findKing(Color kingColor) const
    {
        return (kingColor == Color::WHITE) ? whiteKingPos : blackKingPos;
    }

    bool Board::isSquareAttacked(Position square, Color attackerColor) const
    {
        if (!square.isValid())
            return false;
        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                Piece *p = getPiecePtr(Position(r, c));
                if (p && p->getColor() == attackerColor)
                {
                    if (p->isValidMove(Position(r, c), square, *this))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool Board::isKingInCheck(Color kingColor) const
    {
        Position kingPos = findKing(kingColor);
        if (!kingPos.isValid())
            return true;
        Color attackerColor = (kingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
        return isSquareAttacked(kingPos, attackerColor);
    }

    void Board::recordPieceStatesForValidation(Position start, Position end)
    {
        currentValidationState.start = start;
        currentValidationState.end = end;
        currentValidationState.capturedPiece = nullptr;

        Piece *pieceAtStart = getPiecePtr(start);
        currentValidationState.pieceAtStartOriginalHasMoved = pieceAtStart ? pieceAtStart->getHasMoved() : false;
    }

    std::unique_ptr<Piece> Board::tryMoveForValidation(Position start, Position end)
    {
        recordPieceStatesForValidation(start, end);

        currentValidationState.capturedPiece = grid[end.row][end.col] ? grid[end.row][end.col]->clone() : nullptr;

        std::unique_ptr<Piece> pieceToMove = std::move(grid[start.row][start.col]);
        std::unique_ptr<Piece> overwrittenPieceAtEnd = std::move(grid[end.row][end.col]);
        grid[end.row][end.col] = std::move(pieceToMove);

        if (grid[end.row][end.col])
        {
            grid[end.row][end.col]->setPosition(end);
            grid[end.row][end.col]->setHasMoved(true);
            if (grid[end.row][end.col]->getType() == PieceType::KING)
            {
                if (grid[end.row][end.col]->getColor() == Color::WHITE)
                    whiteKingPos = end;
                else
                    blackKingPos = end;
            }
        }
        return overwrittenPieceAtEnd;
    }

    void Board::revertValidationMove(Position start, Position end, std::unique_ptr<Piece> originalPieceAtEndFromTryMove)
    {
        std::unique_ptr<Piece> movedPiece = std::move(grid[end.row][end.col]);
        grid[start.row][start.col] = std::move(movedPiece);
        grid[end.row][end.col] = std::move(originalPieceAtEndFromTryMove);

        if (grid[start.row][start.col])
        {
            grid[start.row][start.col]->setPosition(start);
            grid[start.row][start.col]->setHasMoved(currentValidationState.pieceAtStartOriginalHasMoved);
            if (grid[start.row][start.col]->getType() == PieceType::KING)
            {
                if (grid[start.row][start.col]->getColor() == Color::WHITE)
                    whiteKingPos = start;
                else
                    blackKingPos = start;
            }
        }
        if (grid[end.row][end.col] && grid[end.row][end.col]->getType() == PieceType::KING)
        {
            if (grid[end.row][end.col]->getColor() == Color::WHITE)
                whiteKingPos = end;
            else
                blackKingPos = end;
        }
    }

    bool Board::promotePawn(Position pawnPos, PieceType promotionType)
    {
        if (!pawnPos.isValid() || !grid[pawnPos.row][pawnPos.col] || grid[pawnPos.row][pawnPos.col]->getType() != PieceType::PAWN)
        {
            return false;
        }
        Piece *pawn = grid[pawnPos.row][pawnPos.col].get();
        Color color = pawn->getColor();

        if (!((color == Color::WHITE && pawnPos.row == 0) || (color == Color::BLACK && pawnPos.row == 7)))
        {
            return false;
        }

        std::unique_ptr<Piece> newPiece;
        switch (promotionType)
        {
        case PieceType::QUEEN:
            newPiece = std::make_unique<Queen>(color, pawnPos);
            break;
        case PieceType::ROOK:
            newPiece = std::make_unique<Rook>(color, pawnPos);
            break;
        case PieceType::BISHOP:
            newPiece = std::make_unique<Bishop>(color, pawnPos);
            break;
        case PieceType::KNIGHT:
            newPiece = std::make_unique<Knight>(color, pawnPos);
            break;
        default:
            return false;
        }
        newPiece->setHasMoved(true);
        setPiece(pawnPos, std::move(newPiece));
        return true;
    }

} // namespace HardChess