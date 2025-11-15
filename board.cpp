/**
 * @file Board.cpp
 * @brief Implementation of the Board class which manages chess pieces,
 *        movement, and game state on an 8x8 chessboard.
 */

#include "Board.h"
#include <iostream>

/**
 * @brief Constructs an empty chessboard and initializes internal flags.
 * 
 * All squares are initialized to nullptr, indicating no pieces are present.
 */
Board::Board() : enPassantAvailable(false) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            squares[i][j] = nullptr;
        }
    }
}

/**
 * @brief Initializes the board with all pieces in standard chess starting positions.
 */
void Board::initialize() {
    // Place black pieces
    squares[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
    squares[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
    squares[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
    squares[0][3] = std::make_unique<Queen>(Color::BLACK, Position(0, 3));
    squares[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
    squares[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
    squares[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
    squares[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));
    
    for (int i = 0; i < 8; i++) {
        squares[1][i] = std::make_unique<Pawn>(Color::BLACK, Position(1, i));
    }
    
    // Place white pieces
    for (int i = 0; i < 8; i++) {
        squares[6][i] = std::make_unique<Pawn>(Color::WHITE, Position(6, i));
    }
    
    squares[7][0] = std::make_unique<Rook>(Color::WHITE, Position(7, 0));
    squares[7][1] = std::make_unique<Knight>(Color::WHITE, Position(7, 1));
    squares[7][2] = std::make_unique<Bishop>(Color::WHITE, Position(7, 2));
    squares[7][3] = std::make_unique<Queen>(Color::WHITE, Position(7, 3));
    squares[7][4] = std::make_unique<King>(Color::WHITE, Position(7, 4));
    squares[7][5] = std::make_unique<Bishop>(Color::WHITE, Position(7, 5));
    squares[7][6] = std::make_unique<Knight>(Color::WHITE, Position(7, 6));
    squares[7][7] = std::make_unique<Rook>(Color::WHITE, Position(7, 7));
}

/**
 * @brief Displays the board in ASCII format to the console.
 */
void Board::display() const {
    std::cout << "\n  a b c d e f g h\n";
    for (int i = 0; i < 8; i++) {
        std::cout << (8 - i) << " ";
        for (int j = 0; j < 8; j++) {
            if (squares[i][j]) {
                std::cout << squares[i][j]->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << (8 - i) << "\n";
    }
    std::cout << "  a b c d e f g h\n\n";
}

/**
 * @brief Retrieves the piece at a given position.
 * 
 * @param pos The target board position.
 * @return Pointer to the piece if present, nullptr otherwise.
 */
Piece* Board::getPiece(const Position& pos) const {
    if (!pos.isValid()) return nullptr;
    return squares[pos.getRow()][pos.getCol()].get();
}

/**
 * @brief Retrieves the piece at a given row and column.
 * 
 * @param row Row index (0–7)
 * @param col Column index (0–7)
 * @return Pointer to the piece or nullptr.
 */
Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return nullptr;
    return squares[row][col].get();
}

/**
 * @brief Checks whether a given position contains no piece.
 * 
 * @param pos Target position.
 * @return true if empty, false if occupied or invalid.
 */
bool Board::isEmpty(const Position& pos) const {
    if (!pos.isValid()) return false;
    return squares[pos.getRow()][pos.getCol()] == nullptr;
}

/**
 * @brief Checks whether the given row and column contain no piece.
 * 
 * @param row Row index.
 * @param col Column index.
 * @return true if empty or invalid, false otherwise.
 */
bool Board::isEmpty(int row, int col) const {
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return true;
    return squares[row][col] == nullptr;
}

/**
 * @brief Moves a piece from one position to another.
 * 
 * Captures the piece at the destination if present.
 * 
 * @param from Starting position.
 * @param to Target position.
 * @return true if movement succeeded, false otherwise.
 */
bool Board::movePiece(const Position& from, const Position& to) {
    if (!from.isValid() || !to.isValid()) return false;
    if (isEmpty(from)) return false;
    
    // Handle capture
    if (!isEmpty(to)) {
        removePiece(to);
    }
    
    // Move piece
    std::unique_ptr<Piece> movingPiece = removePiece(from);
    if (movingPiece) {
        movingPiece->setPosition(to);
        setPiece(to, std::move(movingPiece));
        return true;
    }
    
    return false;
}

/**
 * @brief Overload of movePiece using row/column integers.
 */
bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    return movePiece(Position(fromRow, fromCol), Position(toRow, toCol));
}

/**
 * @brief Places a piece at a specified position.
 * 
 * @param pos Target position.
 * @param piece Unique pointer to the piece (owners*
**/