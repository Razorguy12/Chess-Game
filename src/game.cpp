#include "Game.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <string>

void Game::start()
{
    std::cout << "=================================\n";
    std::cout << "    Welcome to CLI Chess Game    \n";
    std::cout << "=================================\n\n";

    // Get player names
    std::string whiteName, blackName;
    std::cout << "Enter name for White player: ";
    std::getline(std::cin, whiteName);
    if (whiteName.empty())
        whiteName = "White";

    std::cout << "Enter name for Black player: ";
    std::getline(std::cin, blackName);
    if (blackName.empty())
        blackName = "Black";

    // Set player names
    whitePlayer.setName(whiteName);
    blackPlayer.setName(blackName);

    std::cout << "\n"
              << whiteName << " (White) vs " << blackName << " (Black)\n";
    std::cout << "\nCommands:\n";
    std::cout << "  - Move: e2 e4\n";
    std::cout << "  - Castle Kingside: O-O or 0-0\n";
    std::cout << "  - Castle Queenside: O-O-O or 0-0-0\n";
    std::cout << "  - Quit: quit or exit\n\n";

    while (!gameOver)
    {
        try
        {
            playTurn();
        }
        catch (const std::exception &e)
        {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }

    std::cout << "\n=================================\n";
    std::cout << "         Game Over!\n";
    std::cout << "=================================\n";
    if (!winner.empty())
    {
        std::cout << "Winner: " << winner << "!\n";
    }
    else
    {
        std::cout << "Result: Draw!\n";
    }
    std::cout << "=================================\n";
}

void Game::playTurn()
{
    board.display();

    std::cout << currentPlayer->getName() << "'s turn";

    bool inCheck = board.isInCheck(currentPlayer->getColor());
    currentPlayer->setIsInCheck(inCheck);

    if (inCheck)
    {
        std::cout << " (in CHECK!)";
    }
    std::cout << "\nEnter move: ";

    std::string input1, input2;
    std::cin >> input1;

    if (input1 == "quit" || input1 == "exit" || input1 == "q")
    {
        // Handle quit/resignation properly
        std::cout << "\n"
                  << currentPlayer->getName() << ", do you want to:\n";
        std::cout << "1. Resign (opponent wins)\n";
        std::cout << "2. Offer draw (both players must agree)\n";
        std::cout << "3. Cancel and continue playing\n";
        std::cout << "Enter choice (1-3): ";

        std::string choice;
        std::cin >> choice;

        if (choice == "1")
        {
            // Current player resigns, opponent wins
            gameOver = true;
            Player *winnerPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
            winner = winnerPlayer->getName();
            std::cout << "\n"
                      << currentPlayer->getName() << " resigns. " << winner << " wins!\n";
        }
        else if (choice == "2")
        {
            // Offer draw
            std::cout << "\n"
                      << currentPlayer->getName() << " offers a draw.\n";
            Player *opponent = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
            std::cout << opponent->getName() << ", do you accept the draw? (y/n): ";

            std::string response;
            std::cin >> response;

            if (response == "y" || response == "Y" || response == "yes")
            {
                gameOver = true;
                std::cout << "\nDraw agreed by both players.\n";
            }
            else
            {
                std::cout << "\nDraw offer declined. Game continues.\n";
            }
        }
        else
        {
            std::cout << "\nContinuing game...\n";
        }
        return;
    }

    if (input1.length() == 4)
    {
        std::cout << "Invalid Format!!! try again" << std::endl;
        return;
    }

    // Check for castling
    if (input1 == "O-O" || input1 == "0-0" || input1 == "o-o")
    {
        handleCastling("kingside");
        return;
    }
    if (input1 == "O-O-O" || input1 == "0-0-0" || input1 == "o-o-o")
    {
        handleCastling("queenside");
        return;
    }

    Position check = parsePosition(input1);
    if (!check.isValid())
    {
        throw std::runtime_error("Invalid move!");
    }
    std::cin >> input2;

    if (!makeMove(input1, input2))
    {
        throw std::runtime_error("Invalid move!");
    }
}

bool Game::makeMove(const std::string &from, const std::string &to)
{
    Position fromPos = parsePosition(from);
    Position toPos = parsePosition(to);

    if (!fromPos.isValid() || !toPos.isValid())
    {
        return false;
    }

    Piece *piece = board.getPiece(fromPos);
    if (!piece)
    {
        throw std::runtime_error("No piece at that position!");
    }

    if (piece->getColor() != currentPlayer->getColor())
    {
        throw std::runtime_error("That's not your piece!");
    }

    if (!piece->isValidMove(toPos, board))
    {
        return false;
    }

    // Check if move would leave king in check
    if (board.wouldBeInCheck(fromPos, toPos, currentPlayer->getColor()))
    {
        throw std::runtime_error("Move would leave king in check!");
    }

    // Check for captured piece BEFORE moving
    Piece *capturedPiece = board.getPiece(toPos);
    if (capturedPiece && capturedPiece->getColor() != currentPlayer->getColor())
    {
        // Calculate piece value (simplified: Pawn=1, Knight/Bishop=3, Rook=5, Queen=9, King=0)
        int pieceValue = 0;
        std::string pieceName = capturedPiece->getName();
        if (pieceName == "Pawn")
            pieceValue = 1;
        else if (pieceName == "Knight" || pieceName == "Bishop")
            pieceValue = 3;
        else if (pieceName == "Rook")
            pieceValue = 5;
        else if (pieceName == "Queen")
            pieceValue = 9;

        currentPlayer->addCapturedPieceValue(pieceValue);
    }

    // Check for en passant BEFORE moving
    bool isEnPassant = SpecialMoves::isEnPassantMove(fromPos, toPos, board);

    // Check if pawn moves two squares (set en passant target)
    bool isPawnDoubleMove = false;
    if (piece->template isType<Pawn>())
    {
        int rowDiff = std::abs(toPos.getRow() - fromPos.getRow());
        if (rowDiff == 2)
        {
            isPawnDoubleMove = true;
        }
    }

    // Clear en passant flag
    board.clearEnPassant();

    // Perform the move
    if (isEnPassant)
    {
        SpecialMoves::performEnPassant(fromPos, toPos, board);
    }
    else
    {
        if (!board.movePiece(fromPos, toPos))
        {
            return false;
        }
    }

    // Set en passant target if pawn moved two squares
    if (isPawnDoubleMove)
    {
        int midRow = (fromPos.getRow() + toPos.getRow()) / 2;
        board.setEnPassantTarget(Position(midRow, fromPos.getCol()));
    }

    // Check for pawn promotion
    piece = board.getPiece(toPos);
    if (piece && piece->template isType<Pawn>())
    {
        if ((piece->getColor() == Color::WHITE && toPos.getRow() == 0) ||
            (piece->getColor() == Color::BLACK && toPos.getRow() == 7))
        {
            handlePromotion(toPos);
        }
    }

    switchPlayer();
    checkGameStatus();

    return true;
}

Position Game::parsePosition(const std::string &pos)
{
    if (pos.length() != 2)
        return Position(-1, -1);

    char col = std::tolower(pos[0]);
    char row = pos[1];

    if (col < 'a' || col > 'h' || row < '1' || row > '8')
    {
        return Position(-1, -1);
    }

    return Position(8 - (row - '0'), col - 'a');
}

void Game::handlePromotion(const Position &pos)
{
    std::cout << "Pawn promotion! Choose piece (Q/R/B/N): ";
    char choice;
    std::cin >> choice;

    SpecialMoves::promotePawn(pos, choice, board);
}

void Game::handleCastling(const std::string &command)
{
    bool kingSide = (command == "kingside");

    if (kingSide)
    {
        if (!SpecialMoves::canCastleKingSide(currentPlayer->getColor(), board))
        {
            throw std::runtime_error("Cannot castle kingside!");
        }
        SpecialMoves::performCastling(currentPlayer->getColor(), true, board);
    }
    else
    {
        if (!SpecialMoves::canCastleQueenSide(currentPlayer->getColor(), board))
        {
            throw std::runtime_error("Cannot castle queenside!");
        }
        SpecialMoves::performCastling(currentPlayer->getColor(), false, board);
    }

    board.clearEnPassant();
    switchPlayer();
    checkGameStatus();
}

bool Game::hasValidMoves(Color color)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece *piece = board.getPiece(i, j);
            if (piece && piece->getColor() == color)
            {
                Position from(i, j);

                for (int ti = 0; ti < 8; ti++)
                {
                    for (int tj = 0; tj < 8; tj++)
                    {
                        Position to(ti, tj);
                        if (piece->isValidMove(to, board))
                        {
                            if (!board.wouldBeInCheck(from, to, color))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void Game::checkGameStatus()
{
    bool inCheck = board.isInCheck(currentPlayer->getColor());
    currentPlayer->setIsInCheck(inCheck);
    bool hasLegalMoves = hasValidMoves(currentPlayer->getColor());

    if (!hasLegalMoves)
    {
        gameOver = true;
        if (inCheck)
        {
            // The other player wins
            Player *winnerPlayer = (currentPlayer == &whitePlayer) ? &blackPlayer : &whitePlayer;
            winner = winnerPlayer->getName();
            std::cout << "\nCheckmate! " << currentPlayer->getName() << " is in checkmate.\n";
            std::cout << winner << " wins the game!\n";
        }
        else
        {
            std::cout << "\nStalemate! " << currentPlayer->getName() << " has no legal moves.\n";
            std::cout << "The game is a draw!\n";
        }
    }
}