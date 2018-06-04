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
	friend class Scene;
public:
	std::string GetPokerFullPath(PokerType PT, PokerClassify PC);
protected:
	ResourceManager();
	~ResourceManager();

	void InitialiseJokerResource();
private:
	char mAppPath[MAX_PATH];
	std::string mPokerResource[Primary][Poker_Black_Joker];
	std::string mBlackJokerPath;
	std::string mRedJokerPath;
};
