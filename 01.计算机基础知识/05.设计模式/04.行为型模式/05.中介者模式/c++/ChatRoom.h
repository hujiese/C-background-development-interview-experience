#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "User.h"
using namespace std;
class User;

class ChatRoom
{
public:
	ChatRoom();
	static void showMessage(shared_ptr<User> user, string message);
	virtual ~ChatRoom();
};

