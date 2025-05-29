#include "HardChess/UI/ConsoleUI.h"
#include "HardChess/Core/Board.h"
#include "HardChess/Core/Player.h"
#include <iostream>
#include <limits>

namespace HardChess
{

    int ConsoleUI::displayMainMenu() const
    {
        std::cout << "==============================" << std::endl;
        std::cout << "      HARD CHESS MAIN MENU    " << std::endl;
        std::cout << "==============================" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. Help & Rules" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice (1-3): ";
        int choice;
        std::cin >> choice;
        while (std::cin.fail() || choice < 1 || choice > 3)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter 1, 2, or 3: ";
            std::cin >> choice;
        }
        return choice;
    }

    void ConsoleUI::displayHelpAndRules() const
    {
        std::cout << "\n--- Help & Rules ---\n";
        std::cout << "Standard chess rules apply.\n";
        std::cout << "- Enter moves in algebraic notation (e.g., e2e4).\n";
        std::cout << "- For pawn promotion, append q, r, b, or n (e.g., a7a8q).\n";
        std::cout << "- Type 'exit' at any move prompt to quit the game.\n";
        std::cout << "- The game ends with checkmate or stalemate.\n";
        std::cout << "---------------------\n\n";
    }

    void ConsoleUI::displayBoard(const Board &board, bool /*flip*/) const
    {
        std::cout << std::endl;

        int startRow = 0;
        int endRow = 8;
        int stepRow = 1;

        int startCol = 0;
        int endCol = 8;
        int stepCol = 1;

        // Print column labels
        std::cout << "    ";
        for (int c = startCol; c != endCol; c += stepCol)
        {
            std::cout << static_cast<char>('a' + c) << " ";
        }
        std::cout << std::endl;

        std::cout << "  +-----------------+" << std::endl;
        for (int r = startRow; r != endRow; r += stepRow)
        {
            int rowLabel = 8 - r;
            std::cout << rowLabel << " | ";
            for (int c = startCol; c != endCol; c += stepCol)
            {
                Piece *piece = board.getPiecePtr(Position(r, c));
                if (piece)
                {
                    std::cout << piece->getSymbol() << " ";
                }
                else
                {
                    std::cout << ((r + c) % 2 == 0 ? "." : " ") << " ";
                }
            }
            std::cout << "| " << rowLabel << std::endl;
        }
        std::cout << "  +-----------------+" << std::endl;

        // Print column labels again
        std::cout << "    ";
        for (int c = startCol; c != endCol; c += stepCol)
        {
            std::cout << static_cast<char>('a' + c) << " ";
        }
        std::cout << std::endl
                  << std::endl;
    }

    void ConsoleUI::displayPlayerStats(const Player &player1, const Player &player2) const
    {
        std::cout << "--- Player Stats ---" << std::endl;
        std::cout << player1.getName() << " (White): Hearts=" << player1.getHearts() << ", Score=" << player1.getScore() << std::endl;
        std::cout << player2.getName() << " (Black): Hearts=" << player2.getHearts() << ", Score=" << player2.getScore() << std::endl;
        std::cout << "--------------------" << std::endl;
    }

    void ConsoleUI::displayPlayerTurn(const Player *currentPlayer) const
    {
        if (currentPlayer)
        {
            std::cout << "--- " << currentPlayer->getName() << "'s Turn (" << currentPlayer->getColorString() << ") ---" << std::endl;
        }
    }

    void ConsoleUI::displayMessage(const std::string &message) const
    {
        std::cout << "[INFO] " << message << std::endl;
    }

    std::string ConsoleUI::getPlayerMove(const Player &player) const
    {
        std::string moveStr;
        std::cout << player.getName() << ", enter your move (e.g., e2e4 or a7a8q for promotion, or type 'exit' to quit): ";
        std::cin >> moveStr;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return "";
        }
        if (moveStr == "exit" || moveStr == "quit")
        {
            std::cout << "[INFO] Exiting the game. Goodbye!" << std::endl;
            exit(0);
        }
        return moveStr;
    }

    std::string ConsoleUI::promptForPieceSelection(const std::string &promptMessage) const
    {
        std::string posStr;
        std::cout << promptMessage;
        std::cin >> posStr;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return "";
        }
        return posStr;
    }

    void ConsoleUI::displayRoundResult(Player *winner) const
    {
        if (winner)
        {
            std::cout << ">>> Round Over! Winner: " << winner->getName() << " (" << winner->getColorString() << ") <<<" << std::endl;
        }
        else
        {
            std::cout << ">>> Round Over! It's a draw (Stalemate). <<<" << std::endl;
        }
    }

    void ConsoleUI::displayMatchResult(Player *winner) const
    {
        std::cout << "\n=========================" << std::endl;
        if (winner)
        {
            std::cout << ">>> MATCH OVER! Congratulations " << winner->getName() << ", you won the match! <<<" << std::endl;
        }
        else
        {
            std::cout << ">>> MATCH OVER! The match is a draw! <<<" << std::endl;
        }
        std::cout << "=========================\n"
                  << std::endl;
    }

    std::string ConsoleUI::formatPosition(Position pos) const
    {
        if (!pos.isValid())
            return "??";
        char file = 'a' + pos.col;
        char rank = '1' + (7 - pos.row);
        return std::string(1, file) + std::string(1, rank);
    }

    PieceType ConsoleUI::promptPawnPromotionChoice(const Player &player) const
    {
        std::cout << player.getName() << ", your pawn can be promoted!" << std::endl;
        std::cout << "Choose piece to promote to (q=Queen, r=Rook, b=Bishop, n=Knight): ";
        char choiceChar;
        while (true)
        {
            std::cin >> choiceChar;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            switch (tolower(choiceChar))
            {
            case 'q':
                return PieceType::QUEEN;
            case 'r':
                return PieceType::ROOK;
            case 'b':
                return PieceType::BISHOP;
            case 'n':
                return PieceType::KNIGHT;
            default:
                std::cout << "Invalid choice. Enter q, r, b, or n: ";
            }
        }
    }

} // namespace HardChess