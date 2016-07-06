#include "Texture.h"



Texture::Texture()
{
	mWidth = 0;
	mHeight = 0;
	mx = 0, my = 0;

	mTexture = nullptr;
}


Texture::~Texture()
{
}

void Texture::LoadTextureFromFile(std::string path, SDL_Renderer * renderer)
{
	Free();

	SDL_Surface * sur = IMG_Load(path.c_str());
	if (sur == nullptr)
	{
		std::cout << IMG_GetError() << std::endl;
		system("PAUSE");
		exit(-1);
	}
	
	mTexture = SDL_CreateTextureFromSurface(renderer, sur);
	if (mTexture == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		system("PAUSE");
		exit(-1);
	}

	mWidth = sur->w;
	mHeight = sur->h;

	SDL_FreeSurface(sur);
}

void Texture::Render(int x, int y, SDL_Renderer * renderer)
{
	SDL_Rect rect = { x, y, mWidth, mHeight };

	mx = x;
	my = y;

	SDL_RenderCopy(renderer, mTexture, NULL, &rect);
}

void Texture::Free()
{
	if (mTexture != nullptr)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}