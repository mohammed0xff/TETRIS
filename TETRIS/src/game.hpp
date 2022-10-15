#pragma once

#include <stdlib.h>
#include <time.h>
#include <thread>
#include <iostream>
#include <vector>
#include <deque>
#include <list>

#include "SDL_mixer.h"

#include "displayManager.hpp"


class Game {
	
	enum class GameState {START_MENU, IN_GAME, GAME_OVER, QUIT};

public:

	Game();
	~Game();

	bool init();
	void stateLoop();

	static bool checkCollision(const Tetramino* tetramino, const Board& m_board, int xPos = 0, int yPos = 0);

private:
	void gameLoop();
	void gameOver();
	void reset();
	void handleInput();
	void hardDrop();
	int clearCompleteRows();
	int getPointsForLinesCount(int clearedRows);

private:
	GameState currGameState;

	DisplayManager displayManager;
	Board m_board;

	Tetramino* curr_Tetramino;
 	std::list<Tetramino*> next_Tetraminos;
	Tetramino* held_Tetramino;
	
	bool lost;
	bool holdChance;
	bool hardDropping;

	int finalScore;
	int speed;
	int speedCounter;
	int level;
	int score;
	int lineCount;

	std::string scoreText;
	std::string levelText;
	std::string lineCountText;

};
