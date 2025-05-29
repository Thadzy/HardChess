#include "HardChess/Core/Game.h"
#include "HardChess/Core/Player.h"
#include "HardChess/UI/ConsoleUI.h"
#include <iostream>

using namespace HardChess;

int main() {
    ConsoleUI ui;

    ui.displayMessage("Welcome to HardChess!");
    int roundsToWinMatch = 2;

    while (true) {
        int menuChoice = ui.displayMainMenu();
        if (menuChoice == 1) {
            // Start Game
            std::string name1, name2;
            std::cout << "Enter name for Player 1 (White): ";
            std::cin >> name1;
            std::cout << "Enter name for Player 2 (Black): ";
            std::cin >> name2;

            Player player1(name1, Color::WHITE);
            Player player2(name2, Color::BLACK);

            int currentRoundNumber = 1;
            ui.displayMessage("Win " + std::to_string(roundsToWinMatch) + " rounds to win the match. Each player has 3 hearts.");

            while (player1.getScore() < roundsToWinMatch && player2.getScore() < roundsToWinMatch &&
                   player1.getHearts() > 0 && player2.getHearts() > 0) {
                
                ui.displayMessage("\n--- Starting Round " + std::to_string(currentRoundNumber) + " ---");
                ui.displayPlayerStats(player1, player2);

                Game currentRound(&player1, &player2, ui);
                currentRound.startRound();

                while (!currentRound.isRoundOver()) {
                    currentRound.playTurn();
                }

                Player* roundWinner = currentRound.getRoundWinner();
                Player* roundLoser = nullptr;

                if (roundWinner) {
                    roundWinner->incrementScore();
                    ui.displayRoundResult(roundWinner);
                    
                    if (roundWinner == &player1) roundLoser = &player2;
                    else roundLoser = &player1;

                    roundLoser->loseHeart();
                    ui.displayMessage(roundLoser->getName() + " loses a heart! Hearts remaining: " + std::to_string(roundLoser->getHearts()));

                    if (roundLoser->getHearts() == 0) {
                        ui.displayMessage(roundLoser->getName() + " has run out of hearts and loses the match!");
                        if (roundWinner->getScore() < roundsToWinMatch) {
                            while (roundWinner->getScore() < roundsToWinMatch) roundWinner->incrementScore();
                        }
                        break;
                    }
                } else {
                    ui.displayRoundResult(nullptr);
                    ui.displayMessage("No hearts lost this round.");
                }
                currentRoundNumber++;
            }

            ui.displayMessage("\n--- Match Finished ---");
            ui.displayPlayerStats(player1, player2);

            if (player1.getScore() >= roundsToWinMatch) {
                ui.displayMatchResult(&player1);
            } else if (player2.getScore() >= roundsToWinMatch) {
                ui.displayMatchResult(&player2);
            } else if (player1.getHearts() == 0 && player2.getHearts() > 0) {
                ui.displayMessage(player1.getName() + " ran out of hearts.");
                ui.displayMatchResult(&player2);
            } else if (player2.getHearts() == 0 && player1.getHearts() > 0) {
                ui.displayMessage(player2.getName() + " ran out of hearts.");
                ui.displayMatchResult(&player1);
            } else {
                ui.displayMessage("The match outcome is undetermined by score or hearts (edge case).");
            }
        } else if (menuChoice == 2) {
            // Help & Rules
            ui.displayHelpAndRules();
        } else if (menuChoice == 3) {
            // Exit
            ui.displayMessage("Exiting. Goodbye!");
            break;
        }
    }

    return 0;
}