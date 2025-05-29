#include "HardChess/Core/Game.h"
#include "HardChess/Core/Knight.h"
#include <iostream>
#include <algorithm>

namespace HardChess {

    Game::Game(Player* p1, Player* p2, ConsoleUI& consoleUi)
        : player1(p1), player2(p2), currentPlayer(nullptr), ui(consoleUi), roundState(RoundState::ONGOING) {
        // Board is default constructed and initializes itself
    }

    void Game::startRound() {
        board.initializeBoard(); // Reset board to starting positions
        currentPlayer = player1; // White always starts
        roundState = RoundState::ONGOING;
        ui.displayMessage("Round started. " + currentPlayer->getName() + " (" + currentPlayer->getColorString() + ") to move.");
        ui.displayBoard(board, false); // Always show White's perspective at start
    }

    Position Game::parsePosition(const std::string& s) const {
        if (s.length() < 2) return Position(-1, -1);
        char file = s[0];
        char rank = s[1];

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
            return Position(-1, -1);
        }
        // Board: (0,0) is a8, (7,7) is h1
        // Input: a1 ... h8
        // 'a' maps to col 0, 'h' to col 7
        // '1' maps to row 7, '8' to row 0
        return Position(7 - (rank - '1'), file - 'a');
    }
    
    bool Game::makeMove(Position start, Position end, PieceType promotionType) {
        if (!start.isValid() || !end.isValid()) {
            ui.displayMessage("Invalid position format or out of bounds.");
            return false;
        }

        Piece* pieceToMove = board.getPiecePtr(start);
        if (!pieceToMove) {
            ui.displayMessage("No piece at starting position " + ui.formatPosition(start) + ".");
            return false;
        }

        if (pieceToMove->getColor() != currentPlayer->getColor()) {
            ui.displayMessage("Cannot move opponent's piece.");
            return false;
        }

        // Prevent capturing the opponent's king
        Piece* targetPiece = board.getPiecePtr(end);
        if (targetPiece && targetPiece->getType() == PieceType::KING) {
            ui.displayMessage("You cannot capture the king! The game should end before this is possible.");
            return false;
        }
        
        // Check if the piece-specific move is valid (ignoring self-check for now)
        if (!pieceToMove->isValidMove(start, end, board)) {
            ui.displayMessage("Piece at " + ui.formatPosition(start) + " cannot move to " + ui.formatPosition(end) + " according to its rules.");
            return false;
        }

        // Simulate move to check for self-check
        Board tempBoard = board; // Use copy constructor
        std::unique_ptr<Piece> tempCaptured = tempBoard.movePiece(start, end); // Simulate on temp board
        
        if (tempBoard.isKingInCheck(currentPlayer->getColor())) {
            ui.displayMessage("Invalid move: Your King would be in check.");
            return false; // Move is illegal as it leaves King in check
        }

        // If all checks pass, make the move on the actual board
        std::unique_ptr<Piece> capturedPiece = board.movePiece(start, end);
        if (capturedPiece) {
            ui.displayMessage(capturedPiece->getColorString() + " " + capturedPiece->getName() + " captured at " + ui.formatPosition(end));
        }

        // Handle Pawn Promotion
        if (pieceToMove->getType() == PieceType::PAWN) {
            Color color = pieceToMove->getColor();
            if ((color == Color::WHITE && end.row == 0) || (color == Color::BLACK && end.row == 7)) {
                if (promotionType != PieceType::NONE) {
                    board.promotePawn(end, promotionType);
                    ui.displayMessage("Pawn promoted at " + ui.formatPosition(end) + "!");
                } else {
                    ui.displayMessage("Error: Pawn reached promotion rank but no promotion type specified.");
                }
            }
        }
        return true;
    }

    void Game::playTurn() {
        if (isRoundOver()) return;

        ui.displayPlayerTurn(currentPlayer);
        
        bool moveMade = false;
        while(!moveMade) {
            std::string moveStr = ui.getPlayerMove(*currentPlayer);

            // Prevent out_of_range on short input
            if (moveStr.length() < 4) {
                ui.displayMessage("Invalid input format for move. Try again (e.g., e2e4 or a7a8q).");
                continue;
            }

            Position startPos = parsePosition(moveStr.substr(0, 2));
            Position endPos = parsePosition(moveStr.substr(2, 2));
            PieceType promotionTarget = PieceType::NONE;

            if (moveStr.length() == 5) { // Potential promotion: e.g., e7e8q
                char promotionChar = moveStr[4];
                switch (promotionChar) {
                    case 'q': promotionTarget = PieceType::QUEEN; break;
                    case 'r': promotionTarget = PieceType::ROOK; break;
                    case 'b': promotionTarget = PieceType::BISHOP; break;
                    case 'n': promotionTarget = PieceType::KNIGHT; break;
                    default: 
                        ui.displayMessage("Invalid promotion piece: " + std::string(1, promotionChar) + ". Use q, r, b, or n.");
                        continue;
                }
            }
            
            // Validate pawn promotion input necessity
            Piece* p = board.getPiecePtr(startPos);
            if (p && p->getType() == PieceType::PAWN) {
                Color pieceColor = p->getColor();
                bool isPromotionSquare = (pieceColor == Color::WHITE && endPos.row == 0) || (pieceColor == Color::BLACK && endPos.row == 7);
                if (p->isValidMove(startPos, endPos, board) && isPromotionSquare && promotionTarget == PieceType::NONE) {
                     ui.displayMessage("Pawn promotion required. Append q, r, b, or n to your move (e.g. " + moveStr.substr(0,4) + "q).");
                     continue;
                }
            }

            moveMade = makeMove(startPos, endPos, promotionTarget);
            if (!moveMade) {
                ui.displayMessage("Please try your move again.");
            }
        }

        // Flip board for the NEXT player (the one about to move)
        Color nextColor = (currentPlayer == player1) ? player2->getColor() : player1->getColor();
        ui.displayBoard(board, nextColor == Color::BLACK);

        checkForEndOfRound();

        if (!isRoundOver()) {
            switchPlayer();
            if (board.isKingInCheck(currentPlayer->getColor())) {
                ui.displayMessage(currentPlayer->getName() + "'s King is in check!");
            }
        }
    }

    bool Game::canPlayerMakeAnyLegalMove(Player* player) {
        Color playerColor = player->getColor();
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position start(r, c);
                Piece* piece = board.getPiecePtr(start);
                if (piece && piece->getColor() == playerColor) {
                    std::vector<Position> possibleMoves = piece->getPossibleMoves(start, board);
                    for (const auto& end : possibleMoves) {
                        // Prevent moves that would capture the king
                        Piece* target = board.getPiecePtr(end);
                        if (target && target->getType() == PieceType::KING) continue;

                        // Simulate move to check if it resolves check or is legal
                        Board tempBoard = board;
                        tempBoard.movePiece(start, end); // Simulate on temp board
                        if (!tempBoard.isKingInCheck(playerColor)) {
                            return true; // Found a legal move
                        }
                    }
                }
            }
        }
        return false;
    }

    void Game::checkForEndOfRound() {
        Player* opponent = (currentPlayer == player1) ? player2 : player1;
        bool opponentInCheck = board.isKingInCheck(opponent->getColor());
        bool opponentHasLegalMoves = canPlayerMakeAnyLegalMove(opponent);

        if (opponentInCheck && !opponentHasLegalMoves) {
            roundState = (currentPlayer->getColor() == Color::WHITE) ? RoundState::CHECKMATE_WHITE_WINS : RoundState::CHECKMATE_BLACK_WINS;
            ui.displayMessage("Checkmate! " + currentPlayer->getName() + " wins the round.");
        } else if (!opponentInCheck && !opponentHasLegalMoves) {
            roundState = RoundState::STALEMATE;
            ui.displayMessage("Stalemate! The round is a draw.");
        }
        // Other draw conditions (50-move, threefold repetition) not implemented
    }

    void Game::switchPlayer() {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    bool Game::isRoundOver() const {
        return roundState != RoundState::ONGOING;
    }

    Player* Game::getRoundWinner() const {
        if (roundState == RoundState::CHECKMATE_WHITE_WINS) return player1;
        if (roundState == RoundState::CHECKMATE_BLACK_WINS) return player2;
        return nullptr; // Draw or ongoing
    }

} // namespace HardChess