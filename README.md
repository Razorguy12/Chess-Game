# Chess-Game
A C++ Chess Engine with full rules, move validation, and special moves, played in the terminal.

## Features
*   **Full Chess Rules:** Implements all standard chess rules.
*   **Move Validation:** Enforces legal moves for all pieces.
*   **Special Moves:** Supports castling, pawn promotion, and en passant.
*   **Game End Conditions:** Detects check, checkmate, and stalemate.
*   **Terminal Interface:** Playable in a standard terminal environment.

## System Requirements
*   A C++17 compatible compiler (e.g., g++)
*   `make` build automation tool

## How to Install and Run
1.  **Clone the repository:**
    ```sh
    git clone https://github.com/your-username/Chess-Game.git
    cd Chess-Game
    ```
2.  **Build the game:**
    Run the `make` command to compile the source code.
    ```sh
    make
    ```
3.  **Run the game:**
    Execute the compiled program to start playing.
    ```sh
    ./chess
    ```

## Code Overview
The project is structured using object-oriented principles in C++.

*   `Game`: The main class that orchestrates the game flow, player turns, and game state.
*   `Board`: Represents the chessboard and manages the placement and movement of pieces.
*   `Piece`: An abstract base class for all chess pieces, with derived classes for each piece type (Pawn, Rook, Knight, Bishop, Queen, King). This uses polymorphism to handle piece-specific logic.
*   `Player`: Represents a player, tracking their color and game status.
*   `Position`: A simple class to represent a position on the board.
*   `SpecialMoves`: A utility class with static methods to handle special moves like castling, promotion, and en passant.

The `main.cpp` file creates a `Game` object and starts the game loop. The game is played by entering moves in algebraic notation (e.g., "e2e4").