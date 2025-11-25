#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"
#include <memory>
#include <vector>

/**
 * @class Board
 * @brief Manages the chess board state and piece positions
 */
class Board
{
private:
    // Composition: Board contains pieces (Dynamic memory)
    std::unique_ptr<Piece> squares[8][8];
    Position enPassantTarget;
    bool enPassantAvailable;

public:
    /**
     * @brief Constructs an empty Board
     */
    Board();

    /**
     * @brief Default destructor
     */
    ~Board() = default;

    /**
     * @brief Initializes the board with starting chess position
     */
    void initialize();

    /**
     * @brief Displays the board in ASCII format
     */
    void display() const;

    /**
     * @brief Gets piece at specified position
     * @param pos Position to query
     * @return Pointer to piece, or nullptr if square is empty
     */
    Piece *getPiece(const Position &pos) const;

    /**
     * @brief Gets piece at specified row and column
     * @param row Row index (0-7)
     * @param col Column index (0-7)
     * @return Pointer to piece, or nullptr if square is empty
     */
    Piece *getPiece(int row, int col) const;

    /**
     * @brief Checks if a position is empty
     * @param pos Position to check
     * @return true if square is empty, false otherwise
     */
    bool isEmpty(const Position &pos) const;

    /**
     * @brief Checks if a position is empty
     * @param row Row index (0-7)
     * @param col Column index (0-7)
     * @return true if square is empty, false otherwise
     */
    bool isEmpty(int row, int col) const;

    /**
     * @brief Moves a piece from one position to another
     * @param from Source position
     * @param to Destination position
     * @return true if move was successful, false otherwise
     */
    bool movePiece(const Position &from, const Position &to);

    /**
     * @brief Moves a piece using row/column indices
     * @param fromRow Source row (0-7)
     * @param fromCol Source column (0-7)
     * @param toRow Destination row (0-7)
     * @param toCol Destination column (0-7)
     * @return true if move was successful, false otherwise
     */
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    /**
     * @brief Places a piece at the specified position
     * @param pos Position to place piece
     * @param piece unique_ptr to the piece to place
     */
    void setPiece(const Position &pos, std::unique_ptr<Piece> piece);

    /**
     * @brief Removes and returns a piece from the board
     * @param pos Position to remove piece from
     * @return unique_ptr to the removed piece, or nullptr if empty
     */
    std::unique_ptr<Piece> removePiece(const Position &pos);

    /**
     * @brief Checks if the path between two positions is clear
     * @param from Source position
     * @param to Destination position
     * @return true if path is clear (no pieces blocking), false otherwise
     */
    bool isPathClear(const Position &from, const Position &to) const;

    /**
     * @brief Checks if a position is under attack by pieces of specified color
     * @param pos Position to check
     * @param byColor Color of attacking pieces
     * @return true if position is under attack, false otherwise
     */
    bool isUnderAttack(const Position &pos, Color byColor) const;

    /**
     * @brief Finds the position of the king of specified color
     * @param color Color of the king to find
     * @return Position of the king
     */
    Position getKingPosition(Color color) const;

    /**
     * @brief Checks if the king of specified color is in check
     * @param color Color of the king to check
     * @return true if king is in check, false otherwise
     */
    bool isInCheck(Color color) const;

    /**
     * @brief Checks if a move would leave the king in check
     * @param from Source position
     * @param to Destination position
     * @param color Color of the player making the move
     * @return true if move would result in check, false otherwise
     */
    bool wouldBeInCheck(const Position &from, const Position &to, Color color);

    /**
     * @brief Sets the en passant target square
     * @param pos Position that can be captured via en passant
     */
    void setEnPassantTarget(const Position &pos)
    {
        enPassantTarget = pos;
        enPassantAvailable = true;
    }

    /**
     * @brief Clears the en passant target
     */
    void clearEnPassant() { enPassantAvailable = false; }

    /**
     * @brief Checks if en passant is currently available
     * @return true if en passant target is set, false otherwise
     */
    bool isEnPassantAvailable() const { return enPassantAvailable; }

    /**
     * @brief Gets the current en passant target position
     * @return Position of the en passant target square
     */
    Position getEnPassantTarget() const { return enPassantTarget; }
};

#endif