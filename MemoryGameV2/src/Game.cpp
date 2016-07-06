#include "Game.h"

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;

	tex = new Texture;

	mState = GameState::PLAYING;
}


Game::~Game()
{
	delete[] mCards;
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Game::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << SDL_GetError() << std::endl;
		system("PAUSE");
		exit(-1);
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::cout << IMG_GetError() << std::endl;
		system("PAUSE");
		exit(-1);
	}

	CreateWindow(cons::TITLE, cons::WIDTH, cons::HEIGHT);

	CreateRenderer();

	InitCards();

	LoadMedia();

	GameLoop();
}

void Game::CreateWindow(std::string title, int w, int h)
{
	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);

	if (mWindow == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		system("PAUSE");
		exit(-1);
	}
}

void Game::CreateRenderer()
{
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		system("PAUSE");
		exit(-1);
	}
}

int Rand(int i) { return std::rand() % i; }

void Game::InitCards()
{
	for (int i = 0; i < cons::CARDS; i++)
		mCards[i] = new Card;

	std::vector<int> ops;
	for (int i = 0; i < cons::CARDS / 2; i++)
		ops.push_back(i);

	std::srand(unsigned(std::time(0)));
	std::random_shuffle(ops.begin(), ops.end(), Rand);

	for (int i = 0; i < cons::CARDS / 2; i++)
		mCards[i]->id = ops[i];

	std::srand(unsigned(std::time(0)));
	std::random_shuffle(ops.begin(), ops.end(), Rand);

	for (int i = 0; i < cons::CARDS / 2; i++)
		mCards[i + (cons::CARDS / 2)]->id = ops[i];
}

void Game::GameLoop()
{
	while (mState == GameState::PLAYING)
	{
		mStartTick = SDL_GetTicks();

		Render();
		Input();

		LimitFrames();
	}
}

void Game::Input()
{
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			mState = GameState::QUIT;
	}
}

void Game::Render()
{
	SDL_RenderClear(mRenderer);

	RenderCards();

	SDL_RenderPresent(mRenderer);
}

void Game::RenderCards()
{
	tex->Render(0, 0, mRenderer);
}

void Game::LimitFrames()
{
	if ((1000 / cons::FPS) > (SDL_GetTicks() - mStartTick))
	{
		SDL_Delay(1000 / cons::FPS - (SDL_GetTicks() - mStartTick));
	}
}

void Game::LoadMedia()
{
	tex->LoadTextureFromFile("res/img.png", mRenderer);

	for (int i = 0; i < cons::CARDS; i++)
	{
		mCards[i]->texture.LoadTextureFromFile("res/back.png", mRenderer);
	}
}
