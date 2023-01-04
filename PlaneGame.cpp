#include "PlaneGame.h"

//初始化
PlaneGame::PlaneGame() :imagefile("NULL"), log(NULL), state(START), window(NULL), renderer(NULL),
bgSurface(NULL),bgTexture(NULL)
{
	//日志
	log = fopen("PlaneGame.log", "w");
	if (log == NULL)exit(-1);

	IMG_Init(IMG_INIT_PNG);
}

//释放
PlaneGame::~PlaneGame()
{
	if (log)fclose(log);
	if (bgSurface)SDL_FreeSurface(bgSurface);
	if (bgTexture) SDL_DestroyTexture(bgTexture);
	if (window) SDL_DestroyWindow(window);
	if (renderer) SDL_DestroyRenderer(renderer);
	if (enemy)delete enemy;
	if (player)delete player;
	IMG_Quit();
	SDL_Quit();
}

void PlaneGame::loadPNG(char *file) {
	if (imagefile != file) { imagefile = file; }
	else { return; }
	bgSurface = IMG_LoadPNG_RW(SDL_RWFromFile(file, "rb"));
	if (!bgSurface) {
		fprintf(log, "cannot open %s\n",file);
		exit(-5);
	}
	bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
	SDL_FreeSurface(bgSurface);
	bgSurface = NULL;
}

//初始操作
void PlaneGame::Init()
{
	//初始化video
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(log, "failed init: %s\n", SDL_GetError());
		exit(-2);
	}

	//bgSurface = IMG_LoadPNG_RW(SDL_RWFromFile("start.png", "rb"));
	m_h = H;
	//创建窗口
	window = SDL_CreateWindow("PlaneGame",
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
	if (!window) {
		fprintf(log, "failed create window:%s\n", SDL_GetError());
		exit(-3);
	}
	SDL_FreeSurface(bgSurface);
	//创建渲染
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(log, "failed create renderer:%s\n", SDL_GetError());
		exit(-4);
	}
}

//游戏主循环
void PlaneGame::mainLoop()
{
	player = new Player(renderer);
	enemy = new Enemy(renderer);
	enemy->start();
	SDL_Event event;
	uint32_t begin, end,_time,rate;
	uint32_t delay;
	while (true) {
		begin = SDL_GetTicks();
		render();
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					if (state !=PLAYING)state = PLAYING;
					break;
				case SDLK_ESCAPE:
					exit(0);
				}
			}
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.type == SDL_MOUSEMOTION) {
				playerX = event.motion.x;
				playerY = event.motion.y;
			}
		}
		end = SDL_GetTicks();
		_time = end - begin;
		rate = 1000 / FRAMERATE;
		delay = rate - _time;
		if (delay < 0) {
			SDL_Delay(delay);
		}
	}
}

//渲染操作
void PlaneGame::render()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	switch (state) {
	case START:
		this->loadPNG("start.png");
		SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
		break;
	case PLAYING:
		this->loadPNG("bg.png");
		SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
		enemy->render(renderer,H);
		player->render(playerX, playerY);
		for (int i = 0; i < enemy->position.size(); ++i) {
			if (enemy->position[i].x >= playerX &&
				enemy->position[i].x <= playerX + player->m_w/2 &&
				enemy->position[i].y >= playerY-120 &&
				enemy->position[i].y <= playerY + player->m_h) {//判断碰撞
				state = END;
			}
		}
		break;
	case END:
		this->loadPNG("end.png");
		SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
		break;
	default:
		break;
	}

	SDL_RenderPresent(renderer);
}

Enemy::Enemy(SDL_Renderer* renderer) : ESurface(NULL), ETexture(NULL)
{
	log = fopen("PlaneGame.log", "w");
	if (log == NULL)exit(-1);

	ESurface = IMG_LoadPNG_RW(SDL_RWFromFile("ball.png", "rb"));
	if (!ESurface) {
		fprintf(log, "cannot open %s\n", "ball.png");
		exit(-5);
	}
	m_w = ESurface->w;
	m_h = ESurface->h;


	ETexture = SDL_CreateTextureFromSurface(renderer, ESurface);
	SDL_FreeSurface(ESurface);
	ESurface = NULL;
}
Enemy::~Enemy()
{
	if (log)fclose(log);
	if (ESurface)SDL_FreeSurface(ESurface);
	if (ETexture) SDL_DestroyTexture(ETexture);
}
//开始计时
void Enemy::start()
{
	m_start = SDL_GetTicks();
}

void Enemy::render(SDL_Renderer *renderer,float height)
{
	uint32_t stop = SDL_GetTicks();
	static std::uniform_real_distribution<float> u(0,height-100);

	if (stop - m_start >= 400) {
		e.seed((unsigned)time(NULL) + e());//更新种子

		float h = u(e);
		SDL_Rect srcrect = { W,h,m_w,m_h };
		position.reserve(100);
		position.push_back(srcrect);
		fprintf(log, "更新位置:{0,%f,%d,%d}\n",h, m_w, m_h);

		m_start = stop;
	}
	if (!position.empty()) {
		int i = 0;
		for (i = 0; i < position.size(); ++i) {
			//if (position[i].x == 0)position[i].x = m_w;
			position[i].x -= 1;
			SDL_Rect rect = position[i];
			SDL_RenderCopy(renderer, ETexture, NULL, &rect);
		}
	}
}

Player::Player(SDL_Renderer* renderer):Prenderer(renderer), PSurface(NULL), PTexture(NULL)
{
	log = fopen("PlaneGame.log", "w");
	if (log == NULL)exit(-1);

	PSurface = IMG_LoadPNG_RW(SDL_RWFromFile("chicken.png", "rb"));
	if (!PSurface) {
		fprintf(log, "cannot open %s\n", "chicken.png");
		exit(-5);
	}

	m_w = PSurface->w;
	m_h = PSurface->h;


	PTexture = SDL_CreateTextureFromSurface(renderer, PSurface);
	SDL_FreeSurface(PSurface);
	PSurface = NULL;
}

Player::~Player()
{
	if (log)fclose(log);
	if (PSurface)SDL_FreeSurface(PSurface);
	if (PTexture) SDL_DestroyTexture(PTexture);
}

void Player::render(Sint32 x, Sint32 y)
{
	SDL_ShowCursor(false);

	SDL_Rect srcrect = { x,y,m_w,m_h };
	SDL_RenderCopy(Prenderer, PTexture, NULL, &srcrect);
}
