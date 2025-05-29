CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS =
SOURCES = src/main.cpp \
          src/Core/Piece.cpp \
          src/Core/Pawn.cpp \
          src/Core/Rook.cpp \
          src/Core/Knight.cpp \
          src/Core/Bishop.cpp \
          src/Core/Queen.cpp \
          src/Core/King.cpp \
          src/Core/Board.cpp \
          src/Core/Player.cpp \
          src/Core/Game.cpp \
          src/UI/ConsoleUI.cpp
EXECUTABLE = HardChess

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE) $(LDFLAGS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) HardChess.dSYM # Added HardChess.dSYM for macOS debug symbols