#!/bin/bash

# Comprehensive Chess Game Test Script
# Tests all types of moves, special moves, and edge cases

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0
TOTAL_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local input="$2"
    local expected_pattern="$3"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo ""
    echo -e "${BLUE}Test #$TOTAL_TESTS: $test_name${NC}"
    
    # Create temporary input file
    local input_file=$(mktemp)
    echo "White" > "$input_file"
    echo "Black" >> "$input_file"
    echo "$input" >> "$input_file"
    echo -e "quit\n1" >> "$input_file"
    
    # Run the program with timeout and capture output
    local output=$(timeout 5 ./chess < "$input_file" 2>&1)
    local exit_code=$?
    
    # Clean up temp file
    rm -f "$input_file"
    
    # Check for timeout
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}Test Failed - Program Timeout${NC}"
        echo "Details: Program did not complete within 5 seconds"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # Check if expected pattern is found in output
    if echo "$output" | grep -qE "$expected_pattern"; then
        echo -e "${GREEN}Test Passed${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        echo -e "${RED}Test Failed${NC}"
        echo "Details: Expected pattern '$expected_pattern' not found in output"
        echo ""
        echo "Board Position at Failure:"
        echo "----------------------------------------"
        # Extract the last board position (look for the board pattern with ranks 8-1)
        echo "$output" | awk '/^[[:space:]]*8/{p=1} p{print} /^[[:space:]]*1/{if(p) exit}' | tail -20
        echo "----------------------------------------"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

# Function to test that a move should fail
test_invalid_move() {
    local test_name="$1"
    local input="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo ""
    echo -e "${BLUE}Test #$TOTAL_TESTS: $test_name${NC}"
    
    local input_file=$(mktemp)
    echo "White" > "$input_file"
    echo "Black" >> "$input_file"
    echo "$input" >> "$input_file"
    echo -e "quit\n1" >> "$input_file"
    
    local output=$(timeout 5 ./chess < "$input_file" 2>&1)
    local exit_code=$?
    
    rm -f "$input_file"
    
    # Check for timeout
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}Test Failed - Program Timeout${NC}"
        echo "Details: Program did not complete within 5 seconds"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # Should contain error message
    if echo "$output" | grep -qiE "(Error|Invalid|not your piece|No piece|illegal|cannot)"; then
        echo -e "${GREEN}Test Passed - Invalid move correctly rejected${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        return 0
    else
        echo -e "${RED}Test Failed${NC}"
        echo "Details: Invalid move was not rejected by the program"
        echo ""
        echo "Board Position at Failure:"
        echo "----------------------------------------"
        # Extract the last board position
        echo "$output" | awk '/^[[:space:]]*8/{p=1} p{print} /^[[:space:]]*1/{if(p) exit}' | tail -20
        echo "----------------------------------------"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
}

# Check if chess executable exists
if [ ! -f "./chess" ]; then
    echo -e "${RED}Error: chess executable not found. Please compile first.${NC}"
    exit 1
fi

echo "=========================================="
echo "  Chess Game Comprehensive Test Suite"
echo "=========================================="

# ============================================
# PAWN MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== PAWN MOVEMENT TESTS ===${NC}"

run_test "Pawn Initial Double Move - White" \
    "e2 e4" \
    "Black's turn|Black to move" \
    "White pawn moves two squares from initial position"

run_test "Pawn Initial Double Move - Black" \
    "e2 e4
e7 e5" \
    "White's turn|White to move" \
    "Black pawn moves two squares from initial position"

run_test "Pawn Single Move Forward - White" \
    "e2 e3" \
    "Black's turn|Black to move" \
    "White pawn moves one square forward"

run_test "Pawn Single Move Forward - Black" \
    "e2 e4
d7 d6" \
    "White's turn|White to move" \
    "Black pawn moves one square forward"

run_test "Pawn Diagonal Capture - White" \
    "e2 e4
d7 d5
e4 d5" \
    "Black's turn|Black to move" \
    "White pawn captures black pawn diagonally"

run_test "Pawn Diagonal Capture - Black" \
    "e2 e4
d7 d5
e4 d5
c7 c5
d5 c6" \
    "Black's turn|Black to move" \
    "White pawn captures another black pawn diagonally"

test_invalid_move "Pawn Cannot Move Backward" \
    "e2 e4
e7 e5
e4 e3" \
    "Pawn should not be able to move backward"

test_invalid_move "Pawn Cannot Move Diagonally Without Capture" \
    "e2 e4
e7 e5
e4 f5" \
    "Pawn cannot move diagonally to empty square"

test_invalid_move "Pawn Cannot Move Forward Into Occupied Square" \
    "e2 e4
e7 e5
e4 e5" \
    "Pawn cannot move forward into occupied square"

test_invalid_move "Pawn Cannot Double Move After First Move" \
    "e2 e3
e7 e5
e3 e5" \
    "Pawn cannot double move after initial move"

test_invalid_move "Pawn Cannot Move Three Squares" \
    "e2 e5" \
    "Pawn cannot move three squares"

test_invalid_move "Pawn Cannot Capture Forward" \
    "e2 e4
d7 d5
e4 d5
e7 e5
d5 e5" \
    "Pawn cannot capture forward (only diagonal)"

# ============================================
# KNIGHT MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== KNIGHT MOVEMENT TESTS ===${NC}"

run_test "Knight L-Shape Move (2 up, 1 right)" \
    "b1 c3" \
    "Black's turn|Black to move" \
    "White knight moves in L-shape pattern"

run_test "Knight L-Shape Move (2 up, 1 left)" \
    "g1 f3" \
    "Black's turn|Black to move" \
    "White knight moves in L-shape pattern"

run_test "Knight L-Shape Move (1 up, 2 right)" \
    "g1 h3" \
    "Black's turn|Black to move" \
    "White knight moves in L-shape pattern"

run_test "Knight L-Shape Move (1 up, 2 left)" \
    "b1 a3" \
    "Black's turn|Black to move" \
    "White knight moves in L-shape pattern"

run_test "Knight Can Jump Over Pieces" \
    "b1 c3" \
    "Black's turn|Black to move" \
    "Knight jumps over pawn at b2"

run_test "Knight Capture" \
    "b1 c3
b8 c6
c3 d5
c6 d4
d5 f6" \
    "Black's turn|Black to move" \
    "Knight captures opponent piece"

test_invalid_move "Knight Invalid Move (Straight Line)" \
    "b1 b3" \
    "Knight cannot move in straight line"

test_invalid_move "Knight Invalid Move (Diagonal)" \
    "b1 d3" \
    "Knight cannot move diagonally"

test_invalid_move "Knight Cannot Capture Own Piece" \
    "b1 d2" \
    "Knight cannot capture own piece"

# ============================================
# BISHOP MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== BISHOP MOVEMENT TESTS ===${NC}"

run_test "Bishop Diagonal Move (Up-Right)" \
    "e2 e4
e7 e5
f1 c4" \
    "Black's turn|Black to move" \
    "White bishop moves diagonally up-right"

run_test "Bishop Diagonal Move (Up-Left)" \
    "d2 d4
d7 d5
c1 f4" \
    "Black's turn|Black to move" \
    "White bishop moves diagonally up-left"

run_test "Bishop Long Diagonal Move" \
    "e2 e4
e7 e5
f1 a6" \
    "Black's turn|Black to move" \
    "Bishop moves multiple squares diagonally"

run_test "Bishop Capture" \
    "e2 e4
d7 d5
f1 c4
c8 g4
c4 f7" \
    "Black's turn|Black to move|CHECK|Checkmate" \
    "Bishop captures opponent piece"

test_invalid_move "Bishop Cannot Move Straight" \
    "e2 e4
e7 e5
f1 c4
f8 c5
c4 c6" \
    "Bishop cannot move in straight line"

test_invalid_move "Bishop Cannot Jump Over Pieces" \
    "f1 c4" \
    "Bishop cannot move through pawn"

test_invalid_move "Bishop Cannot Move Like Knight" \
    "e2 e4
e7 e5
f1 d3" \
    "Bishop cannot move in L-shape"

# ============================================
# ROOK MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== ROOK MOVEMENT TESTS ===${NC}"

run_test "Rook Horizontal Move" \
    "a2 a4
a7 a5
a1 a3" \
    "Black's turn|Black to move" \
    "Rook moves horizontally"

run_test "Rook Vertical Move" \
    "a2 a4
a7 a5
a1 a3
a8 a6" \
    "White's turn|White to move" \
    "Rook moves vertically"

run_test "Rook Long Distance Move" \
    "h2 h4
h7 h5
h1 h3
h8 h6" \
    "White's turn|White to move" \
    "Rook moves multiple squares"

run_test "Rook Capture" \
    "a2 a4
a7 a5
a1 a3
a8 a6
a3 a5" \
    "Black's turn|Black to move" \
    "Rook captures opponent piece"

test_invalid_move "Rook Cannot Move Diagonally" \
    "a2 a4
a7 a5
a1 b2" \
    "Rook cannot move diagonally"

test_invalid_move "Rook Cannot Jump Over Pieces" \
    "a1 a3" \
    "Rook cannot jump over pawn"

test_invalid_move "Rook Cannot Move Like Knight" \
    "a2 a4
a7 a5
a1 b3" \
    "Rook cannot move in L-shape"

# ============================================
# QUEEN MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== QUEEN MOVEMENT TESTS ===${NC}"

run_test "Queen Diagonal Move" \
    "e2 e4
e7 e5
d1 f3" \
    "Black's turn|Black to move" \
    "Queen moves diagonally"

run_test "Queen Horizontal Move" \
    "e2 e4
e7 e5
d1 e2" \
    "Black's turn|Black to move" \
    "Queen moves horizontally"

run_test "Queen Vertical Move" \
    "d2 d4
d7 d5
d1 d3" \
    "Black's turn|Black to move" \
    "Queen moves vertically"

run_test "Queen Long Distance Move" \
    "e2 e4
e7 e5
d1 h5" \
    "Black's turn|Black to move" \
    "Queen moves multiple squares diagonally"

run_test "Queen Capture" \
    "e2 e4
d7 d5
d1 h5
d5 d4
h5 e5" \
    "Black's turn|Black to move|CHECK" \
    "Queen captures opponent piece"

test_invalid_move "Queen Cannot Jump Over Pieces" \
    "d1 d3" \
    "Queen cannot jump over pawn"

test_invalid_move "Queen Cannot Move Like Knight" \
    "e2 e4
e7 e5
d1 f2" \
    "Queen cannot move in L-shape"

# ============================================
# KING MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== KING MOVEMENT TESTS ===${NC}"

run_test "King Move One Square Forward" \
    "e2 e4
e7 e5
e1 e2" \
    "Black's turn|Black to move" \
    "King moves one square forward"

run_test "King Move One Square Diagonal" \
    "e2 e4
e7 e5
e1 e2
e8 e7" \
    "White's turn|White to move" \
    "King moves one square diagonally"

run_test "King Move One Square Sideways" \
    "d2 d4
d7 d5
e1 d2" \
    "Black's turn|Black to move" \
    "King moves one square sideways"

run_test "King Capture" \
    "e2 e4
d7 d5
e1 e2
d5 d4
e2 d3
d4 d3" \
    "White's turn|White to move" \
    "King captures opponent piece"

test_invalid_move "King Cannot Move Two Squares" \
    "e2 e4
e7 e5
e1 e3" \
    "King cannot move more than one square"

test_invalid_move "King Cannot Move Into Check" \
    "e2 e4
e7 e5
f1 c4
d8 h4
e1 e2" \
    "King cannot move into check"

test_invalid_move "King Cannot Move Like Knight" \
    "e2 e4
e7 e5
e1 f3" \
    "King cannot move in L-shape"

# ============================================
# CASTLING
# ============================================
echo ""
echo -e "${YELLOW}=== CASTLING TESTS ===${NC}"

run_test "Kingside Castling - White (O-O)" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
g8 f6
O-O" \
    "Black's turn|Black to move" \
    "White performs kingside castling"

run_test "Kingside Castling - White (0-0)" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
g8 f6
0-0" \
    "Black's turn|Black to move" \
    "White performs kingside castling with zeros"

run_test "Queenside Castling - White (O-O-O)" \
    "d2 d4
d7 d5
b1 c3
b8 c6
c1 f4
c8 f5
d1 d2
d8 d7
O-O-O" \
    "Black's turn|Black to move" \
    "White performs queenside castling"

run_test "Queenside Castling - White (0-0-0)" \
    "d2 d4
d7 d5
b1 c3
b8 c6
c1 f4
c8 f5
d1 d2
d8 d7
0-0-0" \
    "Black's turn|Black to move" \
    "White performs queenside castling with zeros"

run_test "Kingside Castling - Black" \
    "e2 e4
e7 e5
g1 f3
g8 f6
f1 c4
f8 c5
O-O
O-O" \
    "White's turn|White to move" \
    "Black performs kingside castling"

run_test "Queenside Castling - Black" \
    "d2 d4
d7 d5
b1 c3
b8 c6
c1 f4
c8 f5
d1 d2
d8 d7
O-O-O
O-O-O" \
    "White's turn|White to move" \
    "Black performs queenside castling"

test_invalid_move "Cannot Castle When Pieces Between King and Rook" \
    "O-O" \
    "Cannot castle with pieces in the way"

test_invalid_move "Cannot Castle After King Moved" \
    "e2 e4
e7 e5
e1 e2
e8 e7
e2 e1
e7 e8
g1 f3
g8 f6
f1 c4
f8 c5
O-O" \
    "Cannot castle after king has moved"

test_invalid_move "Cannot Castle After Rook Moved" \
    "a2 a4
a7 a5
a1 a2
a8 a7
a2 a1
a7 a8
d2 d4
d7 d5
b1 c3
b8 c6
c1 f4
c8 f5
d1 d2
d8 d7
O-O-O" \
    "Cannot castle after rook has moved"

test_invalid_move "Cannot Castle Through Check" \
    "e2 e4
e7 e5
g1 f3
d8 h4
f1 c4
h4 e4
O-O" \
    "Cannot castle through check"

test_invalid_move "Cannot Castle While In Check" \
    "e2 e4
e7 e5
f1 c4
d8 h4
g1 f3
h4 e4
e1 e2
e4 e2
O-O" \
    "Cannot castle while in check"

test_invalid_move "Cannot Castle Into Check" \
    "e2 e4
d7 d5
g1 f3
c8 g4
f1 c4
g4 f3
O-O" \
    "Cannot castle into check"

# ============================================
# EN PASSANT
# ============================================
echo ""
echo -e "${YELLOW}=== EN PASSANT TESTS ===${NC}"

run_test "En Passant Capture - White" \
    "e2 e4
a7 a6
e4 e5
d7 d5
e5 d6" \
    "Black's turn|Black to move" \
    "White pawn captures black pawn en passant"

run_test "En Passant Capture - Black" \
    "a2 a3
e7 e5
b2 b4
e5 e4
f2 f4
e4 f3" \
    "White's turn|White to move" \
    "Black pawn captures white pawn en passant"

run_test "En Passant From Left" \
    "e2 e4
a7 a6
e4 e5
f7 f5
e5 f6" \
    "Black's turn|Black to move" \
    "En passant capture from the left"

run_test "En Passant From Right" \
    "d2 d4
a7 a6
d4 d5
c7 c5
d5 c6" \
    "Black's turn|Black to move" \
    "En passant capture from the right"

test_invalid_move "En Passant Only On Next Move" \
    "e2 e4
a7 a6
e4 e5
d7 d5
b1 c3
b8 c6
e5 d6" \
    "En passant must be done immediately"

test_invalid_move "En Passant Only After Double Pawn Move" \
    "e2 e4
d7 d6
e4 e5
d6 d5
e5 d6" \
    "En passant only works after double pawn move"

# ============================================
# PAWN PROMOTION
# ============================================
echo ""
echo -e "${YELLOW}=== PAWN PROMOTION TESTS ===${NC}"

run_test "Pawn Promotion to Queen" \
    "e2 e4
d7 d5
e4 d5
c7 c6
d5 c6
b7 b6
c6 b7
b8 c6
b7 b8
Q" \
    "Black's turn|Black to move" \
    "White pawn promotes to Queen"

run_test "Pawn Promotion to Rook" \
    "e2 e4
d7 d5
e4 d5
c7 c6
d5 c6
b7 b6
c6 b7
b8 c6
b7 b8
R" \
    "Black's turn|Black to move" \
    "White pawn promotes to Rook"

run_test "Pawn Promotion to Bishop" \
    "e2 e4
d7 d5
e4 d5
c7 c6
d5 c6
b7 b6
c6 b7
b8 c6
b7 b8
B" \
    "Black's turn|Black to move" \
    "White pawn promotes to Bishop"

run_test "Pawn Promotion to Knight" \
    "e2 e4
d7 d5
e4 d5
c7 c6
d5 c6
b7 b6
c6 b7
b8 c6
b7 b8
N" \
    "Black's turn|Black to move" \
    "White pawn promotes to Knight"

run_test "Black Pawn Promotion" \
    "a2 a3
e7 e5
b2 b3
e5 e4
c2 c3
e4 e3
d2 e3
d7 d5
e3 d4
d5 d4
e1 d2
d4 d3
d2 d1
d3 d2
d1 e1
d2 d1
Q" \
    "White's turn|White to move" \
    "Black pawn promotes to Queen"

# ============================================
# CHECK AND CHECKMATE
# ============================================
echo ""
echo -e "${YELLOW}=== CHECK AND CHECKMATE TESTS ===${NC}"

run_test "Check Detection - Queen" \
    "e2 e4
e7 e5
d1 h5
g8 f6
h5 e5" \
    "CHECK|Check" \
    "Queen puts king in check"

run_test "Check Detection - Bishop" \
    "e2 e4
e7 e5
f1 c4
b8 c6
c4 f7" \
    "CHECK|Check" \
    "Bishop puts king in check"

run_test "Check Detection - Knight" \
    "e2 e4
e7 e5
g1 f3
f8 c5
f3 g5
g8 f6
g5 f7" \
    "CHECK|Check" \
    "Knight puts king in check"

run_test "Check Detection - Rook" \
    "e2 e4
e7 e5
d1 h5
e8 e7
h5 e5" \
    "CHECK|Check" \
    "Rook puts king in check"

run_test "Scholar's Mate - Checkmate" \
    "e2 e4
e7 e5
f1 c4
b8 c6
d1 f3
f8 c5
f3 f7" \
    "Checkmate|CHECKMATE|Game Over" \
    "Famous Scholar's Mate checkmate pattern"

run_test "Back Rank Mate" \
    "e2 e4
e7 e5
g1 f3
f8 c5
f1 c4
b8 c6
O-O
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
b1 c3
h7 h6
c3 d5
h6 g5
f3 g5
f6 d5
g5 f7
e8 f7
c4 d5" \
    "CHECK|Checkmate|CHECKMATE" \
    "Back rank checkmate pattern"

test_invalid_move "Cannot Move Into Check" \
    "e2 e4
d8 h4
e1 e2" \
    "King cannot move into check"

test_invalid_move "Must Block or Move When In Check" \
    "e2 e4
e7 e5
d1 h5
b8 c6
h5 f7
g8 h6" \
    "Must respond to check"

test_invalid_move "Cannot Leave King In Check" \
    "e2 e4
e7 e5
d1 h5
b8 c6
h5 f7
a7 a6" \
    "Cannot make move that leaves king in check"

# ============================================
# STALEMATE
# ============================================
echo ""
echo -e "${YELLOW}=== STALEMATE TESTS ===${NC}"

run_test "Simple Stalemate Position" \
    "e2 e3
a7 a5
d1 h5
a8 a6
h5 a5
h7 h5
a5 c7
a6 h6
h2 h4
f7 f6
c7 d7
e8 f7
d7 b7
d8 d3
b7 b8
d3 h7
b8 c8
f7 g6
c8 e6" \
    "Stalemate|STALEMATE|Draw" \
    "Position where player has no legal moves but not in check"

# ============================================
# INVALID INPUT HANDLING
# ============================================
echo ""
echo -e "${YELLOW}=== INVALID INPUT TESTS ===${NC}"

test_invalid_move "Invalid Position - Out of Bounds (Column)" \
    "i2 i4" \
    "Position outside board boundaries"

test_invalid_move "Invalid Position - Out of Bounds (Row)" \
    "e0 e2" \
    "Position outside board boundaries"

test_invalid_move "Invalid Position - Out of Bounds (High Row)" \
    "e9 e7" \
    "Position outside board boundaries"

test_invalid_move "Move From Empty Square" \
    "e3 e4" \
    "No piece at starting position"

test_invalid_move "Move Opponent's Piece" \
    "e7 e5" \
    "Cannot move opponent's piece"

test_invalid_move "Incomplete Move Input" \
    "e2" \
    "Incomplete move specification"

test_invalid_move "Same Source and Destination" \
    "e2 e2" \
    "Source and destination are the same"

test_invalid_move "Move To Capture Own Piece" \
    "b1 d2" \
    "Cannot capture own piece"

# ============================================
# COMPLEX GAME SCENARIOS
# ============================================
echo ""
echo -e "${YELLOW}=== COMPLEX GAME SCENARIOS ===${NC}"

run_test "Italian Game Opening" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
b1 c3
g8 f6
d2 d3
d7 d6" \
    "White's turn|White to move" \
    "Famous Italian Game opening sequence"

run_test "Spanish Opening (Ruy Lopez)" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 b5
a7 a6
b5 a4
g8 f6" \
    "White's turn|White to move" \
    "Famous Ruy Lopez opening"

run_test "Multiple Captures Sequence" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 d4
g1 e2
d4 e4" \
    "White's turn|White to move" \
    "Series of captures and recaptures"

run_test "Piece Coordination" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
b1 d2
h7 h6
g5 f6
g7 f6
d2 f1
c6 d4" \
    "White's turn|White to move" \
    "Complex position with multiple piece interactions"

run_test "Pawn Structure Changes" \
    "d2 d4
d7 d5
c2 c4
e7 e6
b1 c3
g8 f6
c1 g5
f8 e7
e2 e3
O-O" \
    "White's turn|White to move" \
    "Queen's Gambit Declined structure"

run_test "Endgame Position" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
b1 c3
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
c3 d5
O-O
g5 f6
g7 f6
O-O
c6 d4
f3 d4
c5 d4
c4 b3
d8 d7
d1 h5
f8 g8" \
    "White's turn|White to move" \
    "Complex middlegame position transitioning to endgame"

# ============================================
# PIN AND SKEWER TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== PIN AND SKEWER TESTS ===${NC}"

run_test "Bishop Pin" \
    "e2 e4
e7 e5
f1 c4
b8 c6
c4 f7
e8 f7
d1 f3" \
    "CHECK|Checkmate|CHECKMATE" \
    "Bishop pins piece to king"

run_test "Rook Pin Setup" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
f1 e1" \
    "Black's turn|Black to move" \
    "Rook pins along file"

run_test "Queen Pin" \
    "e2 e4
c7 c5
g1 f3
d7 d6
d2 d4
c5 d4
f3 d4
g8 f6
b1 c3
a7 a6
c1 g5" \
    "Black's turn|Black to move" \
    "Queen pins knight to king"

# ============================================
# DISCOVERED CHECK TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== DISCOVERED CHECK TESTS ===${NC}"

run_test "Discovered Check Setup" \
    "e2 e4
e7 e5
f1 c4
b8 c6
d1 f3
g8 f6
f3 f7" \
    "CHECK|Checkmate|CHECKMATE" \
    "Moving piece reveals check from another piece"

# ============================================
# ZUGZWANG POSITIONS
# ============================================
echo ""
echo -e "${YELLOW}=== ZUGZWANG POSITION TESTS ===${NC}"

run_test "Opposition in King Endgame" \
    "e2 e4
d7 d5
e4 d5
d8 d5
g1 f3
d5 d4
f1 c4
d4 c4
O-O
c4 c2
e1 f1
c2 d2
f1 g1
d2 d1
g1 h2
d1 d2
h2 g1
d2 d1
g1 h2
d1 e2" \
    "White's turn|White to move" \
    "Position where any move worsens position"

# ============================================
# PIECE VALUE TRADES
# ============================================
echo ""
echo -e "${YELLOW}=== PIECE TRADE TESTS ===${NC}"

run_test "Equal Trade - Knights" \
    "e2 e4
e7 e5
g1 f3
b8 c6
b1 c3
g8 f6
f3 e5
c6 e5" \
    "White's turn|White to move" \
    "Equal value piece exchange"

run_test "Equal Trade - Bishops" \
    "e2 e4
e7 e5
f1 c4
f8 c5
c4 f7
e8 f7" \
    "White's turn|White to move" \
    "Bishop for bishop trade"

run_test "Unequal Trade - Pawn for Knight" \
    "e2 e4
e7 e5
g1 f3
b8 c6
d2 d4
e5 d4
f3 d4
c6 d4" \
    "White's turn|White to move" \
    "Trading pawn for knight (favorable)"

run_test "Queen Trade" \
    "e2 e4
e7 e5
d1 h5
b8 c6
h5 e5
d8 e7
e5 e7
f8 e7" \
    "White's turn|White to move" \
    "Queen exchange"

# ============================================
# PAWN STRUCTURE TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== PAWN STRUCTURE TESTS ===${NC}"

run_test "Doubled Pawns" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
g5 f6
g7 f6" \
    "White's turn|White to move" \
    "Creating doubled pawns"

run_test "Isolated Pawn" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 a5
d2 d4
c7 c6
g1 f3
c8 f5" \
    "White's turn|White to move" \
    "Creating isolated d-pawn"

run_test "Passed Pawn Creation" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 d8
d2 d4
g8 f6
g1 f3
c8 f5
f1 d3
f5 d3
d1 d3
b8 c6
d3 e3
e7 e6
c1 f4
f8 d6
f4 d6
c7 d6
O-O-O
e8 c8
d4 d5
e6 d5
c3 d5
f6 d5
e3 d4" \
    "Black's turn|Black to move" \
    "Creating and advancing passed pawn"

run_test "Pawn Chain" \
    "e2 e4
d7 d5
e4 d5
d8 d5
d2 d4
g8 f6
g1 f3
c8 f5
f1 e2
e7 e6
O-O
f8 d6
c2 c4
d5 d8
b1 c3
O-O" \
    "White's turn|White to move" \
    "Building pawn chain structure"

# ============================================
# FORK TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== FORK (DOUBLE ATTACK) TESTS ===${NC}"

run_test "Knight Fork - King and Rook" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
b1 c3
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
c3 d5
f6 d5
c4 d5
g4 f3
d1 f3
c6 d4
f3 f4
d4 c2" \
    "White's turn|White to move" \
    "Knight forks king and rook"

run_test "Knight Fork - Queen and Rook" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
b1 c3
c8 g4
c3 d5
f6 d5
c4 d5
c6 e7
c1 g5
e7 g6
g5 d8
a8 d8
d5 c6" \
    "Black's turn|Black to move" \
    "Bishop forks queen and rook"

run_test "Pawn Fork" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 a5
d2 d4
c8 f5
g1 f3
e7 e6
f1 d3
f5 d3
d1 d3
b8 c6
d3 e3
g8 f6
c1 g5
f8 e7
O-O-O
O-O
d4 d5
e6 d5
c3 d5
f6 d5
e3 d4
d5 f6
g5 f6
e7 f6
d4 d7" \
    "Black's turn|Black to move" \
    "Pawn forks two pieces"

# ============================================
# BATTERY (DOUBLED PIECES) TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== BATTERY TESTS ===${NC}"

run_test "Queen and Bishop Battery" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 g5
h7 h6
g5 h4
g7 g5
h4 g3
c8 g4
d1 d2" \
    "Black's turn|Black to move" \
    "Queen and bishop on same diagonal"

run_test "Rook Doubling on File" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
b1 d2
h7 h6
g5 f6
d8 f6
d2 f1
O-O
f1 e3
f6 d8
f1 e1" \
    "Black's turn|Black to move" \
    "Doubling rooks on e-file"

# ============================================
# SACRIFICE TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== SACRIFICE TESTS ===${NC}"

run_test "Greek Gift Sacrifice Setup" \
    "e2 e4
e7 e6
d2 d4
d7 d5
b1 c3
g8 f6
c1 g5
f8 e7
e4 e5
f6 d7
g5 e7
d8 e7
f2 f4
O-O
g1 f3
c7 c5
d1 d2
b8 c6
O-O-O
c5 c4
f3 g5
h7 h6" \
    "White's turn|White to move" \
    "Setup for classic bishop sacrifice"

run_test "Exchange Sacrifice" \
    "e2 e4
c7 c5
g1 f3
d7 d6
d2 d4
c5 d4
f3 d4
g8 f6
b1 c3
a7 a6
c1 g5
e7 e6
d1 d2
f8 e7
O-O-O
O-O
f2 f4
d8 c7
f1 d3
b8 c6
h1 e1
c6 d4
d2 d4
b7 b5
d4 g7" \
    "Black's turn|Black to move" \
    "Rook sacrifice for attack"

# ============================================
# BACK RANK WEAKNESS TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== BACK RANK WEAKNESS TESTS ===${NC}"

run_test "Back Rank Mate Threat" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
b1 d2
h7 h6
g5 f6
d8 f6
d2 f1
O-O-O
f1 e3
f6 d8
c4 b5
c6 d4
b5 d7" \
    "Black's turn|Black to move" \
    "Exploiting back rank weakness"

# ============================================
# UNDERPROMOTION TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== UNDERPROMOTION TESTS ===${NC}"

run_test "Underpromotion to Avoid Stalemate" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 d4
c3 e2
d4 d1
e1 d1
c8 f5
d1 c1
f5 c2
c1 c2
e7 e6
c2 d3
e6 d5
d3 e3
d5 d4
e3 f4
d4 d3
f4 g5
d3 d2
g5 f6
b8 c6
f6 g7
c6 e5
g7 h7
d2 d1
R" \
    "Black's turn|Black to move" \
    "Promoting to rook instead of queen"

# ============================================
# MULTIPLE CHECK RESPONSES
# ============================================
echo ""
echo -e "${YELLOW}=== CHECK RESPONSE TESTS ===${NC}"

run_test "Block Check" \
    "e2 e4
e7 e5
d1 h5
b8 c6
h5 f7
e8 f7
f1 c4
f7 e7
c4 e6" \
    "Black's turn|Black to move|CHECK" \
    "Must block check"

run_test "Capture Checking Piece" \
    "e2 e4
e7 e5
d1 h5
g8 f6
h5 e5
d8 e7
e5 e7
f8 e7" \
    "White's turn|White to move" \
    "Capture piece giving check"

run_test "Move King Out of Check" \
    "e2 e4
e7 e5
d1 h5
e8 e7" \
    "White's turn|White to move" \
    "King moves to escape check"

# ============================================
# CASTLING EDGE CASES
# ============================================
echo ""
echo -e "${YELLOW}=== CASTLING EDGE CASE TESTS ===${NC}"

test_invalid_move "Cannot Castle When King In Check" \
    "e2 e4
e7 e5
g1 f3
d8 h4
f1 c4
h4 e4
O-O" \
    "Cannot castle while in check"

test_invalid_move "Cannot Castle When Destination In Check" \
    "e2 e4
d7 d5
g1 f3
c8 g4
f1 c4
g4 f3
O-O" \
    "Cannot castle into check"

test_invalid_move "Cannot Castle When Path Through Check" \
    "e2 e4
e7 e5
g1 f3
d8 h4
f1 c4
h4 e4
O-O" \
    "Cannot castle through attacked square"

# ============================================
# LONG MOVE SEQUENCES
# ============================================
echo ""
echo -e "${YELLOW}=== LONG SEQUENCE TESTS ===${NC}"

run_test "20+ Move Game" \
    "d2 d4
g8 f6
c2 c4
e7 e6
g1 f3
d7 d5
b1 c3
f8 e7
c1 f4
O-O
e2 e3
c7 c5
d4 c5
e7 c5
d1 c2
b8 c6
a2 a3
d8 a5
f1 d3
c5 e7
O-O
d5 c4
d3 c4
e6 e5
f4 g3
a5 c7
c4 d5
c6 d8
f1 d1
f8 e8
b2 b4
c8 e6
d5 e6
f7 e6" \
    "White's turn|White to move" \
    "Long positional game"

# ============================================
# ALL PIECES ON BOARD MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== ALL PIECE TYPE MOVES IN ONE GAME ===${NC}"

run_test "Using All Piece Types" \
    "e2 e4
e7 e6
d2 d4
d7 d5
b1 d2
c7 c5
e4 d5
e6 d5
g1 f3
b8 c6
f1 b5
f8 d6
O-O
g8 e7
d4 c5
d6 c5
d2 b3
c5 e7
f1 e1
O-O
c1 f4
c8 f5
d1 d2
d8 b6
a1 d1
a8 d8
c2 c4
d5 c4" \
    "White's turn|White to move" \
    "Game using pawns, knights, bishops, rooks, queens, and kings"

# ============================================
# FIFTY MOVE RULE SETUP
# ============================================
echo ""
echo -e "${YELLOW}=== ENDGAME RULE TESTS ===${NC}"

run_test "King and Pawn vs King Endgame" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
h2 h3
g4 f3
d1 f3
c6 d4
f3 d1
d4 c2
d1 d2
c2 a1
b1 c3
a1 c2
a2 a4
c2 e3
f2 e3
d8 d7
c4 b5
c7 c6
b5 d7
e8 d7" \
    "White's turn|White to move" \
    "Simplified endgame position"

# ============================================
# SPECIAL BOARD POSITIONS
# ============================================
echo ""
echo -e "${YELLOW}=== SPECIAL POSITION TESTS ===${NC}"

run_test "All Pieces Developed" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d3
d7 d6
c1 e3
c5 e3
f2 e3
c8 e6
c4 e6
f7 e6
b1 c3
d8 d7
d1 d2
O-O-O
a1 d1
d7 c8" \
    "White's turn|White to move" \
    "Position with all pieces developed"

run_test "Closed Position" \
    "d2 d4
d7 d5
c2 c4
e7 e6
b1 c3
g8 f6
c1 g5
f8 e7
e2 e3
O-O
g1 f3
b8 d7
a1 c1
c7 c6
f1 d3
d5 c4
d3 c4
f6 d5
g5 e7
d8 e7
O-O
d5 c3
b2 c3" \
    "Black's turn|Black to move" \
    "Closed pawn structure"

# ============================================
# PROMOTION VARIATIONS
# ============================================
echo ""
echo -e "${YELLOW}=== PROMOTION VARIATION TESTS ===${NC}"

run_test "Promotion with Capture" \
    "e2 e4
d7 d5
e4 d5
c7 c6
d5 c6
b7 b6
c6 b7
b8 a6
b7 a8
Q" \
    "Black's turn|Black to move" \
    "Pawn promotes by capturing"

run_test "Both Sides Promote" \
    "a2 a4
h7 h5
a4 a5
h5 h4
a5 a6
h4 h3
a6 b7
h3 g2
b7 a8
Q
g2 h1
Q" \
    "White's turn|White to move" \
    "Both players promote pawns"

# ============================================
# MATERIAL IMBALANCE
# ============================================
echo ""
echo -e "${YELLOW}=== MATERIAL IMBALANCE TESTS ===${NC}"

run_test "Queen vs Multiple Pieces" \
    "e2 e4
e7 e5
d1 h5
b8 c6
h5 e5
g8 f6
e5 e7
f8 e7" \
    "White's turn|White to move" \
    "Early queen trade"

run_test "Rook and Pawns vs Knights and Bishops" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
b1 c3
g8 f6
d2 d3
d7 d6
c1 g5
c8 g4
c3 d5
g4 f3
g2 f3
c6 d4
c2 c3
d4 f3
d1 f3
c5 d4
c3 d4
f6 d7
d5 f6
g7 f6
f3 f6
d8 f6
g5 f6
d7 f6" \
    "White's turn|White to move" \
    "Unbalanced material distribution"

# ============================================
# EDGE RANK/FILE MOVES
# ============================================
echo ""
echo -e "${YELLOW}=== EDGE OF BOARD TESTS ===${NC}"

run_test "Pieces on First Rank" \
    "a2 a3
a7 a6
a1 a2
a8 a7" \
    "White's turn|White to move" \
    "Rook moves on first rank"

run_test "Pieces on Eighth Rank" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 d8" \
    "White's turn|White to move" \
    "Queen returns to eighth rank"

run_test "Pieces on A-File" \
    "a2 a4
a7 a5" \
    "White's turn|White to move" \
    "Pawns advance on a-file"

run_test "Pieces on H-File" \
    "h2 h4
h7 h5" \
    "White's turn|White to move" \
    "Pawns advance on h-file"

# ============================================
# MAXIMUM MOVE DISTANCES
# ============================================
echo ""
echo -e "${YELLOW}=== MAXIMUM MOVE DISTANCE TESTS ===${NC}"

run_test "Queen Maximum Distance" \
    "e2 e4
e7 e5
d1 h5" \
    "Black's turn|Black to move" \
    "Queen moves maximum diagonal distance"

run_test "Bishop Maximum Distance" \
    "e2 e4
e7 e5
f1 a6" \
    "Black's turn|Black to move" \
    "Bishop moves full diagonal"

run_test "Rook Maximum Distance" \
    "a2 a4
a7 a5
a1 a3
a8 a6
a3 h3" \
    "Black's turn|Black to move" \
    "Rook moves full rank"

# ============================================
# PIECE MOBILITY TESTS
# ============================================
echo ""
echo -e "${YELLOW}=== PIECE MOBILITY TESTS ===${NC}"

run_test "Trapped Knight" \
    "e2 e4
d7 d5
e4 d5
d8 d5
b1 c3
d5 e5
f1 e2
e5 e6" \
    "White's turn|White to move" \
    "Knight with limited mobility"

run_test "Trapped Bishop" \
    "e2 e3
d7 d5
f1 d3
c8 f5
d3 f5" \
    "Black's turn|Black to move" \
    "Bishop capture and trap potential"

run_test "Active Pieces" \
    "e2 e4
e7 e5
g1 f3
b8 c6
f1 c4
f8 c5
O-O
g8 f6
d2 d4
e5 d4
e4 e5" \
    "Black's turn|Black to move" \
    "Active piece placement"

# ============================================
# SUMMARY
# ============================================
echo ""
echo "=========================================="
echo "          TEST SUITE COMPLETE"
echo "=========================================="
echo ""
echo "Total Tests Run: ${TOTAL_TESTS}"
echo -e "${GREEN}Tests Passed: ${TESTS_PASSED}${NC}"
echo -e "${RED}Tests Failed: ${TESTS_FAILED}${NC}"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    PERCENTAGE=100
    echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${GREEN}   ✓ ALL TESTS PASSED! (100%)${NC}"
    echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    echo "Your chess implementation is working correctly!"
    exit 0
else
    PERCENTAGE=$((TESTS_PASSED * 100 / TOTAL_TESTS))
    echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${RED}   ✗ SOME TESTS FAILED${NC}"
    echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    echo "Success Rate: ${PERCENTAGE}%"
    echo ""
    echo "Please review the failed tests above and fix the issues."
    echo "Run the script again after making corrections."
    exit 1
fi \
    "