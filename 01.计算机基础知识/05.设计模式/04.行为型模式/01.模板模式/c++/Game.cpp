#include "Game.h"


Game::Game()
{
}

void Game::play()
{
	//��ʼ����Ϸ
	initialize();

	//��ʼ��Ϸ
	startPlay();

	//������Ϸ
	endPlay();
}

Game::~Game()
{
}
