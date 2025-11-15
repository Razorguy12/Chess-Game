#include "Game.h"
#include <iostream>
#include <cctype>
#include <algorithm>

void Game::start() {
    std::cout << "=================================\n";
    std::cout << "    Welcome to CLI Chess Game    \n";
    std::cout << "=================================\n\n";
    std::cout << "Commands:\n";
    std::cout << "  - Move: e2 e4\n";
    std::cout << "  - Castle Kingside: O-O or 0-0\n";
    std::cout << "  - Castle Queenside: O-O-O or 0-0-0\n";
    std::cout << "  - Quit: quit or exit\n\n";
    
    while (!gameOver) {
        try {
            playTurn();
        } catch (const ChessException& e) {
            std::cout << "Error: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }
    
    std::cout << "\nGame Over! ";
    if (!winner.empty()) {
        std::cout << winner << " wins!\n";
    } else {
        std::cout << "Draw!\n";
    }
}

void Game::playTurn() {
    board.display();
    
    std::string colorName = (currentPlayer == Color::WHITE) ? "White" : "Black";
    std::cout << colorName << "'s turn";
    
    if (board.isInCheck(currentPlayer)) {
        std::cout << " (in CHECK!)";
    }
    std::cout << "\nEnter move: ";
    
    std::string input1, input2;
    std::cin >> input1;
    
    if (input1 == "quit" || input1 == "exit") {
        gameOver = true;
        return;
    }
    
    // Check for castling
    if (input1 == "O-O" || input1 == "0-0" || input1 == "o-o") {
        handleCastling("kingside");
        return;
    }
    if (input1 == "O-O-O" || input1 == "0-0-0" || input1 == "o-o-o") {
        handleCastling("queenside");
        return;
    }
    
    std::cin >> input2;
    
    if (!makeMove(input1, input2)) {
        throw ChessException("Invalid move!");
    }
}

bool Game::makeMove(const std::string& from, const std::string& to) {
    Position fromPos = parsePosition(from);
    Position toPos = parsePosition(to);
    
    if (!fromPos.isValid() || !toPos.isValid()) {
        return false;
    }
    
    Piece* piece = board.getPiece(fromPos);
    if (!piece) {
        throw ChessException("No piece at that position!");
    }
    
    if (piece->getColor() != currentPlayer) {
        throw ChessException("That's not your piece!");
    }
    
    if (!piece->isValidMove(toPos, board)) {
        return false;
    }
    
    // Check if move would leave king in check
    if (board.wouldBeInCheck(fromPos, toPos, currentPlayer)) {
        throw ChessException("Move would leave king in check!");
    }
    
    // Check for en passant BEFORE moving
    bool isEnPassant = SpecialMoves::isEnPassantMove(fromPos, toPos, board);
    
    // Check if pawn moves two squares (set en passant target)
    bool isPawnDoubleMove = false;
    if (piece->template isType<Pawn>()) {
        int rowDiff = std::abs(toPos.getRow() - fromPos.getRow());
        if (rowDiff == 2) {
            isPawnDoubleMove = true;
        }
    }
    
    // Clear en passant flag
    board.clearEnPassant();
    
    // Perform the move
    if (isEnPassant) {
        SpecialMoves::performEnPassant(fromPos, toPos, board);
    } else {
        if (!board.movePiece(fromPos, toPos)) {
            return false;
        }
    }
    
    // Set en passant target if pawn moved two squares
    if (isPawnDoubleMove) {
        int midRow = (fromPos.getRow() + toPos.getRow()) / 2;
        board.setEnPassantTarget(Position(midRow, fromPos.getCol()));
    }
    
    // Check for pawn promotion
    piece = board.getPiece(toPos);
    if (piece && piece->template isType<Pawn>()) {
        if ((piece->getColor() == Color::WHITE && toPos.getRow() == 0) ||
            (piece->getColor() == Color::BLACK && toPos.getRow() == 7)) {
            handlePromotion(toPos);
        }
    }
    
    switchPlayer();
    checkGameStatus();
    
    return true;
}

Position Game::parsePosition(const std::string& pos) {
    if (pos.length() != 2) return Position(-1, -1);
    
    char col = std::tolower(pos[0]);
    char row = pos[1];
    
    if (col < 'a' || col > 'h' || row < '1' || row > '8') {
        return Position(-1, -1);
    }
    
    return Position(8 - (row - '0'), col - 'a');
}

void Game::handlePromotion(const Position& pos) {
    std::cout << "Pawn promotion! Choose piece (Q/R/B/N): ";
    char choice;
    std::cin >> choice;
    
    SpecialMoves::promotePawn(pos, choice, board);
}

void Game::handleCastling(const std::string& command) {
    bool kingSide = (command == "kingside");
    
    if (kingSide) {
        if (!SpecialMoves::canCastleKingSide(currentPlayer, board)) {
            throw ChessException("Cannot castle kingside!");
        }
        SpecialMoves::performCastling(currentPlayer, true, board);
    } else {
        if (!SpecialMoves::canCastleQueenSide(currentPlayer, board)) {
            throw ChessException("Cannot castle queenside!");
        }
        SpecialMoves::performCastling(currentPlayer, false, board);
    }
    
    board.clearEnPassant();
    switchPlayer();
    checkGameStatus();
}

bool Game::hasValidMoves(Color color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board.getPiece(i, j);
            if (piece && piece->getColor() == color) {
                Position from(i, j);
                
                for (int ti = 0; ti < 8; ti++) {
                    for (int tj = 0; tj < 8; tj++) {
                        Position to(ti, tj);
                        if (piece->isValidMove(to, board)) {
                            if (!board.wouldBeInCheck(from, to, color)) {
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

void Game::checkGameStatus() {
    bool inCheck = board.isInCheck(currentPlayer);
    bool hasLegalMoves = hasValidMoves(currentPlayer);
    
    if (!hasLegalMoves) {
        gameOver = true;
        if (inCheck) {
            winner = (currentPlayer == Color::WHITE) ? "Black" : "White";
            std::cout << "\nCheckmate! " << winner << " wins!\n";
        } else {
            std::cout << "\nStalemate! It's a draw!\n";
        }
    }
}