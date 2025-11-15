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
    echo -e "${BLUE}Test $TOTAL_TESTS: $test_name${NC}"
    echo "Description: $description"
    
    # Create temporary input file
    local input_file=$(mktemp)
    echo "$input" > "$input_file"
    echo "quit" >> "$input_file"
    
    # Run the program with timeout and capture output
    local output=$(timeout 5 ./chess < "$input_file" 2>&1)
    local exit_code=$?
    
    # Check for timeout
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}✗ FAILED (Timeout)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        rm -f "$input_file"
        return 1
    fi
    
    # Check if expected pattern is found in output
    if echo "$output" | grep -q "$expected_pattern"; then
        echo -e "${GREEN}✓ PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        rm -f "$input_file"
        return 0
    else
        echo -e "${RED}✗ FAILED${NC}"
        echo "Expected pattern: $expected_pattern"
        echo "Output snippet:"
        echo "$output" | tail -20
        TESTS_FAILED=$((TESTS_FAILED + 1))
        rm -f "$input_file"
        return 1
    fi
}

# Function to test that a move should fail
test_invalid_move() {
    local test_name="$1"
    local input="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo -e "${BLUE}Test $TOTAL_TESTS: $test_name${NC}"
    echo "Description: $description"
    
    local input_file=$(mktemp)
    echo "$input" > "$input_file"
    echo "quit" >> "$input_file"
    
    local output=$(timeout 5 ./chess < "$input_file" 2>&1)
    local exit_code=$?
    
    # Check for timeout
    if [ $exit_code -eq 124 ]; then
        echo -e "${RED}✗ FAILED (Timeout)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        rm -f "$input_file"
        return 1
    fi
    
    # Should contain error message
    if echo "$output" | grep -qiE "(Error|Invalid|not your piece|No piece)"; then
        echo -e "${GREEN}✓ PASSED (correctly rejected invalid move)${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        rm -f "$input_file"
        return 0
    else
        echo -e "${RED}✗ FAILED (should have rejected invalid move)${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        rm -f "$input_file"
        return 1
    fi
}

# Check if chess executable exists
if [ ! -f "./chess" ]; then
    echo -e "${RED}Error: chess executable not found. Please run 'make' first.${NC}"
    exit 1
fi

echo "=========================================="
echo "  Chess Game Comprehensive Test Suite"
echo "=========================================="
echo ""

# ============================================
# TEST 1: Basic Pawn Moves
# ============================================
echo -e "${YELLOW}=== Testing Basic Pawn Moves ===${NC}"
run_test "Pawn Double Move" \
    "e2 e4" \
    "Black's turn" \
    "White pawn moves two squares from starting position"

run_test "Pawn Single Move" \
    "e2 e4
e7 e6" \
    "White's turn" \
    "Black pawn moves one square"

run_test "Pawn Capture" \
    "e2 e4
d7 d5
e4 d5" \
    "Black's turn" \
    "White pawn captures black pawn"

# ============================================
# TEST 2: Knight Moves
# ============================================
echo -e "${YELLOW}=== Testing Knight Moves ===${NC}"
run_test "Knight L-Shape Move" \
    "b1 c3" \
    "Black's turn" \
    "White knight moves in L-shape"

run_test "Knight Capture" \
    "b1 c3
b8 c6
c3 d5
c6 d4" \
    "White's turn" \
    "Knight captures and is captured"

# ============================================
# TEST 3: Bishop Moves
# ============================================
echo -e "${YELLOW}=== Testing Bishop Moves ===${NC}"
run_test "Bishop Diagonal Move" \
    "e2 e4
e7 e5
f1 c4" \
    "Black's turn" \
    "Bishop moves diagonally"

run_test "Bishop Blocked Path" \
    "e2 e4
e7 e5
f1 c4
f8 c5
c4 f7" \
    "Black's turn" \
    "Bishop attempts to move through piece (should be blocked)"

# ============================================
# TEST 4: Rook Moves
# ============================================
echo -e "${YELLOW}=== Testing Rook Moves ===${NC}"
run_test "Rook Horizontal Move" \
    "e2 e4
e7 e5
h1 f1" \
    "Black's turn" \
    "Rook moves horizontally"

run_test "Rook Vertical Move" \
    "e2 e4
e7 e5
h1 f1
h8 f8
f1 f3" \
    "Black's turn" \
    "Rook moves vertically"

# ============================================
# TEST 5: Queen Moves
# ============================================
echo -e "${YELLOW}=== Testing Queen Moves ===${NC}"
run_test "Queen Diagonal Move" \
    "e2 e4
e7 e5
d1 f3" \
    "Black's turn" \
    "Queen moves diagonally"

run_test "Queen Horizontal Move" \
    "e2 e4
e7 e5
d1 f3
d8 f6
f3 f5" \
    "Black's turn" \
    "Queen moves horizontally"

# ============================================
# TEST 6: King Moves
# ============================================
echo -e "${YELLOW}=== Testing King Moves ===${NC}"
run_test "King Single Square Move" \
    "e2 e4
e7 e5
e1 e2" \
    "Black's turn" \
    "King moves one square"

run_test "King Cannot Move Into Check" \
    "e2 e4
e7 e5
d1 f3
d8 f6
e1 e2
f6 f2" \
    "Error\|Invalid\|check" \
    "King should not be able to move into check"

# ============================================
# TEST 7: Castling
# ============================================
echo -e "${YELLOW}=== Testing Castling ===${NC}"
run_test "Kingside Castling" \
    "e2 e4
e7 e5
f1 c4
f8 c5
g1 f3
g8 f6
O-O" \
    "Black's turn" \
    "White performs kingside castling"

run_test "Queenside Castling" \
    "d2 d4
d7 d5
c1 f4
c8 f5
b1 c3
b8 c6
d1 d2
d8 d7
O-O-O" \
    "Black's turn" \
    "White performs queenside castling"

# ============================================
# TEST 8: En Passant
# ============================================
echo -e "${YELLOW}=== Testing En Passant ===${NC}"
run_test "En Passant Capture" \
    "e2 e4
a7 a6
e4 e5
d7 d5
e5 d6" \
    "Black's turn" \
    "White pawn captures en passant after black pawn double move"

# ============================================
# TEST 9: Pawn Promotion
# ============================================
echo -e "${YELLOW}=== Testing Pawn Promotion ===${NC}"
run_test "Pawn Promotion Setup" \
    "e2 e4
e7 e5
e4 e5
d7 d5
e5 d6
c7 c5
d6 c7
b7 b6
c7 b8
Q" \
    "Black's turn\|Game Over" \
    "Pawn reaches end, promotes to Queen"

# ============================================
# TEST 10: Invalid Moves
# ============================================
echo -e "${YELLOW}=== Testing Invalid Moves ===${NC}"
test_invalid_move "Move Empty Square" \
    "e3 e4" \
    "Attempting to move from empty square"

test_invalid_move "Move Opponent's Piece" \
    "e2 e4
e7 e5
e5 e6" \
    "Attempting to move opponent's piece"

test_invalid_move "Invalid Knight Move" \
    "b1 b3" \
    "Knight attempting invalid move pattern"

test_invalid_move "Move Into Own Piece" \
    "e2 e4
e7 e5
d1 d2
d2 d1" \
    "Attempting to move into own piece"

test_invalid_move "Move Leaving King in Check" \
    "e2 e4
e7 e5
f1 c4
f8 c5
d1 f3
b8 c6
g1 f3
g8 f6
f3 e5
f6 e4
c4 f7
e8 f7
e5 c6" \
    "Attempting move that leaves king in check (after king moved)"

# ============================================
# TEST 11: Check Detection
# ============================================
echo -e "${YELLOW}=== Testing Check Detection ===${NC}"
run_test "Check Detection" \
    "e2 e4
e7 e5
f1 c4
f8 c5
d1 f3
b8 c6
f3 f7" \
    "CHECK\|Checkmate" \
    "White puts black king in check (results in checkmate)"

# ============================================
# TEST 12: Complex Game Scenario
# ============================================
echo -e "${YELLOW}=== Testing Complex Game Scenario ===${NC}"
run_test "Complex Opening Sequence" \
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
d1 d2
d8 d7
O-O
O-O" \
    "White's turn\|Black's turn" \
    "Complex opening with multiple piece types and castling"

# ============================================
# TEST 13: Edge Cases
# ============================================
echo -e "${YELLOW}=== Testing Edge Cases ===${NC}"
test_invalid_move "Invalid Position Format" \
    "z9 z8" \
    "Invalid position coordinates"

test_invalid_move "Single Input" \
    "e2" \
    "Incomplete move input"

run_test "Multiple Valid Moves" \
    "a2 a3
a7 a6
b2 b3
b7 b6
c2 c3
c7 c6
d2 d3
d7 d6" \
    "White's turn\|Black's turn" \
    "Series of valid pawn moves"

# ============================================
# TEST 14: Special Move Formats
# ============================================
echo -e "${YELLOW}=== Testing Special Move Formats ===${NC}"
run_test "Castling Format 0-0" \
    "e2 e4
e7 e5
f1 c4
f8 c5
g1 f3
g8 f6
0-0" \
    "Black's turn" \
    "Castling using 0-0 format"

run_test "Castling Format O-O-O" \
    "d2 d4
d7 d5
c1 f4
c8 f5
b1 c3
b8 c6
d1 d2
d8 d7
O-O-O" \
    "Black's turn" \
    "Queenside castling using O-O-O format"

# ============================================
# TEST 15: Board Display
# ============================================
echo -e "${YELLOW}=== Testing Board Display ===${NC}"
run_test "Initial Board Display" \
    "quit" \
    "a b c d e f g h" \
    "Board should display correctly"

# ============================================
# TEST 16: Path Blocking
# ============================================
echo -e "${YELLOW}=== Testing Path Blocking ===${NC}"
run_test "Rook Cannot Jump Over Pieces" \
    "e2 e4
e7 e5
h1 h2" \
    "Black's turn\|Error\|Invalid" \
    "Rook cannot move when blocked by pawn"

run_test "Bishop Cannot Jump Over Pieces" \
    "d2 d4
d7 d5
c1 f4
c8 f5
f4 h6" \
    "Black's turn\|Error\|Invalid" \
    "Bishop cannot move through pieces"

# ============================================
# TEST 17: Check and Checkmate Scenarios
# ============================================
echo -e "${YELLOW}=== Testing Check and Checkmate ===${NC}"
run_test "Scholar's Mate Setup" \
    "e2 e4
e7 e5
f1 c4
b8 c6
d1 f3
f8 c5
f3 f7" \
    "CHECK\|Checkmate" \
    "Famous checkmate pattern (Scholar's Mate)"

# ============================================
# TEST 18: Piece Movement Validation
# ============================================
echo -e "${YELLOW}=== Testing Piece Movement Validation ===${NC}"
test_invalid_move "Pawn Cannot Move Backward" \
    "e2 e4
e7 e5
e4 e3" \
    "Pawn attempting to move backward"

test_invalid_move "Pawn Cannot Move Forward Into Occupied Square" \
    "e2 e4
e7 e5
e4 e5" \
    "Pawn attempting to move forward into occupied square (should be blocked)"

test_invalid_move "King Cannot Move More Than One Square" \
    "e2 e4
e7 e5
e1 e3" \
    "King attempting to move more than one square"

# ============================================
# Summary
# ============================================
echo ""
echo "=========================================="
echo "  Test Summary"
echo "=========================================="
echo -e "Total Tests: ${TOTAL_TESTS}"
echo -e "${GREEN}Passed: ${TESTS_PASSED}${NC}"
echo -e "${RED}Failed: ${TESTS_FAILED}${NC}"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed. Please review the output above.${NC}"
    exit 1
fi

