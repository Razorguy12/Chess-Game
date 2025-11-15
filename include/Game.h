/**
 * @file Game.h
 * @brief Defines the Game class for managing chess game flow and logic
 * 
 * @details This header file contains the Game class which orchestrates the entire
 * chess game. It manages the board state, player turns, move validation, special
 * moves (castling, promotion, en passant), and game-ending conditions (checkmate,
 * stalemate). The class uses composition to contain a Board object and two Player
 * objects. It provides a custom exception class (ChessException) for handling
 * chess-specific errors. The game flow is managed through methods like start(),
 * playTurn(), and makeMove(), which handle user input, validate moves, and update
 * game state accordingly.
 */

#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "SpecialMoves.h"
#include "Player.h"
#include <string>
#include <stdexcept>

/**
 * @class ChessException
 * @brief Custom exception class for chess-specific errors
 * 
 * @details The ChessException class extends std::runtime_error to provide
 * chess-specific error messages. It is thrown when invalid moves are attempted,
 * such as moving opponent's pieces, moving into check, or attempting illegal
 * special moves. This allows the game to provide meaningful error feedback
 * to the user.
 */
class ChessException : public std::runtime_error {
public:
    /**
     * @brief Constructs a ChessException with an error message
     * @param msg Error message describing the chess rule violation
     */
    explicit ChessException(const std::string& msg) : std::runtime_error(msg) {}
};

/**
 * @class Game
 * @brief Manages the chess game flow, turns, and state
 * 
 * @details The Game class is the main controller for the chess game. It contains
 * a Board object for the game state and two Player objects (white and black).
 * The currentPlayer pointer tracks whose turn it is. The class handles the game
 * loop through start() and playTurn(), processes moves through makeMove(), and
 * manages special moves through handlePromotion() and handleCastling(). Move
 * validation includes checking piece ownership, move legality, and check conditions.
 * The game detects checkmate and stalemate by checking if the current player has
 * any legal moves available. Position parsing converts user input (e.g., "e2")
 * into Position objects for board manipulation.
 */
class Game {
private:
    Board board;
    Player whitePlayer;
    Player blackPlayer;
    Player* currentPlayer;
    bool gameOver;
    std::string winner;

public:
    /**
     * @brief Constructs a Game and initializes the board
     * @details Creates white and black players with default names, sets white
     * as the starting player, and initializes the board to the starting position
     */
    Game() : whitePlayer("White", Color::WHITE), blackPlayer("Black", Color::BLACK), 
             currentPlayer(&whitePlayer), gameOver(false) {
        board.initialize();
    }
    
    /**
     * @brief Starts the main game loop
     * @details Displays welcome message and game instructions, then enters a loop
     * that calls playTurn() until the game ends. Handles exceptions and displays
     * the final game result (winner or draw).
     */
    void start();
    
    /**
     * @brief Handles a single turn for the current player
     * @details Displays the board, shows current player and check status, prompts
     * for input, and processes the move. Handles special commands like quit and
     * castling notation (O-O, O-O-O).
     */
    void playTurn();
    
    /**
     * @brief Attempts to make a move from one position to another
     * @param from Source position in chess notation (e.g., "e2")
     * @param to Destination position in chess notation (e.g., "e4")
     * @return true if move was successful, false if invalid
     * @details Validates the move: checks piece exists, belongs to current player,
     * follows piece movement rules, doesn't leave king in check. Handles captures,
     * en passant, and pawn promotion. Updates player check status and switches turns.
     */
    bool makeMove(const std::string& from, const std::string& to);
    
    /**
     * @brief Parses a chess notation string into a Position object
     * @param pos String in chess notation (e.g., "e4", "a1")
     * @return Position object, or invalid position if parsing fails
     * @details Converts algebraic notation (file letter + rank number) to internal
     * 0-based row/column coordinates. Returns invalid position for malformed input.
     */
    Position parsePosition(const std::string& pos);
    
    /**
     * @brief Handles pawn promotion when a pawn reaches the opposite end
     * @param pos Position of the pawn to promote
     * @details Prompts user to choose promotion piece (Queen, Rook, Bishop, Knight)
     * and calls SpecialMoves::promotePawn() to replace the pawn.
     */
    void handlePromotion(const Position& pos);
    
    /**
     * @brief Handles castling move
     * @param command String indicating castling type ("kingside" or "queenside")
     * @details Validates castling conditions and performs the castling move using
     * SpecialMoves::performCastling(). Switches player turn after successful castling.
     */
    void handleCastling(const std::string& command);
    
    /**
     * @brief Switches the current player to the opponent
     * @details Toggles currentPlayer pointer between whitePlayer and blackPlayer
     */
    void switchPlayer() {
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
     * @details Allows access to current player's data (name, check status, etc.)
     */
    Player* getCurrentPlayerObject() const { return currentPlayer; }
    
    /**
     * @brief Gets a pointer to the white player object
     * @return Pointer to the white Player object
     */
    Player* getWhitePlayer() const { return const_cast<Player*>(&whitePlayer); }
    
    /**
     * @brief Gets a pointer to the black player object
     * @return Pointer to the black Player object
     */
    Player* getBlackPlayer() const { return const_cast<Player*>(&blackPlayer); }
    
    /**
     * @brief Checks if the game has ended
     * @return true if game is over (checkmate or stalemate), false otherwise
     */
    bool isGameOver() const { return gameOver; }
    
    /**
     * @brief Checks if a player has any valid moves available
     * @param color Color of the player to check
     * @return true if player has at least one legal move, false otherwise
     * @details Iterates through all pieces of the specified color and checks if
     * any can make a legal move (valid pattern and doesn't leave king in check).
     * Used to detect checkmate and stalemate conditions.
     */
    bool hasValidMoves(Color color);
    
    /**
     * @brief Checks game status and updates gameOver and winner if game ends
     * @details Checks if current player is in check and has valid moves. If no
     * valid moves exist, ends the game: checkmate if in check, stalemate otherwise.
     * Updates the winner string accordingly.
     */
    void checkGameStatus();
};

#endif