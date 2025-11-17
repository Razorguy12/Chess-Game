#include "SpecialMoves.h"

bool SpecialMoves::canCastleKingSide(Color color, Board &board)
{
    int row = (color == Color::WHITE) ? 7 : 0;

    // Check if king and rook haven't moved
    Piece *king = board.getPiece(row, 4);
    Piece *rook = board.getPiece(row, 7);

    if (!king || !rook)
        return false;
    if (king->hasMovedBefore() || rook->hasMovedBefore())
        return false;
    if (!king->template isType<King>() || !rook->template isType<Rook>())
        return false;

    // Check if squares between are empty
    if (!board.isEmpty(row, 5) || !board.isEmpty(row, 6))
        return false;

    // Check if king is in check or passes through check
    Color enemyColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (board.isUnderAttack(Position(row, 4), enemyColor))
        return false;
    if (board.isUnderAttack(Position(row, 5), enemyColor))
        return false;
    if (board.isUnderAttack(Position(row, 6), enemyColor))
        return false;

    return true;
}

bool SpecialMoves::canCastleQueenSide(Color color, Board &board)
{
    int row = (color == Color::WHITE) ? 7 : 0;

    // Check if king and rook haven't moved
    Piece *king = board.getPiece(row, 4);
    Piece *rook = board.getPiece(row, 0);

    if (!king || !rook)
        return false;
    if (king->hasMovedBefore() || rook->hasMovedBefore())
        return false;
    if (!king->template isType<King>() || !rook->template isType<Rook>())
        return false;

    // Check if squares between are empty
    if (!board.isEmpty(row, 1) || !board.isEmpty(row, 2) || !board.isEmpty(row, 3))
        return false;

    // Check if king is in check or passes through check
    Color enemyColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (board.isUnderAttack(Position(row, 4), enemyColor))
        return false;
    if (board.isUnderAttack(Position(row, 3), enemyColor))
        return false;
    if (board.isUnderAttack(Position(row, 2), enemyColor))
        return false;

    return true;
}

void SpecialMoves::performCastling(Color color, bool kingSide, Board &board)
{
    int row = (color == Color::WHITE) ? 7 : 0;

    if (kingSide)
    {
        // Move king
        auto king = board.removePiece(Position(row, 4));
        king->setPosition(Position(row, 6));
        board.setPiece(Position(row, 6), std::move(king));

        // Move rook
        auto rook = board.removePiece(Position(row, 7));
        rook->setPosition(Position(row, 5));
        board.setPiece(Position(row, 5), std::move(rook));
    }
    else
    {
        // Move king
        auto king = board.removePiece(Position(row, 4));
        king->setPosition(Position(row, 2));
        board.setPiece(Position(row, 2), std::move(king));

        // Move rook
        auto rook = board.removePiece(Position(row, 0));
        rook->setPosition(Position(row, 3));
        board.setPiece(Position(row, 3), std::move(rook));
    }
}

void SpecialMoves::promotePawn(const Position &pos, char choice, Board &board)
{
    Piece *piece = board.getPiece(pos);
    if (!piece || !piece->template isType<Pawn>())
        return;

    Color color = piece->getColor();
    auto promoted = board.removePiece(pos);

    switch (choice)
    {
    case 'Q':
    case 'q':
        board.setPiece(pos, std::make_unique<Queen>(color, pos));
        break;
    case 'R':
    case 'r':
        board.setPiece(pos, std::make_unique<Rook>(color, pos));
        break;
    case 'B':
    case 'b':
        board.setPiece(pos, std::make_unique<Bishop>(color, pos));
        break;
    case 'N':
    case 'n':
        board.setPiece(pos, std::make_unique<Knight>(color, pos));
        break;
    default:
        board.setPiece(pos, std::make_unique<Queen>(color, pos));
        break;
    }
}

bool SpecialMoves::isEnPassantMove(const Position &from, const Position &to, Board &board)
{
    Piece *piece = board.getPiece(from);
    if (!piece || !piece->template isType<Pawn>())
        return false;

    if (!board.isEnPassantAvailable())
        return false;
    if (to != board.getEnPassantTarget())
        return false;

    return true;
}

void SpecialMoves::performEnPassant(const Position &from, const Position &to, Board &board)
{
    auto pawn = board.removePiece(from);
    Color color = pawn->getColor();

    // Remove the captured pawn
    int capturedRow = (color == Color::WHITE) ? to.getRow() + 1 : to.getRow() - 1;
    board.removePiece(Position(capturedRow, to.getCol()));

    // Move the pawn
    pawn->setPosition(to);
    board.setPiece(to, std::move(pawn));
}