#include "pch.h"
#include "AccountManager.h"
#include "PlayerManager.h"

PlayerManager GPlayerManager;

void PlayerManager::PlayerThenAccount()
{
	WRITE_LOCK;

	this_thread::sleep_for(1s);

	GAccountManager.Lock();
}

void PlayerManager::Lock()
{
	WRITE_LOCK;
}