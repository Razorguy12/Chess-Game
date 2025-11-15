#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"
#include <memory>
#include <vector>

class Board {
private:
    // Composition: Board contains pieces (Dynamic memory)
    std::unique_ptr<Piece> squares[8][8];
    Position enPassantTarget;
    bool enPassantAvailable;

public:
    Board();
    ~Board() = default;
    
    void initialize();
    void display() const;
    
    Piece* getPiece(const Position& pos) const;
    Piece* getPiece(int row, int col) const;
    bool isEmpty(const Position& pos) const;
    bool isEmpty(int row, int col) const;
    
    // Function overloading
    bool movePiece(const Position& from, const Position& to);
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);
    
    void setPiece(const Position& pos, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(const Position& pos);
    
    bool isPathClear(const Position& from, const Position& to) const;
    bool isUnderAttack(const Position& pos, Color byColor) const;
    
    Position getKingPosition(Color color) const;
    bool isInCheck(Color color) const;
    bool wouldBeInCheck(const Position& from, const Position& to, Color color);
    
    // En passant
    void setEnPassantTarget(const Position& pos) { 
        enPassantTarget = pos; 
        enPassantAvailable = true; 
    }
    void clearEnPassant() { enPassantAvailable = false; }
    bool isEnPassantAvailable() const { return enPassantAvailable; }
    Position getEnPassantTarget() const { return enPassantTarget; }
};

#endif