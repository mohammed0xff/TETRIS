#pragma once

#include <vector>
#include <deque>

#include "shapes.h"
#include "universals.hpp"
#include "tetramino.hpp"


class Board
{
public:
	Board();
	~Board();

	void placeTetramino(const Tetramino*);
	blockShape blockAt(int x, int y) const;
	void removeLine(int x);
	void clear();

private:
	std::deque<std::vector<blockShape>> m_blocks;
};