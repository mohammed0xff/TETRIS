#include "tetramino.hpp"


Tetramino::Tetramino(blockShape shape)
    : x_board(TETRAMINO_START_X)
    , y_board(TETRAMINO_START_Y)
    , m_shape(shape)
{
    if (shape == blockShape::Empty) {
        m_shape = static_cast<blockShape> ((std::rand() % 6) + 1); 
    }
    m_blocks = Shapes::getShapesBlock(m_shape);
}

Tetramino::~Tetramino()
{}


void Tetramino::rotateLeft() {

    // creating M * N matrix to perform 90 deg rotation.
    std::vector<std::vector<bool>> newRotation(
        m_blocks[0].size(), std::vector<bool>(m_blocks.size(), 0)
    );

    // new rotation pointers 
    int x = newRotation.size()-1;
    int y = 0;

    for (size_t i = 0; i < m_blocks.size(); i++)
    {
        for (size_t j = 0; j < m_blocks[0].size(); j++)
        {
            newRotation[x--][y] = m_blocks[i][j];
        }
        x = newRotation.size() - 1;
        y++;
    }

    m_blocks = newRotation;
}

void Tetramino::rotateRight() {

    std::vector<std::vector<bool>> newRotation(
        m_blocks[0].size(), std::vector<bool>(m_blocks.size(), 0)
    );
    int x = 0;
    int y = newRotation[0].size() - 1;

    for (size_t i = 0; i < m_blocks.size(); i++)
    {
        for (size_t j = 0; j < m_blocks[0].size(); j++)
        {
            newRotation[x++][y] = m_blocks[i][j];
        }
        x = 0;
        y--;
    }
    m_blocks = newRotation;
}

void Tetramino::reset() {
    x_board = TETRAMINO_START_X;
    y_board = TETRAMINO_START_Y;
    // get default rotation.
    m_blocks = Shapes::getShapesBlock(m_shape);
}

blockShape Tetramino::shape() const {
    return m_shape;
}

const std::vector <std::vector<bool>>& Tetramino::blocks() const {
    return m_blocks;
}

int Tetramino::xPos() const {
    return x_board;
}

int Tetramino::yPos() const {
    return y_board;
}

void Tetramino::moveRight() {
    y_board++;
}

void Tetramino::moveLeft() {
    y_board--;
}

void Tetramino::moveDown() {
    x_board++;
}