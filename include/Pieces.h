#ifndef PIECES_H
#define PIECES_H

#include "Position.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @enum Color
 * @brief Represents the two colors in chess
 */
enum class Color
{
    WHITE,
    BLACK
};

/**
 * @class Piece
 * @brief Abstract base class for all chess pieces
 */
class Piece
{
protected:
    Color color;
    Position position;
    bool hasMoved;
    char symbol;

public:
    /**
     * @brief Constructs a Piece with specified attributes
     * @param c Color of the piece (WHITE or BLACK)
     * @param pos Initial position on the board
     * @param sym Symbol character for the piece (uppercase)
     */
    Piece(Color c, Position pos, char sym)
        : color(c), position(pos), hasMoved(false), symbol(sym) {}

    /**
     * @brief Virtual destructor for proper polymorphic destruction
     */
    virtual ~Piece() = default;

    /**
     * @brief Pure virtual function to validate piece movement
     * @param to Destination position
     * @param board Reference to the game board
     * @return true if the move is valid according to piece rules, false otherwise
     */
    virtual bool isValidMove(const Position &to, const class Board &board) const = 0;

    /**
     * @brief Gets the display symbol for the piece
     * @return Unicode chess piece string
     */
    virtual std::string getSymbol() const;

    /**
     * @brief Pure virtual function to get piece name
     * @return String name of the piece type
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Gets the color of the piece
     * @return Color enum value (WHITE or BLACK)
     */
    Color getColor() const { return color; }

    /**
     * @brief Gets the current position of the piece
     * @return Position object representing current location
     */
    Position getPosition() const { return position; }

    /**
     * @brief Sets the position and marks piece as moved
     * @param pos New position for the piece
     */
    void setPosition(const Position &pos)
    {
        position = pos;
        hasMoved = true;
    }

    /**
     * @brief Checks if the piece has moved from its initial position
     * @return true if piece has moved, false otherwise
     */
    bool hasMovedBefore() const { return hasMoved; }

    /**
     * @brief Sets the movement status of the piece
     * @param moved Boolean indicating if piece has moved
     */
    void setHasMoved(bool moved) { hasMoved = moved; }

    /**
     * @brief Template function for runtime type checking
     * @tparam T Type to check against
     * @return true if piece is of type T, false otherwise
     */
    template <typename T>
    bool isType() const
    {
        return dynamic_cast<const T *>(this) != nullptr;
    }
};

/**
 * @class Pawn
 * @brief Represents a pawn chess piece
 */
class Pawn : public Piece
{
public:
    /**
     * @brief Constructs a Pawn piece
     * @param c Color of the pawn
     * @param pos Initial position
     */
    Pawn(Color c, Position pos) : Piece(c, pos, 'P') {}
    bool isValidMove(const Position &to, const class Board &board) const override;
    std::string getName() const override { return "Pawn"; }
};

/**
 * @class Rook
 * @brief Represents a rook chess piece
 */
class Rook : public Piece
{
public:
    /**
     * @brief Constructs a Rook piece
     * @param c Color of the rook
     * @param pos Initial position
     */
    Rook(Color c, Position pos) : Piece(c, pos, 'R') {}
    bool isValidMove(const Position &to, const class Board &board) const override;
    std::string getName() const override { return "Rook"; }
};

/**
 * @class Knight
 * @brief Represents a knight chess piece
 */
class Knight : public Piece
{
public:
    /**
     * @brief Constructs a Knight piece
     * @param c Color of the knight
     * @param pos Initial position
     */
    Knight(Color c, Position pos) : Piece(c, pos, 'N') {}
    bool isValidMove(const Position &to, const class Board &board) const override;
    std::string getName() const override { return "Knight"; }
};

/**
 * @class Bishop
 * @brief Represents a bishop chess piece
 */
class Bishop : public Piece
{
public:
    /**
     * @brief Constructs a Bishop piece
     * @param c Color of the bishop
     * @param pos Initial position
     */
    Bishop(Color c, Position pos) : Piece(c, pos, 'B') {}
    bool isValidMove(const Position &to, const class Board &board) const override;
    std::string getName() const override { return "Bishop"; }
};

/**
 * @class Queen
 * @brief Represents a queen chess piece
 */
class Queen : public Piece
{
public:
    /**
     * @brief Constructs a Queen piece
     * @param c Color of the queen
     * @param pos Initial position
     */
    Queen(Color c, Position pos) : Piece(c, pos, 'Q') {}
    bool isValidMove(const Position &to, const class Board &board) const override;
    std::string getName() const override { return "Queen"; }
};

/**
 * @class King
 * @brief Represents a king chess piece
 */
class King : public Piece
{
public:
    /**
     * @brief Constructs a King piece
     * @param c Color of the king
     * @param pos Initial position
     */
    King(Color c, Position pos) : Piece(c, pos, 'K') {}
    bool isValidMove(const Position &to, const class Board &board) const override;
    std::string getName() const override { return "King"; }
};

#endif