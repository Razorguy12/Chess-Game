/**
 * @file Pieces.cpp
 * @brief Implements the movement rules for all chess piece types:
 *        Pawn, Rook, Knight, Bishop, Queen, and King.
 */

#include "Pieces.h"
#include "Board.h"
#include <cmath>

/**
 * @brief Validates a pawn move.
 *
 * Pawns move differently depending on color. This function checks:
 * - One-step forward movement
 * - Two-step movement from starting rank
 * - Diagonal captures
 * - En passant captures
 *
 * @param to The target square.
 * @param board Reference to the board for checking occupancy.
 * @return true If the move is legal.
 * @return false Otherwise.
 */
bool Pawn::isValidMove(const Position& to, Board& board) const {
    int rowDiff = to.getRow() - position.getRow();
    int colDiff = std::abs(to.getCol() - position.getCol());
    int direction = (color == Color::WHITE) ? -1 : 1;
    
    // Forward single-square move
    if (colDiff == 0 && rowDiff == direction && board.isEmpty(to)) {
        return true;
    }
    
    // Forward two-square move from starting position
    if (colDiff == 0 && !hasMoved && rowDiff == 2 * direction) {
        Position middle(position.getRow() + direction, position.getCol());
        if (board.isEmpty(middle) && board.isEmpty(to)) {
            return true;
        }
    }
    
    // Diagonal capture
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

/**
 * @brief Validates a rook move.
 *
 * Rooks move horizontally or vertically any number of squares.
 * This function verifies:
 * - Straight-line movement
 * - Path clearance
 * - Destination not occupied by own color
 *
 * @param to The target square.
 * @param board Reference to the board.
 * @return true If the move is legal.
 */
bool Rook::isValidMove(const Position& to, Board& board) const {
    if (position == to) return false;
    
    // Must move in straight line
    if (position.getRow() != to.getRow() && position.getCol() != to.getCol()) {
        return false;
    }
    
    // Path must be unobstructed
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Cannot capture own piece
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

/**
 * @brief Validates a knight move.
 *
 * Knights move in an L-shape and jump over pieces.
 * No path checking is required.
 *
 * @param to The target square.
 * @param board Reference to the board.
 * @return true If the move is legal.
 */
bool Knight::isValidMove(const Position& to, Board& board) const {
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Must move in valid knight pattern
    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))) {
        return false;
    }
    
    // Cannot capture own piece
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

/**
 * @brief Validates a bishop move.
 *
 * Bishops move diagonally any number of squares.
 * This function checks:
 * - Diagonal travel
 * - Clear path
 * - Destination not occupied by same color
 *
 * @param to The target square.
 * @param board Reference to the board.
 * @return true If the move is legal.
 */
bool Bishop::isValidMove(const Position& to, Board& board) const {
    if (position == to) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Must move diagonally
    if (rowDiff != colDiff) {
        return false;
    }
    
    // Check path
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Cannot capture own piece
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

/**
 * @brief Validates a queen move.
 *
 * The queen combines rook and bishop movement:
 * - Straight lines OR diagonals
 * - Path must be clear
 * - Cannot capture own piece
 *
 * @param to The target square.
 * @param board Reference to the board.
 * @return true If the move is legal.
 */
bool Queen::isValidMove(const Position& to, Board& board) const {
    if (position == to) return false;
    
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // Must move like rook or bishop
    bool straight = (position.getRow() == to.getRow() || position.getCol() == to.getCol());
    bool diagonal = (rowDiff == colDiff);
    
    if (!straight && !diagonal) {
        return false;
    }
    
    // Path must be clear
    if (!board.isPathClear(position, to)) {
        return false;
    }
    
    // Cannot capture own piece
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}

/**
 * @brief Validates a king move (excluding castling).
 *
 * Kings move one square in any direction.
 * Castling is handled separately in SpecialMoves.
 *
 * @param to The target square.
 * @param board Reference to the board.
 * @return true If the move is legal.
 */
bool King::isValidMove(const Position& to, Board& board) const {
    int rowDiff = std::abs(to.getRow() - position.getRow());
    int colDiff = std::abs(to.getCol() - position.getCol());
    
    // King moves only one square
    if (rowDiff > 1 || colDiff > 1) {
        return false;
    }
    
    // Can't stay in place
    if (rowDiff == 0 && colDiff == 0) {
        return false;
    }
    
    // Cannot capture own color
    if (!board.isEmpty(to) && board.getPiece(to)->getColor() == color) {
        return false;
    }
    
    return true;
}
