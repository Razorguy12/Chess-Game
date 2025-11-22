/**
 * @file Game.h
 * @brief Defines the Game class for managing chess game flow and logic
 */

#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "SpecialMoves.h"
#include "Player.h"
#include <string>
#include <stdexcept>


class Game
{
private:
    Board board;
    Player whitePlayer;
    Player blackPlayer;
    Player *currentPlayer;
    bool gameOver;
    std::string winner;

public:
    Game() : whitePlayer("White", Color::WHITE),
             blackPlayer("Black", Color::BLACK),
             currentPlayer(&whitePlayer),
             gameOver(false)
    {
        board.initialize();
    }

    void start();
    void playTurn();
    bool makeMove(const std::string &from, const std::string &to);
    Position parsePosition(const std::string &pos);
    void handlePromotion(const Position &pos);
    void handleCastling(const std::string &command);

    void switchPlayer()
    {
        currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
    }

    Color getCurrentPlayer() const { return currentPlayer->getColor(); }
    Player *getCurrentPlayerObject() const { return currentPlayer; }

    /**
     * @brief Get pointer to white player 
     */
    Player *getWhitePlayer() { return &whitePlayer; }

    /**
     * @brief Get pointer to black player 
     */
    Player *getBlackPlayer() { return &blackPlayer; }

    bool isGameOver() const { return gameOver; }
    bool hasValidMoves(Color color);
    void checkGameStatus();
};

#endif
