#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "SpecialMoves.h"
#include <string>
#include <stdexcept>

// Custom exception class (Exception Handling)
class ChessException : public std::runtime_error {
public:
    explicit ChessException(const std::string& msg) : std::runtime_error(msg) {}
};

class Game {
private:
    Board board;
    Color currentPlayer;
    bool gameOver;
    std::string winner;

public:
    Game() : currentPlayer(Color::WHITE), gameOver(false) {
        board.initialize();
    }
    
    void start();
    void playTurn();
    bool makeMove(const std::string& from, const std::string& to);
    
    Position parsePosition(const std::string& pos);
    void handlePromotion(const Position& pos);
    void handleCastling(const std::string& command);
    
    void switchPlayer() {
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
    
    Color getCurrentPlayer() const { return currentPlayer; }
    bool isGameOver() const { return gameOver; }
    
    bool hasValidMoves(Color color);
    void checkGameStatus();
};

#endif