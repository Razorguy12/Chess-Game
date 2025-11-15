#ifndef POSITION_H
#define POSITION_H

#include <iostream>

class Position {
private:
    int row;
    int col;

public:
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    int getRow() const { return row; }
    int getCol() const { return col; }
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    
    // Operator overloading
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << (char)('a' + pos.col) << (8 - pos.row);
        return os;
    }
};

#endif