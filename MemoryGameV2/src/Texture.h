#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

class Texture
{
public:
	Texture();
	~Texture();

	void LoadTextureFromFile(std::string path, SDL_Renderer * renderer);

	void Render(int x, int y, SDL_Renderer * renderer);

	int GetX() { return mx; }
	int GetY() { return my; }

	void SetX(int amn) { mx = amn; }
	void SetY(int amn) { my = amn; }

private:
	void Free();

	int mWidth;
	int mHeight;

	int mx, my;

	SDL_Texture * mTexture;
};

#endif //TEXTURE_H