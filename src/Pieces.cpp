#include "Pieces.h"
#include "Board.h"
#include <cmath>

bool Pawn::isValidMove(const Position& to, Board& board) const {
    int rowDiff = to.getRow() - position.getRow();
    int colDiff = std::abs(to.getCol() - position.getCol());
    int direction = (color == Color::WHITE) ? -1 : 1;
    
    // Move forward one square
    if (colDiff == 0 && rowDiff == direction && board.isEmpty(to)) {
        return true;
    }
    
    // Move forward two squares from starting position
    if (colDiff == 0 && !hasMoved && rowDiff == 2 * direction) {
        Position middle(position.getRow() + direction, position.getCol());
        if (board.isEmpty(middle) && board.isEmpty(to)) {
            return true;
        }
    }
    
    // Capture diagonally
    if (colDiff == 1 && rowDiff == direction) {
        if (!board.isEmpty(to) && board.getPiece(to)->getColor() != color) {
            return true;
        }
        // En passant
        if (board.isEnPassantAvailable() && to == board.getEnPassantTarget()) {
            return true;
        }
    }
    
    return false;
}

bool Rook::isValidMove(const Position& to, Board& board) const {
    if (position == to) return false;
    
    // Must move in straight line (horizontal or vertical)
    if (position.getRow() != to.getRow() && position.getCol() != to.getCol()) {
        return false;
    }
    
    // Check if path is clear
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Check destination
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

bool Knight::isValidMove(const Position& to, Board& board) const {
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // L-shape movement
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
        return false;
    }
    
    // Check destination
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

bool Bishop::isValidMove(const Position& to, Board& board) const {
    if (position == to) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Must move diagonally
    if (rowDiff != colDiff) {
        return false;
    }
    
    // Check if path is clear
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Check destination
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

bool Queen::isValidMove(const Position& to, Board& board) const {
    if (position == to) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Must move in straight line or diagonal
    if (rowDiff != colDiff && position.getRow() != to.getRow() && position.getCol() != to.getCol()) {
        return false;
    }
    
    // Check if path is clear
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Check destination
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

bool King::isValidMove(const Position& to, Board& board) const {
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Must move one square in any direction
    if (rowDiff > 1 || colDiff > 1) {
        return false;
    }
    
    if (rowDiff == 0 && colDiff == 0) {
        return false;
    }
    
    // Check destination
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}