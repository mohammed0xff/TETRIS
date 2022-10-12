#pragma once
#include <random>
#include <vector>
#include <deque>

#include "shapes.hpp"
#include "universals.hpp"


class Tetramino
{
public:
    Tetramino(blockShape = blockShape::Empty);
    ~Tetramino();

    void rotateLeft();
    void rotateRight();
    void moveRight();
    void moveLeft();
    void moveDown();
    void reset();

    int xPos() const;
    int yPos() const;
    blockShape shape() const;
    const std::vector<std::vector<bool>>& blocks() const;
    
private:
    int x_board; // goes with rows
    int y_board; // goes with columns
    // board[x][y] ---> x block down & y block right.
    std::vector<std::vector<bool>> m_blocks;
    blockShape m_shape;
};
