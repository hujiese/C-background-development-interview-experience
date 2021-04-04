#pragma once
#include <iostream>
#include <string>
#include "ChatRoom.h"
using namespace std;
class ChatRoom;
class User
{
public:
	explicit User();
	User(string name);
	string getName() const;
	void setName(const string name);
	void sendMessage(const string message);
	virtual ~User();

private:
	string name;
};

