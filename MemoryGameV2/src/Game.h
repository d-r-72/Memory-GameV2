#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "constants.h"
#include "Texture.h"

enum class GameState { PLAYING, QUIT };

struct Card
{
	Texture backTex;
	
	std::string resPath;
	
	bool renderBack;
	bool render;
	
	int id;
};

class Game
{
public:
	Game();
	~Game();

	void Init();

private:
	void InitCards();

	void GameLoop();
	void Input();
	
	void Match();

	void Render();
	void RenderCards();

	void LimitFrames();

	void LoadMedia();

	template <class Type>
	void ShuffleVector(std::vector<Type> &vec);

	void CreateWindow(std::string title, int w, int h);
	void CreateRenderer();

	SDL_Window * mWindow;
	SDL_Renderer * mRenderer;

	SDL_Event e;

	std::vector<Card *> mCards;
	std::vector<int> mRenderOrder;

	Texture * frontTex;

	int currentFlips;
	int mPairs;

	std::vector<int> idChoices;

	Uint32 mStartTick;

	GameState mState;
};

#endif //GAME_H