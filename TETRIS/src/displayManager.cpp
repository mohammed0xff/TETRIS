#include "displayManager.hpp"
#include "shapes.hpp"
#include "tetramino.hpp"
#include "game.hpp"



DisplayManager::DisplayManager()
{
	rect.x = rect.y = rect.w = rect.h = 0;
	textTexture = NULL;
	textSurface = NULL;
	textColor = { 255, 255, 255, 255 };
	
}

DisplayManager::~DisplayManager() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
}


bool DisplayManager::init() {

	// Initializing SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not be initialized.\n";
		std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	// Initializing SDL_IMAGE
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cerr << "SDL_image could not be initialized.\n";
		std::cerr << "SDL_image Error : % s\n", IMG_GetError(), "\n";;
		return false;
	}

	// Initializing SDL_TTF
	if (TTF_Init() == -1) {
		std::cerr << "SDL_ttf could not be initializeed.\n";
		std::cerr << "SDL_ttf Error : % s\n", TTF_GetError(), "\n";;
		return false;
	}

	// Creating window
	m_window = SDL_CreateWindow("TETRIS", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		SDL_WINDOW_SHOWN);

	if (m_window == nullptr) {
		std::cerr << "Error creating window..\n";
		std::cerr << SDL_GetError() << "\n";
		return false;
	}

	// Creating renderer for m_window
	m_renderer = SDL_CreateRenderer(m_window, 0, 0);
	if (m_renderer == nullptr) {
		std::cerr << "Error creating renderer..\n";
		std::cerr << SDL_GetError() << "\n";
		return false;
	}
	// loading assets
	if (!loadGameAssets()) return false;

	return true;
}

bool DisplayManager::loadGameAssets() {

	// to make sure we have all assets we need.
	bool allIsWell = true;

	// loading shapes images
	m_ShapesTextures.push_back(
		loadTexture("assets/empty.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/I.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/O.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/J.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/T.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/L.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/Z.png")
	);
	m_ShapesTextures.push_back(
		loadTexture("assets/S.png")
	);

	for (auto i : m_ShapesTextures) 
		allIsWell &= i != nullptr;
	
	// loading background image.
	bg_texture = loadTexture("assets/bg.png");
	allIsWell &= bg_texture != nullptr;
	
	// loading font for text rendering.
	font = TTF_OpenFont("assets/pixeled.ttf", 64);
	allIsWell &= bg_texture != nullptr;

	return allIsWell;
}



// Makes a call to RenderPresent
void DisplayManager::drawUpdate()
{
	SDL_RenderPresent(m_renderer);
}

// Draws the board
void DisplayManager::drawBoard(const Board& m_board)
{
	rect.x = BOARD_START_POSX;
	rect.y = BOARD_START_POSY;
	rect.w = rect.h = BLOCK_SIZE;

	for (int x = 0; x < BOARD_HEIGHT; x++)
	{
		for (int y = 0; y < BOARD_WIDTH; y++)
		{
			blockShape shape = m_board.blockAt(x, y);
			SDL_RenderCopy(m_renderer, m_ShapesTextures[static_cast<int>(shape)], NULL, &rect);
			rect.x += BLOCK_SIZE + BLOCK_OFFSET;
			rect.w = rect.h = BLOCK_SIZE;
		}
		rect.x = BOARD_START_POSX; 
		rect.y += BLOCK_SIZE + BLOCK_OFFSET;
	}
}


void DisplayManager::drawTetramino(const Tetramino* tetramino, Uint8 alpha, int xBoard, int yBoard )
{
	if (xBoard == -1 && yBoard == -1) {
		xBoard = tetramino->xPos();
		yBoard = tetramino->yPos();
	}

	// Getting index of shape texture.
	blockShape shape = tetramino->shape();
	int textureIdx = static_cast<int>(shape);
	// Setting texture alpha.
	SDL_SetTextureAlphaMod(m_ShapesTextures[textureIdx], alpha);

	std::vector<std::vector<bool>> TetBlocks = tetramino->blocks();

	rect.x = BOARD_START_POSX + yBoard * BLOCK_SIZE + (yBoard * BLOCK_OFFSET);
	rect.y = BOARD_START_POSY + xBoard * BLOCK_SIZE + (xBoard * BLOCK_OFFSET);;
	rect.w = rect.h = BLOCK_SIZE;
	for (int x = 0; x < TetBlocks.size(); x++)
	{
		for (int y = 0; y < TetBlocks[0].size(); y++) {

			if (TetBlocks[x][y] == 1)
			{
				SDL_RenderCopy(m_renderer, m_ShapesTextures[textureIdx], NULL, &rect);
			}
			rect.x += BLOCK_SIZE + BLOCK_OFFSET;
		}
		rect.x = BOARD_START_POSX + yBoard * BLOCK_SIZE + (yBoard * BLOCK_OFFSET);
		rect.y += BLOCK_SIZE + BLOCK_OFFSET;
	}
	// Resetting texture alpha.
	SDL_SetTextureAlphaMod(m_ShapesTextures[textureIdx], 0xff);
}


void DisplayManager::drawTetraminoOffBoard(const Tetramino* tetramino, Uint8 alpha, float xPos, float yPos)
{
	// Getting index of shape texture.
	blockShape shape = tetramino->shape();
	int textureIdx = static_cast<int>(shape);
	// Setting texture alpha.
	SDL_SetTextureAlphaMod(m_ShapesTextures[textureIdx], alpha);

	std::vector<std::vector<bool>> TetBlocks = tetramino->blocks();
	
	rect.w = rect.h = BLOCK_SIZE;
	rect.x = xPos;
	rect.y = yPos;
	for (int x = 0; x < TetBlocks.size(); x++)
	{
		for (int y = 0; y < TetBlocks[0].size(); y++) {

			if (TetBlocks[x][y] == 1)
			{
				SDL_RenderCopy(m_renderer, m_ShapesTextures[textureIdx], NULL, &rect);
			}
			rect.x += BLOCK_SIZE + BLOCK_OFFSET;
		}
		rect.x = xPos;
		rect.y += BLOCK_SIZE + BLOCK_OFFSET;
	}
}



void DisplayManager::drawHeld(const Tetramino* tetramino) {
	if (tetramino == nullptr) return;
	drawTetraminoOffBoard(tetramino, 255, HELD_TET_X, HELD_TET_Y);
}

void DisplayManager::drawNext(const std::list<Tetramino*> tetraminos) {
	int y = 0;
	for (auto i : tetraminos) {
		drawTetraminoOffBoard(i, 255, NEXT_TET_X, NEXT_TET_Y + y);
		y += 3 * BLOCK_SIZE + 10;
	}
}

void DisplayManager::drawShadow(const Tetramino* currTetramino, const Board& m_board)
{
	int xBoard = currTetramino->xPos();
	while (!Game::checkCollision(currTetramino, m_board, xBoard + 1, currTetramino->yPos())) {
		xBoard++;
	}
	drawTetramino(currTetramino, 70, xBoard, currTetramino->yPos());
}

void DisplayManager::drawText(std::string text, int x, int y, int w, int h)
{
	textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
	rect.x = x;
	rect.y = y;
	rect.h = h;
	rect.w = w;
	SDL_RenderCopy(m_renderer, textTexture, NULL, &rect);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void DisplayManager::clear()
{
	rect.w = SCREEN_WIDTH;
	rect.h = SCREEN_HEIGHT;
	rect.x = rect.y = 0;
	SDL_RenderCopy(m_renderer, bg_texture, NULL, &rect);
}


SDL_Texture* DisplayManager::loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), 0);
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}
