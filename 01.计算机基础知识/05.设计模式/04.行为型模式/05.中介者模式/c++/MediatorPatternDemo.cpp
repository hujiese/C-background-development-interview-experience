#include <iostream>
#include <string>
#include "ChatRoom.h"
#include "User.h"
using namespace std;

int main(void)
{
	shared_ptr<User> robert(new User("Robert"));
	shared_ptr<User> john(new User("John"));

	robert->sendMessage("Hi! John!");
	john->sendMessage("Hello! Robert!");

	return 0;
}