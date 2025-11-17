/**
 * @file Position.h
 * @brief Defines the Position class for representing chess board coordinates
 *
 * @details This header file contains the Position class which represents a position
 * on the chess board using row and column indices. The class provides methods to
 * validate positions, access coordinates, and supports operator overloading for
 * comparison and output. Positions are stored internally as 0-based indices (0-7)
 * for both row and column, where row 0 is the top of the board (rank 8 in chess
 * notation) and column 0 is the left side (file 'a' in chess notation).
 */

#ifndef POSITION_H
#define POSITION_H

#include <iostream>

/**
 * @class Position
 * @brief Represents a position on the chess board using row and column coordinates
 *
 * @details The Position class encapsulates a location on the 8x8 chess board.
 * Internally, it uses 0-based indexing where row 0 corresponds to rank 8 (top row)
 * and column 0 corresponds to file 'a' (leftmost column). The class provides
 * validation to ensure positions are within board bounds, supports equality
 * comparison, and can be output in standard chess notation (e.g., "e4").
 * The class uses composition to store row and column as private members, ensuring
 * data encapsulation and controlled access through getter and setter methods.
 */
class Position
{
private:
    int row;
    int col;

public:
    /**
     * @brief Constructs a Position with specified row and column
     * @param r Row index (0-7, where 0 is top row/rank 8)
     * @param c Column index (0-7, where 0 is left column/file 'a')
     * @details Default constructor initializes to (0,0) which represents square a8
     */
    Position(int r = 0, int c = 0) : row(r), col(c) {}

    /**
     * @brief Gets the row index
     * @return Row index (0-7)
     */
    int getRow() const { return row; }

    /**
     * @brief Gets the column index
     * @return Column index (0-7)
     */
    int getCol() const { return col; }

    /**
     * @brief Sets the row index
     * @param r Row index (0-7)
     */
    void setRow(int r) { row = r; }

    /**
     * @brief Sets the column index
     * @param c Column index (0-7)
     */
    void setCol(int c) { col = c; }

    /**
     * @brief Validates if the position is within board bounds
     * @return true if row and column are both between 0 and 7, false otherwise
     * @details Checks that both coordinates are valid for an 8x8 chess board
     */
    bool isValid() const
    {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    /**
     * @brief Equality comparison operator
     * @param other Position to compare with
     * @return true if both positions have the same row and column
     * @details Compares row and column values for equality
     */
    bool operator==(const Position &other) const
    {
        return row == other.row && col == other.col;
    }

    /**
     * @brief Inequality comparison operator
     * @param other Position to compare with
     * @return true if positions are different
     * @details Uses the equality operator for comparison
     */
    bool operator!=(const Position &other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Output stream operator for chess notation
     * @param os Output stream reference
     * @param pos Position to output
     * @return Reference to the output stream
     * @details Converts internal 0-based coordinates to standard chess notation
     * (e.g., row 4, col 4 becomes "e4"). Column is converted to letter (a-h),
     * row is converted to number (1-8) where row 0 = rank 8.
     */
    friend std::ostream &operator<<(std::ostream &os, const Position &pos)
    {
        os << (char)('a' + pos.col) << (8 - pos.row);
        return os;
    }
};

#endif