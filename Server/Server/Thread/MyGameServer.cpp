#include <iostream>
#include "MyGameServer.h"

MyGameServer::MyGameServer()
{
}

MyGameServer::~MyGameServer()
{
}

void MyGameServer::DoSomething(int playerID)
{
	Room room;

	{
		CriticalSectionLock lock(critSec);
		room = *(roomList.find(playerID)->second);
	} // unlock
	
	{
		CriticalSectionLock lock(room.critSec);
		room.DoSomething(playerID);
	} // unlock
}
