#include "PlaneGame.h"
int main(int argc,char* argv[]) {
	PlaneGame Game;
	Game.Init();
	Game.mainLoop();
	return 0;
}