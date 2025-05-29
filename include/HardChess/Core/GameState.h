#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>

// Removed POWER_UP_SELECTION from GamePhase for HardChess
enum class GamePhase {
    MENU, PLAYING, GAME_OVER, PAUSED
};

class GameState {
private:
    GamePhase currentPhase;
    int roundNumber;
    std::string lastMoveDescription;
    bool isCheck;
    bool isCheckmate;
    std::vector<std::string> gameHistory;
    
public:
    GameState();
    
    // Phase management
    GamePhase getCurrentPhase() const { return currentPhase; }
    void setPhase(GamePhase phase) { currentPhase = phase; }
    
    // Round management
    int getRoundNumber() const { return roundNumber; }
    void incrementRound() { roundNumber++; }
    void resetRound() { roundNumber = 1; }
    
    // Game status
    void setCheck(bool check) { isCheck = check; }
    void setCheckmate(bool checkmate) { isCheckmate = checkmate; }
    bool getCheck() const { return isCheck; }
    bool getCheckmate() const { return isCheckmate; }
    
    // Move history
    void addMove(const std::string& move);
    std::vector<std::string> getHistory() const { return gameHistory; }
    void clearHistory() { gameHistory.clear(); }
    
    // Utility
    std::string getPhaseString() const;
};

#endif // GAMESTATE_H