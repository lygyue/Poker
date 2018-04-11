/*!
 * \file ResourceManager.h
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */
#pragma once
#include <string>
#include <windows.h>
#include "Common.h"
#include "PokerDef.h"


class ResourceManager
{
public:
	static ResourceManager* GetInstance();
	static void ReleaseInstance();

	std::string GetPokerFullPath(PokerType PT, PokerClassify PC);
protected:
	ResourceManager();
	~ResourceManager();

	void InitialiseJokerResource();

	static ResourceManager* Instance;
private:
	char mAppPath[MAX_PATH];
	std::string mPokerResource[Primary][Poker_Black_Joker];
	std::string mBlackJokerPath;
	std::string mRedJokerPath;
};
