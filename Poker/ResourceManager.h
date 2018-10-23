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
#include <vector>

class ResourceManager
{
	friend class Scene;
public:
	std::string GetPokerFullPath(PokerType PT, PokerClassify PC);
	std::string GetRandomBackGroundImagePath() const;
protected:
	ResourceManager();
	~ResourceManager();

	bool Initialise();
	void InitialiseJokerResource();
	void InitialiseBackGroundImages();
private:
	char mAppPath[MAX_PATH];
	std::string mPokerResource[Primary][Poker_Black_Joker];
	std::string mBlackJokerPath;
	std::string mRedJokerPath;
	std::vector<std::string> mBackgroundImageArray;
};
