#ifndef PLAYER_H
#define PLAYER_H

#include "Pieces.h"
#include <string>

/**
 * @class Player
 * @brief Represents a chess player with associated game state
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
     */
    Player();

    /**
     * @brief Parameterized constructor
     * @param playerName Name of the player
     * @param playerColor Color of the player (WHITE or BLACK)
     */
    Player(const std::string &playerName, Color playerColor);

    /**
     * @brief Copy constructor
     * @param other Player object to copy from
     */
    Player(const Player &other);

    /**
     * @brief Assignment operator
     * @param other Player object to assign from
     * @return Reference to this Player object
     */
    Player &operator=(const Player &other);

    /**
     * @brief Destructor
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
     */
    void addCapturedPieceValue(int value);

    /**
     * @brief Checks if player is white
     * @return true if color is WHITE, false otherwise
     */
    bool isWhite() const;

    /**
     * @brief Checks if player is black
     * @return true if color is BLACK, false otherwise
     */
    bool isBlack() const;

    /**
     * @brief Resets player state for a new game
     */
    void reset();
};

#endif
