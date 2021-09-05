#pragma once
#include "CriticalSection.h"
#include <string>
#include <list>
#include <map>
#include <memory>

using namespace std;

class MyGameServer
{
private:
	class Room
	{
	public:
		CriticalSection		critSec;
		string				roomName;
		list<int>			players;
		list<int>			characters;

		void	DoSomething(int playerID) {};
	};

	map<int, shared_ptr<Room>>	roomList;
	string						serverName;
	CriticalSection				critSec;

public:
	MyGameServer();
	~MyGameServer();

	void	DoSomething(int playerID);
};