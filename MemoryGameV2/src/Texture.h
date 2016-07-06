#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


class Texture
{
public:
	Texture();
	~Texture();

	void LoadTextureFromFile(std::string path, SDL_Renderer * renderer);

	void Render(int x, int y, SDL_Renderer * renderer);

private:
	void Free();

	int mWidth;
	int mHeight;

	SDL_Texture * mTexture;
};

#endif //TEXTURE_H