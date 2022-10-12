#include <iostream>
#include <memory>
#include <utility>

#include "tetramino.hpp"
#include "game.h"



Game::Game() 
	: speed(GAME_SPEED)
{
	currGameState = Game::GameState::IN_GAME;
}

Game::~Game() {

}


bool Game::init() {

	// Music
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* theme = Mix_LoadMUS("assets/theme.wav");
	if (theme == nullptr) {
		std::cerr << "Can't open `assets/theme.wav`";
		return false;
	}
	// Mix_PlayMusic(theme, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
	return displayManager.init();
}


void Game::stateLoop()
{
	while (currGameState != GameState::QUIT)
	{
		switch (currGameState)
		{
		case GameState::IN_GAME:
			gameLoop();
			break;
		case GameState::GAME_OVER:
			gameOver();
			break;
		}
	}
}

void Game::handleInput() {

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			lost = true;
			currGameState = GameState::QUIT;
		}
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				curr_Tetramino->rotateRight();
				if (checkCollision(curr_Tetramino, m_board))
				{
					// rotate to the opposite direction.
					curr_Tetramino->rotateLeft(); 
				}
				break;

			case SDLK_DOWN:
				if (not checkCollision(curr_Tetramino, m_board, curr_Tetramino->xPos() + 1, curr_Tetramino->yPos()))
				{
					curr_Tetramino->moveDown();
				}
				break;

			case SDLK_RIGHT:
				if (not checkCollision(curr_Tetramino, m_board, curr_Tetramino->xPos(), curr_Tetramino->yPos() + 1))
				{
					curr_Tetramino->moveRight();
				}
				break;

			case SDLK_LEFT:
				if (not checkCollision(curr_Tetramino, m_board, curr_Tetramino->xPos(), curr_Tetramino->yPos() - 1))
				{
					curr_Tetramino->moveLeft();
				}
				break;

			case SDLK_SPACE:
				hardDrop();
				break;

			case SDLK_c:
				if (held_Tetramino != nullptr) {
					if (holdChance) {
						Tetramino* temp = held_Tetramino;
						held_Tetramino = curr_Tetramino;
						held_Tetramino->reset();
						curr_Tetramino = temp;
						holdChance = false;
					};
				}
				else {
					held_Tetramino = curr_Tetramino;
					held_Tetramino->reset();
					curr_Tetramino = next_Tetraminos.front();
					next_Tetraminos.pop_front();
					next_Tetraminos.push_back(
						new Tetramino(Shapes::getDifferentShape(curr_Tetramino->shape()))
					);
				}
				break;
			case SDLK_ESCAPE:
				lost = true;
				break;
			}
		}
	}

}


void Game::gameLoop()
{
	// init Tetraminos
	curr_Tetramino = new Tetramino();
	next_Tetraminos.push_back(
		new Tetramino(Shapes::getDifferentShape(curr_Tetramino->shape()))
	);
	for (size_t i = 0; i < 2; i++)
	{
		next_Tetraminos.push_back( 
			new Tetramino(
				Shapes::getDifferentShape(next_Tetraminos.back()->shape())
			)
		);
	}
	held_Tetramino = nullptr;

	// init game variables
	lost = false;
	holdChance = true;
	finalScore = 0;
	score = 0;
	level = 1;
	lineCount = 0;
	hardDropping = false;
	speed = GAME_SPEED;


	Uint32 timestamp = SDL_GetTicks();
	Uint32 frameStart;
	Uint32 frameEnd;
	Uint32 elapsedTime;
	int frame_count = 0;

	while (!lost && currGameState == Game::GameState::IN_GAME)
	{
		// Time start
		frameStart = SDL_GetTicks();

		// Rendering the game screen
		displayManager.clear();
		displayManager.drawBoard(m_board);
		displayManager.drawTetramino(curr_Tetramino, 255);
		displayManager.drawShadow(curr_Tetramino, m_board);
		displayManager.drawNext(next_Tetraminos);
		displayManager.drawHeld(held_Tetramino);

		// game stats
		scoreText = std::to_string(score);
		levelText = std::to_string(level);
		lineCountText = std::to_string(lineCount);

		displayManager.drawText(scoreText, 150 - std::strlen(scoreText.c_str()) * 10, 410, std::strlen(scoreText.c_str()) * 15, 40);
		displayManager.drawText(levelText, 130, 530, std::strlen(levelText.c_str()) * 15, 40);
		displayManager.drawText(lineCountText, 130, 655, std::strlen(levelText.c_str()) * 15, 40);

		displayManager.drawUpdate();

		// Time end
		frameEnd = SDL_GetTicks();

		// Calculate duration 
		elapsedTime = frameEnd - frameStart;
		frame_count++;
		handleInput();
		
		// Moving tetramino one step down every {speed} ms
		if (frameEnd - timestamp >= speed || hardDropping) {

			if (not checkCollision(curr_Tetramino, m_board, curr_Tetramino->xPos() + 1, curr_Tetramino->yPos()))
			{
				curr_Tetramino->moveDown();
			}
			else
			{
				holdChance = true;
				m_board.placeTetramino(curr_Tetramino);

				int clearedRows = clearCompleteRows();
				if (clearedRows > 0) {
					score += level * getPointsForLinesCount(clearedRows);
					// level ups each 10 lines.
					level = lineCount / 10 + 1;
					speed = GAME_SPEED - level * 100;
				}

				delete curr_Tetramino;
				curr_Tetramino = nullptr;
				curr_Tetramino = next_Tetraminos.front();
				next_Tetraminos.pop_front();
				next_Tetraminos.push_back(new Tetramino(Shapes::getDifferentShape(curr_Tetramino->shape())));

				lost = checkCollision(curr_Tetramino, m_board);
			}
			hardDropping = false;
			frame_count = 0;
			timestamp = frameEnd;
		}

		// If the time for this frame is too small 
		// apply some delay to correct the frame rate.
		if (elapsedTime < MsPerFrame) {
			SDL_Delay(MsPerFrame - elapsedTime);
		}
	}
	finalScore = score;
	if (currGameState != GameState::QUIT)
		currGameState = GameState::GAME_OVER;

	reset();
}

int Game::getPointsForLinesCount(int clearedRows) {
	switch (clearedRows)
	{
	case 1:
		return 40;
	case 2:
		return 100;
	case 3:
		return 300;
	case 4:
		return 1200;
	default:
		return 0;
	}
}


void Game::reset() {
	if (held_Tetramino)
		delete held_Tetramino;
	held_Tetramino = nullptr;
	
	delete curr_Tetramino;
	curr_Tetramino = nullptr;

	for (auto i : next_Tetraminos) {
		delete i;
	}
	next_Tetraminos.clear();

	m_board.clear();
}


void Game::gameOver()
{
	while (currGameState == GameState::GAME_OVER)
	{
		displayManager.clear();
		displayManager.drawText("GAME OVER", SCREEN_WIDTH / 8, 30, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 10);
		std::string scoreText = "YOUR SCORE: " + std::to_string(finalScore);
		displayManager.drawText(scoreText, SCREEN_WIDTH / 4, 200, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 20);
		displayManager.drawText("press SPACE to restart", SCREEN_WIDTH / 8, 300, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 15);
		displayManager.drawText("press ESCAPE to exit", SCREEN_WIDTH / 8, 400, SCREEN_WIDTH / 1.25, SCREEN_HEIGHT / 15);
		displayManager.drawUpdate();

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				currGameState = GameState::QUIT;
				break;
			}
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_SPACE:
					currGameState = GameState::IN_GAME;
					break;
				case SDLK_ESCAPE:
					currGameState = GameState::QUIT;
					break;
				}
			}
		}
	}
}


bool Game::checkCollision(const Tetramino* tetramino, const Board& m_board, int xPos, int yPos)
{
	if (xPos == 0 && yPos == 0) {
		xPos = tetramino->xPos();
		yPos = tetramino->yPos();
	}

	std::vector<std::vector<bool>> Tetramino = tetramino->blocks();

	for (int x = 0; x < Tetramino.size(); x++)
		for (int y = 0; y < Tetramino[0].size(); y++)
		{
			if (Tetramino[x][y] == 0) continue;

			if (xPos + x < 0 || xPos + x >= BOARD_HEIGHT)
				return true;
			if (xPos + x < 0 || yPos + y >= BOARD_WIDTH)
				return true;

			blockShape block = m_board.blockAt(xPos + x, yPos + y);
			if (block != blockShape::Empty)
				return true;
		}
	return false;
}

void Game::hardDrop()
{
	while (not checkCollision(curr_Tetramino, m_board, curr_Tetramino->xPos() + 1, curr_Tetramino->yPos())) {
		curr_Tetramino->moveDown();
	}
	hardDropping = true;
}

int Game::clearCompleteRows()
{
	int clearedRows = 0;
	for (int x = 0; x < BOARD_HEIGHT; x++)
	{
		// check if the row is complete
		bool lineComplete = true;
		for (int y = 0; y < BOARD_WIDTH; y++)
		{
			blockShape block = m_board.blockAt(x, y);
			if (block == blockShape::Empty) { lineComplete = false; break; }
		}
		if (lineComplete)
		{
			m_board.removeLine(x);
			clearedRows++;
			lineCount++;
		}
	}
	return clearedRows;
}