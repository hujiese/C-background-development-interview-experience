#pragma once
class Game
{
public:
	Game();
	virtual void initialize() = 0;
	virtual void startPlay() = 0;
	virtual void endPlay() = 0;
	void play();//Ä£°å
	virtual ~Game();
};

