/**
 * @file Player.h
 * @brief Defines the Player class for representing chess players
 *
 * @details This header file contains the Player class which encapsulates player
 * information and game state for a chess player. The class stores the player's name,
 * color (white or black), check status, score, and captured piece value. It provides
 * methods to access and modify these attributes, supporting the game's player management
 * system. The class implements the rule of three (copy constructor, assignment operator,
 * destructor) for proper resource management, though in this case it manages simple
 * data types that don't require special handling.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "Pieces.h"
#include <string>

/**
 * @class Player
 * @brief Represents a chess player with associated game state
 *
 * @details The Player class stores and manages player-specific information including
 * name, color, check status, score, and captured piece values. The isInCheck flag
 * tracks whether the player's king is currently in check, which is updated by the
 * game logic. The score can be used for tracking game performance, and capturedPieceValue
 * accumulates the total value of pieces captured by this player. The class provides
 * getters and setters for all attributes, with utility methods to check color and
 * reset game state. The reset() method allows reinitializing player state for a new game.
 */
class Player
{
private:
    std::string name;
    Color color;
    bool isInCheck;
    int score;
    int capturedPieceValue;

public:
    /**
     * @brief Default constructor
     * @details Initializes player with empty name, WHITE color, and zero values
     */
    Player();

    /**
     * @brief Parameterized constructor
     * @param playerName Name of the player
     * @param playerColor Color of the player (WHITE or BLACK)
     * @details Initializes player with specified name and color, sets other values to defaults
     */
    Player(const std::string &playerName, Color playerColor);

    /**
     * @brief Copy constructor
     * @param other Player object to copy from
     * @details Creates a deep copy of the player's data
     */
    Player(const Player &other);

    /**
     * @brief Assignment operator
     * @param other Player object to assign from
     * @return Reference to this Player object
     * @details Performs deep copy assignment, handling self-assignment
     */
    Player &operator=(const Player &other);

    /**
     * @brief Destructor
     * @details Cleans up player resources (currently no dynamic memory to manage)
     */
    ~Player();

    /**
     * @brief Gets the player's name
     * @return String containing the player's name
     */
    std::string getName() const;

    /**
     * @brief Gets the player's color
     * @return Color enum value (WHITE or BLACK)
     */
    Color getColor() const;

    /**
     * @brief Gets the check status
     * @return true if player's king is in check, false otherwise
     */
    bool getIsInCheck() const;

    /**
     * @brief Gets the player's score
     * @return Integer score value
     */
    int getScore() const;

    /**
     * @brief Gets the total value of captured pieces
     * @return Integer value representing total captured piece value
     * @details The value is calculated based on standard piece values:
     * Pawn=1, Knight/Bishop=3, Rook=5, Queen=9
     */
    int getCapturedPieceValue() const;

    /**
     * @brief Sets the player's name
     * @param playerName New name for the player
     */
    void setName(const std::string &playerName);

    /**
     * @brief Sets the check status
     * @param inCheck Boolean indicating if king is in check
     * @details Updated by game logic after each move to reflect current board state
     */
    void setIsInCheck(bool inCheck);

    /**
     * @brief Sets the player's score
     * @param newScore New score value
     */
    void setScore(int newScore);

    /**
     * @brief Sets the captured piece value
     * @param value New captured piece value
     */
    void setCapturedPieceValue(int value);

    /**
     * @brief Adds to the captured piece value
     * @param value Value to add to current captured piece total
     * @details Used when a piece is captured to increment the total value.
     * This method is preferred over setCapturedPieceValue() for captures.
     */
    void addCapturedPieceValue(int value);

    /**
     * @brief Checks if player is white
     * @return true if color is WHITE, false otherwise
     * @details Convenience method for color checking
     */
    bool isWhite() const;

    /**
     * @brief Checks if player is black
     * @return true if color is BLACK, false otherwise
     * @details Convenience method for color checking
     */
    bool isBlack() const;

    /**
     * @brief Resets player state for a new game
     * @details Resets check status, score, and captured piece value to zero.
     * Name and color remain unchanged.
     */
    void reset();
};

#endif
