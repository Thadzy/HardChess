#ifndef HARDCHESS_UI_CONSOLEUI_H
#define HARDCHESS_UI_CONSOLEUI_H

#include <string>
#include "HardChess/Core/CommonTypes.h"

// Forward declarations to minimize include dependencies in header
namespace HardChess {
    class Board;
    class Player;
    class Piece;
    enum class PieceType;
    class Position;
}

namespace HardChess {

    class ConsoleUI {
      public:
        ConsoleUI() = default;

        // Main menu and help
        int displayMainMenu() const;
        void displayHelpAndRules() const;

        // Game UI
        void displayBoard(const Board& board, bool flip = false) const;
        void displayPlayerStats(const Player& player1, const Player& player2) const;
        void displayPlayerTurn(const Player* currentPlayer) const;
        void displayMessage(const std::string& message) const;

        std::string getPlayerMove(const Player& player) const;
        std::string promptForPieceSelection(const std::string& promptMessage) const;

        void displayRoundResult(Player* winner) const; // nullptr for draw
        void displayMatchResult(Player* winner) const; // nullptr for draw

        std::string formatPosition(Position pos) const; // Helper to convert Position to "a1" string

        PieceType promptPawnPromotionChoice(const Player& player) const;
    };

} // namespace HardChess

#endif // HARDCHESS_UI_CONSOLEUI_H