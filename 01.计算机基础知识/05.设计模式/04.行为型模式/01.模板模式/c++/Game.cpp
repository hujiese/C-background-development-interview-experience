#include "Game.h"


Game::Game()
{
}

void Game::play()
{
	//初始化游戏
	initialize();

	//开始游戏
	startPlay();

	//结束游戏
	endPlay();
}

Game::~Game()
{
}
