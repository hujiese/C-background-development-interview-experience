#include <iostream>
#include <memory>
#include "Game.h"
#include "Football.h"
#include "Cricket.h"

using namespace std;

int main(void)
{
	shared_ptr<Game> game(new Cricket());//¶àÌ¬
	game->play();

	cout << endl;

	game.reset(new Football());
	game->play();

	return 0;
}