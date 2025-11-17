/**
 * @file Board.h
 * @brief Defines the Board class for managing the chess board state
 *
 * @details This header file contains the Board class which represents the 8x8 chess board.
 * The board uses composition to store pieces in a 2D array of unique_ptr<Piece>, enabling
 * automatic memory management. The board tracks piece positions, validates moves, checks
 * for check conditions, and manages special move states like en passant. The class provides
 * methods for piece manipulation, path validation, attack detection, and game state queries.
 * Function overloading is used to provide multiple interfaces for position access (Position
 * objects vs. row/column indices).
 */

#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"
#include <memory>
#include <vector>

/**
 * @class Board
 * @brief Manages the chess board state and piece positions
 *
 * @details The Board class encapsulates the 8x8 chess board using a 2D array of
 * unique_ptr<Piece> pointers, demonstrating composition and automatic memory management.
 * The board maintains piece positions, validates movement paths, detects check conditions,
 * and manages special move states. The en passant mechanism is tracked through
 * enPassantTarget and enPassantAvailable flags. The class provides overloaded methods
 * for accessing positions using either Position objects or row/column indices, offering
 * flexibility in usage. Check detection works by finding the king's position and checking
 * if it's under attack by any opponent piece.
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
     * @details Initializes all squares to nullptr and sets en passant flags to false
     */
    Board();

    /**
     * @brief Default destructor
     * @details unique_ptr automatically handles piece cleanup
     */
    ~Board() = default;

    /**
     * @brief Initializes the board with starting chess position
     * @details Places all pieces in their standard starting positions:
     * white pieces on ranks 6-7, black pieces on ranks 0-1
     */
    void initialize();

    /**
     * @brief Displays the board in ASCII format
     * @details Prints the board with ranks and files labeled, showing
     * piece symbols (uppercase for white, lowercase for black)
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
     * @details Overloaded version using row/column indices
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
     * @details Overloaded version using row/column indices
     */
    bool isEmpty(int row, int col) const;

    /**
     * @brief Moves a piece from one position to another
     * @param from Source position
     * @param to Destination position
     * @return true if move was successful, false otherwise
     * @details Handles piece movement, including captures. The destination
     * square is cleared if occupied. Updates piece position and movement status.
     */
    bool movePiece(const Position &from, const Position &to);

    /**
     * @brief Moves a piece using row/column indices
     * @param fromRow Source row (0-7)
     * @param fromCol Source column (0-7)
     * @param toRow Destination row (0-7)
     * @param toCol Destination column (0-7)
     * @return true if move was successful, false otherwise
     * @details Overloaded version using row/column indices
     */
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);

    /**
     * @brief Places a piece at the specified position
     * @param pos Position to place piece
     * @param piece unique_ptr to the piece to place
     * @details Takes ownership of the piece. Any existing piece at the
     * position is replaced (and destroyed).
     */
    void setPiece(const Position &pos, std::unique_ptr<Piece> piece);

    /**
     * @brief Removes and returns a piece from the board
     * @param pos Position to remove piece from
     * @return unique_ptr to the removed piece, or nullptr if empty
     * @details Transfers ownership of the piece to the caller
     */
    std::unique_ptr<Piece> removePiece(const Position &pos);

    /**
     * @brief Checks if the path between two positions is clear
     * @param from Source position
     * @param to Destination position
     * @return true if path is clear (no pieces blocking), false otherwise
     * @details Only checks straight lines (horizontal, vertical, diagonal).
     * Does not validate if positions are on a straight line.
     */
    bool isPathClear(const Position &from, const Position &to) const;

    /**
     * @brief Checks if a position is under attack by pieces of specified color
     * @param pos Position to check
     * @param byColor Color of attacking pieces
     * @return true if position is under attack, false otherwise
     * @details Iterates through all pieces of the specified color and checks
     * if any can attack the target position.
     */
    bool isUnderAttack(const Position &pos, Color byColor) const;

    /**
     * @brief Finds the position of the king of specified color
     * @param color Color of the king to find
     * @return Position of the king
     * @details Searches the board for the king piece of the specified color
     */
    Position getKingPosition(Color color) const;

    /**
     * @brief Checks if the king of specified color is in check
     * @param color Color of the king to check
     * @return true if king is in check, false otherwise
     * @details Finds the king's position and checks if it's under attack
     * by any opponent piece.
     */
    bool isInCheck(Color color) const;

    /**
     * @brief Checks if a move would leave the king in check
     * @param from Source position
     * @param to Destination position
     * @param color Color of the player making the move
     * @return true if move would result in check, false otherwise
     * @details Temporarily performs the move, checks for check condition,
     * then restores the board state. Used to validate legal moves.
     */
    bool wouldBeInCheck(const Position &from, const Position &to, Color color);

    /**
     * @brief Sets the en passant target square
     * @param pos Position that can be captured via en passant
     * @details Marks the specified position as the en passant target and
     * enables the en passant flag. Called when a pawn moves two squares.
     */
    void setEnPassantTarget(const Position &pos)
    {
        enPassantTarget = pos;
        enPassantAvailable = true;
    }

    /**
     * @brief Clears the en passant target
     * @details Disables en passant availability. Should be called after
     * each move to clear the previous en passant opportunity.
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