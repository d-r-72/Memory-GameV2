#include "Game.h"

Game::Game()
{
	mWindow = nullptr;
	mRenderer = nullptr;

	frontTex = new Texture;

	currentFlips = 0;
	mPairs = 0;

	mState = GameState::PLAYING;
}


Game::~Game()
{
	for (int i = 0; i < cons::CARDS; i++)
		delete mCards[i];

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

	int imgFlags = IMG_INIT_PNG || IMG_INIT_JPG;
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
		mRenderOrder.push_back(i);

	ShuffleVector(mRenderOrder);

	for (int i = 0; i < cons::CARDS; i++)
		mCards.push_back(new Card);

	for (int i = 0; i < cons::CARDS; i++)
		mCards[i]->renderBack = false;

	for (int i = 0; i < cons::CARDS; i++)
		mCards[i]->render = true;

	std::vector<int> ops;
	for (int i = 0; i < cons::CARDS / 2; i++)
		ops.push_back(i);

	ShuffleVector(ops);

	for (int i = 0; i < cons::CARDS / 2; i++)
		mCards[i]->id = ops[i];

	ShuffleVector(ops);

	for (int i = 0; i < cons::CARDS / 2; i++)
		mCards[i + (cons::CARDS / 2)]->id = ops[i];

	for (int i = 0; i < cons::CARDS; i++)
	{
		std::string path = "MemoryGameV2/res/back/back";
		path.append(std::to_string(mCards[i]->id) + ".png");
		mCards[i]->resPath = path;
	}
	
	for (int i = 0; i < cons::CARDS; i++)
	{
		std::cout << mCards[i]->resPath << std::endl;
	}

	for (int i = 0; i < cons::CARDS; i++)
	{
		std::cout << "Card #" << i << " Id: " << mCards[i]->id << std::endl;
	}

	std::cout << "Render order " << std::endl;
	for (int i = 0; i < cons::CARDS; i++)
	{
		std::cout << mRenderOrder[i] << std::endl;
	}
}

template <class Type>
void Game::ShuffleVector(std::vector<Type> &vec)
{
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(vec.begin(), vec.end(), Rand);
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

		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_e:
				for (int i = 0; i < cons::CARDS; i++)
				{
					mCards[i]->renderBack = (!mCards[i]->renderBack);
				}
				break;
			}
		}

		int x, y;
		SDL_GetMouseState(&x, &y);

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			for (int i = 0; i < mCards.size(); i++)
			{
				bool inCard = true;
				if (x < mCards[i]->backTex.GetX())
					inCard = false;
				if (x > mCards[i]->backTex.GetX() + /*Card Width*/ 100)
					inCard = false;
				if (y > mCards[i]->backTex.GetY() + /*Card Height*/ 200)
					inCard = false;
				if (y < mCards[i]->backTex.GetY())
					inCard = false;

				if (inCard)
				{
					if (mCards[i]->render == true)
					{
						if (mCards[i]->renderBack == true)
						{
							mCards[i]->renderBack = (!mCards[i]->renderBack);
							currentFlips--;
						}
						else if(currentFlips <= cons::MAX_CARD_FLIPS)
						{
							mCards[i]->renderBack = (!mCards[i]->renderBack);
							idChoices.push_back(mCards[i]->id);

							if (currentFlips == cons::MAX_CARD_FLIPS)
							{
								if (idChoices[0] == idChoices[1])
									Match();
								
								idChoices.clear();
							}
							currentFlips++;
						}
					}
					//std::cout << "Current flips " << currentFlips << std::endl;
				}
			}			
		}
	}
}

void Game::Match()
{
	mPairs++;
	std::cout << "Current Pairs: " << mPairs << '\n';

	for (int i = 0; i < mCards.size(); i++)
	{
		if (mCards[i]->renderBack == true)
		{
			mCards[i]->render = false;
		}
	}
	
	bool done = true;
	
	for (int i = 0; i < mCards.size(); i++)
	{
		if (mCards[i]->render == true)
			done = false;
	}

	if (done)
		mState = GameState::QUIT;

	currentFlips = -1;
}

void Game::Render()
{
	SDL_RenderClear(mRenderer);

	RenderCards();

	SDL_RenderPresent(mRenderer);
}

void Game::RenderCards()
{
	for (int i = 0; i < cons::CARDS; i++)
	{
		if (mCards[mRenderOrder[i]]->render == true)
		{
			if ((i * 132) > cons::WIDTH)
			{
				if (mCards[mRenderOrder[i]]->renderBack == false)
				{
					frontTex->Render(((i - cons::CARDS / 2) * 132), (cons::HEIGHT - 
					(/*Card Height*/ 200 + /*Height Above bottom*/ 60)), mRenderer);

					mCards[mRenderOrder[i]]->backTex.SetX(((i - cons::CARDS / 2) * 132));
					mCards[mRenderOrder[i]]->backTex.SetY((cons::HEIGHT - (200 + 60)));
				}
				else
				{
					mCards[mRenderOrder[i]]->backTex.Render(((i - cons::CARDS / 2) * 132), (cons::HEIGHT -
					(/*Card Height*/ 200 + /*Height Above bottom*/ 60)), mRenderer);
				}
			}
			else
			{
				if (mCards[mRenderOrder[i]]->renderBack == false)
				{
					frontTex->Render(i * 132, (cons::HEIGHT / 12), mRenderer);

					mCards[mRenderOrder[i]]->backTex.SetX(i * 132);
					mCards[mRenderOrder[i]]->backTex.SetY((cons::HEIGHT / 12));
				}
				else
				{
					mCards[mRenderOrder[i]]->backTex.Render(i * 132, (cons::HEIGHT / 12), mRenderer);
				}
			}
		}
	}
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
	frontTex->LoadTextureFromFile("MemoryGameV2/res/front.png", mRenderer);

	for (int i = 0; i < cons::CARDS; i++)
		mCards[i]->backTex.LoadTextureFromFile(mCards[i]->resPath, mRenderer);
	
}
