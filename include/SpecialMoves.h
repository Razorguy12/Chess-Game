#ifndef SPECIALMOVES_H
#define SPECIALMOVES_H

#include "Board.h"

class SpecialMoves {
public:
    // Castling
    static bool canCastleKingSide(Color color, Board& board);
    static bool canCastleQueenSide(Color color, Board& board);
    static void performCastling(Color color, bool kingSide, Board& board);
    
    // Promotion
    static void promotePawn(const Position& pos, char choice, Board& board);
    
    // En Passant
    static bool isEnPassantMove(const Position& from, const Position& to, Board& board);
    static void performEnPassant(const Position& from, const Position& to, Board& board);
};

#endif