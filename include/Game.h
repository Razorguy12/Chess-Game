#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "SpecialMoves.h"
#include "Player.h"
#include <string>
#include <stdexcept>

/**
 * @class Game
 * @brief Manages the chess game flow, turns, and state
 */
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
    /**
     * @brief Constructs a Game and initializes the board
     */
    Game() : whitePlayer("White", Color::WHITE),
             blackPlayer("Black", Color::BLACK),
             currentPlayer(&whitePlayer),
             gameOver(false)
    {
        board.initialize();
    }

    /**
     * @brief Starts the main game loop
     */
    void start();

    /**
     * @brief Handles a single turn for the current player
     */
    void playTurn();

    /**
     * @brief Attempts to make a move from one position to another
     * @param from Source position in chess notation (e.g., "e2")
     * @param to Destination position in chess notation (e.g., "e4")
     * @return true if move was successful, false if invalid
     */
    bool makeMove(const std::string &from, const std::string &to);

    /**
     * @brief Parses a chess notation string into a Position object
     * @param pos String in chess notation (e.g., "e4", "a1")
     * @return Position object, or invalid position if parsing fails
     */
    Position parsePosition(const std::string &pos);

    /**
     * @brief Handles pawn promotion when a pawn reaches the opposite end
     * @param pos Position of the pawn to promote
     */
    void handlePromotion(const Position &pos);

    /**
     * @brief Handles castling move
     * @param command String indicating castling type ("kingside" or "queenside")
     */
    void handleCastling(const std::string &command);

    /**
     * @brief Switches the current player to the opponent
     * @details Toggles currentPlayer pointer between whitePlayer and blackPlayer
     */
    void switchPlayer()
    {
        currentPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
    }

    /**
     * @brief Gets the color of the current player
     * @return Color enum value (WHITE or BLACK) of current player
     */
    Color getCurrentPlayer() const { return currentPlayer->getColor(); }

    /**
     * @brief Gets a pointer to the current player object
     * @return Pointer to the current Player object
     */
    Player *getCurrentPlayerObject() const { return currentPlayer; }

    /**
     * @brief Gets a pointer to the white player object
     * @return Pointer to the white Player object
     */
    Player *getWhitePlayer() { return &whitePlayer; }

    /**
     * @brief Gets a pointer to the black player object
     * @return Pointer to the black Player object
     */
    Player *getBlackPlayer() { return &blackPlayer; }

    /**
     * @brief Checks if the game has ended
     * @return true if game is over (checkmate or stalemate), false otherwise
     */
    bool isGameOver() const { return gameOver; }

    /**
     * @brief Checks if a player has any valid moves available
     * @param color Color of the player to check
     * @return true if player has at least one legal move, false otherwise
     */
    bool hasValidMoves(Color color);

    /**
     * @brief Checks game status and updates gameOver and winner if game ends
     */
    void checkGameStatus();
};

#endif
