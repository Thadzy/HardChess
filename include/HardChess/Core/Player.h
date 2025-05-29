#ifndef HARDCHESS_CORE_PLAYER_H
#define HARDCHESS_CORE_PLAYER_H

#include "HardChess/Core/CommonTypes.h"
#include <string>

namespace HardChess {

    class Player {
      private:
        std::string name;
        Color color;
        int hearts;
        int score; // Rounds won

      public:
        Player(const std::string& name, Color c);

        const std::string& getName() const;
        Color getColor() const;
        int getHearts() const;
        int getScore() const;
        
        std::string getColorString() const;

        void loseHeart();
        void incrementScore();
        void resetForNewMatch();
    };

} // namespace HardChess

#endif // HARDCHESS_CORE_PLAYER_H