/**
 * @file Pieces.h
 * @brief Defines the chess piece hierarchy and Color enumeration
 * 
 * @details This header file contains the abstract base class Piece and all concrete
 * piece implementations (Pawn, Rook, Knight, Bishop, Queen, King). The design uses
 * polymorphism where Piece is an abstract base class with a pure virtual function
 * isValidMove() that each derived class must implement according to its specific
 * movement rules. The Color enumeration represents the two sides in chess (WHITE and BLACK).
 * Each piece stores its color, position, movement status, and symbol. The class hierarchy
 * demonstrates object-oriented principles including abstraction, polymorphism, and inheritance.
 */

#ifndef PIECES_H
#define PIECES_H

#include "Position.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @enum Color
 * @brief Represents the two colors in chess
 * @details Used to distinguish between white and black pieces and players
 */
enum class Color { WHITE, BLACK };

/**
 * @class Piece
 * @brief Abstract base class for all chess pieces
 * 
 * @details The Piece class serves as the foundation for all chess pieces using
 * polymorphism and abstraction. It defines common attributes (color, position,
 * movement status, symbol) and provides a pure virtual function isValidMove()
 * that each derived class must implement with its specific movement rules.
 * The class uses protected members to allow derived classes direct access while
 * maintaining encapsulation from external code. The hasMoved flag tracks whether
 * a piece has moved, which is important for special moves like castling and
 * pawn double-move. The template function isType() enables runtime type checking
 * using dynamic_cast, allowing the game logic to identify specific piece types.
 */
class Piece {
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
     * @details Initializes all member variables, setting hasMoved to false
     */
    Piece(Color c, Position pos, char sym) 
        : color(c), position(pos), hasMoved(false), symbol(sym) {}
    
    /**
     * @brief Virtual destructor for proper polymorphic destruction
     * @details Ensures derived class destructors are called when deleting through base pointer
     */
    virtual ~Piece() = default;
    
    /**
     * @brief Pure virtual function to validate piece movement
     * @param to Destination position
     * @param board Reference to the game board
     * @return true if the move is valid according to piece rules, false otherwise
     * @details Each derived class implements this with its specific movement rules.
     * This function checks if the move follows the piece's movement pattern but
     * does not check for check conditions or board state beyond piece-specific rules.
     */
    virtual bool isValidMove(const Position& to, const class Board& board) const = 0;
    
    /**
     * @brief Gets the display symbol for the piece
     * @return Unicode chess piece string
     * @details Returns Unicode chess piece symbols: white pieces use filled symbols
     * (♔ ♕ ♖ ♗ ♘ ♙) and black pieces use outlined symbols (♚ ♛ ♜ ♝ ♞ ♟)
     */
    virtual std::string getSymbol() const;
    
    /**
     * @brief Pure virtual function to get piece name
     * @return String name of the piece type
     * @details Each derived class returns its specific name (e.g., "Pawn", "Rook")
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
     * @details Automatically sets hasMoved to true when position changes,
     * which is important for special moves like castling
     */
    void setPosition(const Position& pos) { position = pos; hasMoved = true; }
    
    /**
     * @brief Checks if the piece has moved from its initial position
     * @return true if piece has moved, false otherwise
     * @details Used to determine eligibility for special moves like castling
     */
    bool hasMovedBefore() const { return hasMoved; }
    
    /**
     * @brief Sets the movement status of the piece
     * @param moved Boolean indicating if piece has moved
     * @details Allows manual control of movement status, useful for board setup
     */
    void setHasMoved(bool moved) { hasMoved = moved; }
    
    /**
     * @brief Template function for runtime type checking
     * @tparam T Type to check against
     * @return true if piece is of type T, false otherwise
     * @details Uses dynamic_cast to perform safe downcasting and type identification.
     * Example: piece->isType<Pawn>() returns true if piece is a Pawn instance.
     */
    template<typename T>
    bool isType() const {
        return dynamic_cast<const T*>(this) != nullptr;
    }
};

/**
 * @class Pawn
 * @brief Represents a pawn chess piece
 * 
 * @details The Pawn class implements pawn-specific movement rules including:
 * forward movement (one or two squares from starting position), diagonal captures,
 * en passant eligibility, and promotion detection. Pawns move differently than
 * they capture, making them unique among chess pieces.
 */
class Pawn : public Piece {
public:
    /**
     * @brief Constructs a Pawn piece
     * @param c Color of the pawn
     * @param pos Initial position
     */
    Pawn(Color c, Position pos) : Piece(c, pos, 'P') {}
    bool isValidMove(const Position& to, const class Board& board) const override;
    std::string getName() const override { return "Pawn"; }
};

/**
 * @class Rook
 * @brief Represents a rook chess piece
 * 
 * @details The Rook class implements rook movement: horizontal and vertical
 * movement along ranks and files. Rooks can move any number of squares in
 * straight lines but cannot jump over other pieces. The hasMoved flag is
 * critical for castling eligibility.
 */
class Rook : public Piece {
public:
    /**
     * @brief Constructs a Rook piece
     * @param c Color of the rook
     * @param pos Initial position
     */
    Rook(Color c, Position pos) : Piece(c, pos, 'R') {}
    bool isValidMove(const Position& to, const class Board& board) const override;
    std::string getName() const override { return "Rook"; }
};

/**
 * @class Knight
 * @brief Represents a knight chess piece
 * 
 * @details The Knight class implements the L-shaped movement pattern unique
 * to knights. Knights are the only pieces that can jump over other pieces,
 * moving in an L-shape (two squares in one direction, then one square perpendicular).
 */
class Knight : public Piece {
public:
    /**
     * @brief Constructs a Knight piece
     * @param c Color of the knight
     * @param pos Initial position
     */
    Knight(Color c, Position pos) : Piece(c, pos, 'N') {}
    bool isValidMove(const Position& to, const class Board& board) const override;
    std::string getName() const override { return "Knight"; }
};

/**
 * @class Bishop
 * @brief Represents a bishop chess piece
 * 
 * @details The Bishop class implements diagonal movement. Bishops move any
 * number of squares diagonally but cannot jump over other pieces. Each player
 * starts with two bishops on opposite-colored squares, and they remain on
 * those colors throughout the game.
 */
class Bishop : public Piece {
public:
    /**
     * @brief Constructs a Bishop piece
     * @param c Color of the bishop
     * @param pos Initial position
     */
    Bishop(Color c, Position pos) : Piece(c, pos, 'B') {}
    bool isValidMove(const Position& to, const class Board& board) const override;
    std::string getName() const override { return "Bishop"; }
};

/**
 * @class Queen
 * @brief Represents a queen chess piece
 * 
 * @details The Queen class combines the movement capabilities of both Rook
 * and Bishop, allowing movement in any straight line (horizontal, vertical,
 * or diagonal) for any number of squares. The queen is the most powerful
 * piece on the board.
 */
class Queen : public Piece {
public:
    /**
     * @brief Constructs a Queen piece
     * @param c Color of the queen
     * @param pos Initial position
     */
    Queen(Color c, Position pos) : Piece(c, pos, 'Q') {}
    bool isValidMove(const Position& to, const class Board& board) const override;
    std::string getName() const override { return "Queen"; }
};

/**
 * @class King
 * @brief Represents a king chess piece
 * 
 * @details The King class implements limited movement (one square in any direction)
 * and is central to game-ending conditions (check, checkmate, stalemate). The
 * hasMoved flag is essential for castling eligibility. The king cannot move
 * into check, which is enforced by the game logic rather than the piece itself.
 */
class King : public Piece {
public:
    /**
     * @brief Constructs a King piece
     * @param c Color of the king
     * @param pos Initial position
     */
    King(Color c, Position pos) : Piece(c, pos, 'K') {}
    bool isValidMove(const Position& to, const class Board& board) const override;
    std::string getName() const override { return "King"; }
};

#endif