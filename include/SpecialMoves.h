#ifndef SPECIALMOVES_H
#define SPECIALMOVES_H

#include "Board.h"

/**
 * @class SpecialMoves
 * @brief Utility class for handling special chess moves
 *
 * @details The SpecialMoves class provides static methods for three types of special
 * moves in chess: castling, pawn promotion, and en passant. Castling allows the king
 * and rook to move together under specific conditions. Pawn promotion occurs when
 * a pawn reaches the opposite end of the board. En passant is a special pawn capture
 * that can occur immediately after an opponent's pawn moves two squares. All methods
 * are static as they operate on the board state without requiring instance data.
 */
class SpecialMoves
{
public:
    /**
     * @brief Checks if kingside castling is legal for the specified color
     * @param color Color of the player attempting to castle
     * @param board Reference to the game board
     * @return true if castling is legal, false otherwise
     * @details Validates that: king and rook haven't moved, squares between are empty,
     * king is not in check, and squares king moves through are not under attack.
     * Kingside castling moves the king two squares right and the rook to the square
     * adjacent to the king on the left.
     */
    static bool canCastleKingSide(Color color, Board &board);

    /**
     * @brief Checks if queenside castling is legal for the specified color
     * @param color Color of the player attempting to castle
     * @param board Reference to the game board
     * @return true if castling is legal, false otherwise
     * @details Validates that: king and rook haven't moved, squares between are empty,
     * king is not in check, and squares king moves through are not under attack.
     * Queenside castling moves the king two squares left and the rook to the square
     * adjacent to the king on the right.
     */
    static bool canCastleQueenSide(Color color, Board &board);

    /**
     * @brief Performs castling for the specified color
     * @param color Color of the player castling
     * @param kingSide true for kingside castling, false for queenside
     * @param board Reference to the game board
     * @details Moves both the king and the appropriate rook to their castling positions.
     * The king moves two squares toward the rook, and the rook moves to the square
     * adjacent to the king on the opposite side. Both pieces are marked as having moved.
     */
    static void performCastling(Color color, bool kingSide, Board &board);

    /**
     * @brief Promotes a pawn to the selected piece type
     * @param pos Position of the pawn to promote
     * @param choice Character representing the desired piece ('Q', 'R', 'B', 'N')
     * @param board Reference to the game board
     * @details Replaces the pawn at the specified position with the chosen piece
     * (Queen, Rook, Bishop, or Knight) of the same color. The new piece is created
     * and placed on the board, and the pawn is removed.
     */
    static void promotePawn(const Position &pos, char choice, Board &board);

    /**
     * @brief Checks if a move is a valid en passant capture
     * @param from Source position of the moving pawn
     * @param to Destination position of the moving pawn
     * @param board Reference to the game board
     * @return true if the move is a valid en passant capture, false otherwise
     * @details Validates that: the moving piece is a pawn, the destination is the
     * en passant target square, and an opponent's pawn is in the correct position
     * to be captured. En passant can only occur immediately after an opponent's
     * pawn moves two squares forward.
     */
    static bool isEnPassantMove(const Position &from, const Position &to, Board &board);

    /**
     * @brief Performs an en passant capture
     * @param from Source position of the capturing pawn
     * @param to Destination position (en passant target square)
     * @param board Reference to the game board
     * @details Moves the capturing pawn to the en passant target square and removes
     * the opponent's pawn that is diagonally adjacent. The captured pawn is on the
     * same row as the source position but in the column of the destination.
     */
    static void performEnPassant(const Position &from, const Position &to, Board &board);
};

#endif