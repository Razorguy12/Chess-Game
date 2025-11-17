#include "Board.h"
#include <iostream>

Board::Board() : enPassantAvailable(false)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            squares[i][j] = nullptr;
        }
    }
}

void Board::initialize()
{
    // Place black pieces
    squares[0][0] = std::make_unique<Rook>(Color::BLACK, Position(0, 0));
    squares[0][1] = std::make_unique<Knight>(Color::BLACK, Position(0, 1));
    squares[0][2] = std::make_unique<Bishop>(Color::BLACK, Position(0, 2));
    squares[0][3] = std::make_unique<Queen>(Color::BLACK, Position(0, 3));
    squares[0][4] = std::make_unique<King>(Color::BLACK, Position(0, 4));
    squares[0][5] = std::make_unique<Bishop>(Color::BLACK, Position(0, 5));
    squares[0][6] = std::make_unique<Knight>(Color::BLACK, Position(0, 6));
    squares[0][7] = std::make_unique<Rook>(Color::BLACK, Position(0, 7));

    for (int i = 0; i < 8; i++)
    {
        squares[1][i] = std::make_unique<Pawn>(Color::BLACK, Position(1, i));
    }

    // Place white pieces
    for (int i = 0; i < 8; i++)
    {
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

void Board::display() const
{
    std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < 8; i++)
    {
        std::cout << (8 - i) << " |";
        for (int j = 0; j < 8; j++)
        {
            if (squares[i][j])
            {
                std::cout << " " << squares[i][j]->getSymbol() << " |";
            }
            else
            {
                std::cout << "   |";
            }
        }
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "    a   b   c   d   e   f   g   h\n\n";
}

Piece *Board::getPiece(const Position &pos) const
{
    if (!pos.isValid())
        return nullptr;
    return squares[pos.getRow()][pos.getCol()].get();
}

Piece *Board::getPiece(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return nullptr;
    return squares[row][col].get();
}

bool Board::isEmpty(const Position &pos) const
{
    if (!pos.isValid())
        return false;
    return squares[pos.getRow()][pos.getCol()] == nullptr;
}

bool Board::isEmpty(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
        return true;
    return squares[row][col] == nullptr;
}

bool Board::movePiece(const Position &from, const Position &to)
{
    if (!from.isValid() || !to.isValid())
        return false;
    if (isEmpty(from))
        return false;

    // Remove destination piece if any (capture)
    if (!isEmpty(to))
    {
        removePiece(to);
    }

    // Move the piece
    std::unique_ptr<Piece> movingPiece = removePiece(from);
    if (movingPiece)
    {
        movingPiece->setPosition(to);
        setPiece(to, std::move(movingPiece));
        return true;
    }

    return false;
}

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    return movePiece(Position(fromRow, fromCol), Position(toRow, toCol));
}

void Board::setPiece(const Position &pos, std::unique_ptr<Piece> piece)
{
    if (pos.isValid())
    {
        squares[pos.getRow()][pos.getCol()] = std::move(piece);
    }
}

std::unique_ptr<Piece> Board::removePiece(const Position &pos)
{
    if (!pos.isValid())
        return nullptr;
    return std::move(squares[pos.getRow()][pos.getCol()]);
}

bool Board::isPathClear(const Position &from, const Position &to) const
{
    int rowDir = 0, colDir = 0;

    if (to.getRow() > from.getRow())
        rowDir = 1;
    else if (to.getRow() < from.getRow())
        rowDir = -1;

    if (to.getCol() > from.getCol())
        colDir = 1;
    else if (to.getCol() < from.getCol())
        colDir = -1;

    int currentRow = from.getRow() + rowDir;
    int currentCol = from.getCol() + colDir;

    while (currentRow != to.getRow() || currentCol != to.getCol())
    {
        if (!isEmpty(currentRow, currentCol))
        {
            return false;
        }
        currentRow += rowDir;
        currentCol += colDir;
    }

    return true;
}

bool Board::isUnderAttack(const Position &pos, Color byColor) const
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece *piece = getPiece(i, j);
            if (piece && piece->getColor() == byColor)
            {
                if (piece->isValidMove(pos, *this))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

Position Board::getKingPosition(Color color) const
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece *piece = getPiece(i, j);
            if (piece && piece->getColor() == color && piece->template isType<King>())
            {
                return Position(i, j);
            }
        }
    }
    return Position(-1, -1);
}

bool Board::isInCheck(Color color) const
{
    Position kingPos = getKingPosition(color);
    if (!kingPos.isValid())
        return false;

    Color enemyColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isUnderAttack(kingPos, enemyColor);
}

bool Board::wouldBeInCheck(const Position &from, const Position &to, Color color)
{
    if (!from.isValid() || !to.isValid())
        return true;
    if (isEmpty(from))
        return true;

    // Save state
    Piece *movingPiece = getPiece(from);
    bool movingPieceHadMoved = movingPiece->hasMovedBefore();

    // Simulate the move
    std::unique_ptr<Piece> tempMoving = const_cast<Board *>(this)->removePiece(from);
    std::unique_ptr<Piece> tempCaptured = const_cast<Board *>(this)->removePiece(to);

    tempMoving->setPosition(to);
    const_cast<Board *>(this)->setPiece(to, std::move(tempMoving));

    bool inCheck = isInCheck(color);

    // Undo the move
    std::unique_ptr<Piece> restore = const_cast<Board *>(this)->removePiece(to);
    restore->setPosition(from);
    restore->setHasMoved(movingPieceHadMoved);
    const_cast<Board *>(this)->setPiece(from, std::move(restore));

    if (tempCaptured)
    {
        const_cast<Board *>(this)->setPiece(to, std::move(tempCaptured));
    }

    return inCheck;
}