#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"
void UserManager::ProcessSave()
{
	//// userLock
	//lock_guard<mutex> guard(_mutex);
	//// accountLock
	//Account* account = AccountManager::Instance()->GetAccount(100);

	// 순서를 맞춰준다.
	// accountLock
	Account* account = AccountManager::Instance()->GetAccount(100);
	// userLock
	lock_guard<mutex> guard(_mutex);
	
	//TODO
}