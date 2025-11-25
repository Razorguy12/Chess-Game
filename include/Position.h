#ifndef POSITION_H
#define POSITION_H

#include <iostream>

/**
 * @class Position
 * @brief Represents a position on the chess board using row and column coordinates
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
     */
    bool isValid() const
    {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    /**
     * @brief Equality comparison operator
     * @param other Position to compare with
     * @return true if both positions have the same row and column
     */
    bool operator==(const Position &other) const
    {
        return row == other.row && col == other.col;
    }

    /**
     * @brief Inequality comparison operator
     * @param other Position to compare with
     * @return true if positions are different
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
     */
    friend std::ostream &operator<<(std::ostream &os, const Position &pos)
    {
        os << (char)('a' + pos.col) << (8 - pos.row);
        return os;
    }
};

#endif