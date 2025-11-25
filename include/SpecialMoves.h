#ifndef SPECIALMOVES_H
#define SPECIALMOVES_H

#include "Board.h"

/**
 * @class SpecialMoves
 * @brief Utility class for handling special chess moves
 */
class SpecialMoves
{
public:
    /**
     * @brief Checks if kingside castling is legal for the specified color
     * @param color Color of the player attempting to castle
     * @param board Reference to the game board
     * @return true if castling is legal, false otherwise
     */
    static bool canCastleKingSide(Color color, Board &board);

    /**
     * @brief Checks if queenside castling is legal for the specified color
     * @param color Color of the player attempting to castle
     * @param board Reference to the game board
     * @return true if castling is legal, false otherwise
     */
    static bool canCastleQueenSide(Color color, Board &board);

    /**
     * @brief Performs castling for the specified color
     * @param color Color of the player castling
     * @param kingSide true for kingside castling, false for queenside
     * @param board Reference to the game board
     */
    static void performCastling(Color color, bool kingSide, Board &board);

    /**
     * @brief Promotes a pawn to the selected piece type
     * @param pos Position of the pawn to promote
     * @param choice Character representing the desired piece ('Q', 'R', 'B', 'N')
     * @param board Reference to the game board
     */
    static void promotePawn(const Position &pos, char choice, Board &board);

    /**
     * @brief Checks if a move is a valid en passant capture
     * @param from Source position of the moving pawn
     * @param to Destination position of the moving pawn
     * @param board Reference to the game board
     * @return true if the move is a valid en passant capture, false otherwise
     */
    static bool isEnPassantMove(const Position &from, const Position &to, Board &board);

    /**
     * @brief Performs an en passant capture
     * @param from Source position of the capturing pawn
     * @param to Destination position (en passant target square)
     * @param board Reference to the game board
     */
    static void performEnPassant(const Position &from, const Position &to, Board &board);
};

#endif