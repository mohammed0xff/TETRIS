#include "board.h"
#include "tetramino.hpp"

Board::Board()
{
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		m_blocks.push_back(
			std::vector<blockShape>(BOARD_WIDTH, blockShape::Empty)
		);
	}
}

Board::~Board() {

}


void Board::placeTetramino(const Tetramino* tetramino)
{
	std::vector<std::vector<bool>> tet_blocks = tetramino->blocks();
	blockShape blockShape = tetramino->shape();
	for (int x = 0; x < tet_blocks.size(); x++)
		for (int y = 0; y < tet_blocks[0].size(); y++)
			if (tet_blocks[x][y] == 1)
				m_blocks[tetramino->xPos() + x][tetramino->yPos() + y] = blockShape;
}

blockShape Board::blockAt(int x, int y) const
{
	return m_blocks[x][y];
}

void Board::removeLine(int i) {
	m_blocks.erase(m_blocks.begin() + i);
	m_blocks.push_front(std::vector<blockShape>(BOARD_WIDTH, blockShape::Empty));
}

void Board::clear() {
	for (int x = 0; x < m_blocks.size(); x++)
		for (int y = 0; y < m_blocks[0].size(); y++)
			m_blocks[x][y] = blockShape::Empty;
}
