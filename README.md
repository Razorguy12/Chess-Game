# Chess-Game
A C++ Chess Game with full rules, move validation, and special moves, played in the terminal.

## Features
*   **Full Chess Rules:** Implements all standard chess rules.
*   **Move Validation:** Enforces legal moves for all pieces.
*   **Special Moves:** Supports castling, pawn promotion, and en passant.
*   **Game End Conditions:** Detects check, checkmate, and stalemate.
*   **Terminal Interface:** Playable in a standard terminal environment.
---

## System Requirements
*   A C++17 compatible compiler (e.g g++ / clang)
*   `make` build automation tool

---

## How to Install and Run
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/Chess-Game.git
    cd Chess-Game
    ```
2. **Build and Run:**\
    You can either use make or compile each yourself.\
    we will show the method using make.
    ```bash
    make run
    ```
3. **Clean up after Game**\
    After the game clean up the object and executable files
    ```bash
    make clean
    ```
---

## Code Overview
The project is structured using object-oriented principles in C++.

*   `Game`: The main class that orchestrates the game flow, player turns, and game state.
*   `Board`: Represents the chessboard and manages the placement and movement of pieces.
*   `Piece`: An abstract base class for all chess pieces, with derived classes for each piece type (Pawn, Rook, Knight, Bishop, Queen, King). This uses polymorphism to handle piece-specific logic.
*   `Player`: Represents a player, tracking their color and game status.
*   `Position`: A simple class to represent a position on the board.
*   `SpecialMoves`: A utility class with static methods to handle special moves like castling, promotion, and en passant.

The `main.cpp` file creates a `Game` object and starts the game loop. The game is played by entering moves in algebraic notation (e.g., "e2 e4").

---

## Game rules

* These follow standard FIDE chess rules -> [FIDE Chess Rule](https://handbook.fide.com/chapter/e012023)
* The input should be in the format <from><space><to>. Any other format in not suitable.

## Output
<video src="Asset/run_chess.mp4" autoplay loop muted playsinline width="600"></video>


## Teams

1. ***Akshai Krishna KP***
2. ***Mohammed Azhar Sait H***
3. ***Karthik Krishna K***
