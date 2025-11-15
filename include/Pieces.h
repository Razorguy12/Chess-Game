#ifndef PIECES_H
#define PIECES_H

#include "Position.h"
#include <vector>
#include <memory>

enum class Color { WHITE, BLACK };

// Abstract base class for all pieces (Abstraction & Polymorphism)
class Piece {
protected:
    Color color;
    Position position;
    bool hasMoved;
    char symbol;

public:
    Piece(Color c, Position pos, char sym) 
        : color(c), position(pos), hasMoved(false), symbol(sym) {}
    
    virtual ~Piece() = default;
    
    // Pure virtual function (Abstraction)
    virtual bool isValidMove(const Position& to, class Board& board) const = 0;
    virtual char getSymbol() const { return color == Color::WHITE ? symbol : symbol + 32; }
    virtual std::string getName() const = 0;
    
    Color getColor() const { return color; }
    Position getPosition() const { return position; }
    void setPosition(const Position& pos) { position = pos; hasMoved = true; }
    bool hasMovedBefore() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }
    
    // Template function
    template<typename T>
    bool isType() const {
        return dynamic_cast<const T*>(this) != nullptr;
    }
};

class Pawn : public Piece {
public:
    Pawn(Color c, Position pos) : Piece(c, pos, 'P') {}
    bool isValidMove(const Position& to, class Board& board) const override;
    std::string getName() const override { return "Pawn"; }
};

class Rook : public Piece {
public:
    Rook(Color c, Position pos) : Piece(c, pos, 'R') {}
    bool isValidMove(const Position& to, class Board& board) const override;
    std::string getName() const override { return "Rook"; }
};

class Knight : public Piece {
public:
    Knight(Color c, Position pos) : Piece(c, pos, 'N') {}
    bool isValidMove(const Position& to, class Board& board) const override;
    std::string getName() const override { return "Knight"; }
};

class Bishop : public Piece {
public:
    Bishop(Color c, Position pos) : Piece(c, pos, 'B') {}
    bool isValidMove(const Position& to, class Board& board) const override;
    std::string getName() const override { return "Bishop"; }
};

class Queen : public Piece {
public:
    Queen(Color c, Position pos) : Piece(c, pos, 'Q') {}
    bool isValidMove(const Position& to, class Board& board) const override;
    std::string getName() const override { return "Queen"; }
};

class King : public Piece {
public:
    King(Color c, Position pos) : Piece(c, pos, 'K') {}
    bool isValidMove(const Position& to, class Board& board) const override;
    std::string getName() const override { return "King"; }
};

#endif