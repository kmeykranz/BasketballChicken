#define _CRT_SECURE_NO_WARNINGS

#ifndef __PLANEGAME_H__
#define __PLANEGAME_H__

#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <random>
#include<vector>

#ifdef __WIN32
#define Main SDL_main
#endif

#ifndef __WIN32
#define Main main
#endif

#define W 960
#define H 540
#define FRAMERATE 60 

enum GameState {
	START,
	END,
	PLAYING
};

class Player {
public:
	Player(SDL_Renderer* renderer);
	~Player();
	void fuck();

	void render(Sint32 x, Sint32 y);
private:
	SDL_Surface* PSurface;
	SDL_Texture* PTexture;
	FILE* log;

	SDL_Renderer* Prenderer;
public:
	int m_w;
	int m_h;
};

class Enemy 
{
public:
	Enemy(SDL_Renderer* renderer);
	~Enemy();

	void start();
	void render(SDL_Renderer* renderer, float weight);

	std::vector<SDL_Rect>position;
private:
	SDL_Surface* ESurface;
	SDL_Texture* ETexture;
	FILE* log;

	uint32_t m_start;
	//random
	std::default_random_engine e;
	int m_w;
	int m_h;
};

class PlaneGame
{
public:
	PlaneGame();
	~PlaneGame();//end

	void loadPNG(char* file);
	void Init();
	void mainLoop();//ÓÎÏ·Ö÷Ñ­»·
	void render();//äÖÈ¾
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* bgSurface;
	SDL_Texture* bgTexture;

	FILE *log;
	GameState state;//ÓÎÏ·×´Ì¬
	char* imagefile;

	int m_h;
	Enemy *enemy;
	Player* player;

	Sint32 playerX;
	Sint32 playerY;
};
#endif