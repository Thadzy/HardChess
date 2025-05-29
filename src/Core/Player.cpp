#include "HardChess/Core/Player.h"

namespace HardChess {

    Player::Player(const std::string& playerName, Color playerColor)
        : name(playerName), color(playerColor), hearts(3), score(0) {}

    void Player::loseHeart() {
        if (hearts > 0) {
            hearts--;
        }
    }

    void Player::incrementScore() {
        score++;
    }
    
    void Player::resetForNewMatch() {
        hearts = 3;
        score = 0;
    }

    const std::string& Player::getName() const {
        return name;
    }

    Color Player::getColor() const {
        return color;
    }

    std::string Player::getColorString() const {
        return (color == Color::WHITE) ? "White" : "Black";
    }

    int Player::getHearts() const {
        return hearts;
    }

    int Player::getScore() const {
        return score;
    }

} // namespace HardChess