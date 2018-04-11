/*!
 * \file ResourceManager.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */

#include "ResourceManager.h"

ResourceManager* ResourceManager::Instance = nullptr;
ResourceManager::ResourceManager()
{
	// Get Current Directory
	memset(mAppPath, 0, MAX_PATH);
	char Temp[MAX_PATH];
	memset(Temp, 0, MAX_PATH);

	GetModuleFileNameA(NULL, Temp, MAX_PATH);
	int nLen = strlen(Temp);
	while (nLen)
	{
		if (Temp[nLen] == '\\' || Temp[nLen] == '/')
		{
			break;
		}
		Temp[nLen--] = '\0';
	}
	strcpy_s(mAppPath, MAX_PATH, Temp);

	InitialiseJokerResource();
}

ResourceManager::~ResourceManager()
{

}

ResourceManager* ResourceManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new ResourceManager;
	}
	return Instance;
}

void ResourceManager::ReleaseInstance()
{
	SAFE_DELETE(Instance);
}

void ResourceManager::InitialiseJokerResource()
{
	std::string ChildFolder = "Resource\\Card\\";
	char* szPoker[] = { "Diamond_", "CLub_", "Heart_", "Spade_" };
	for (int i = 0; i < Primary; i++)
	{
		for (int j = 0; j < Poker_Black_Joker; j++)
		{
			mPokerResource[i][j] = mAppPath;
			mPokerResource[i][j] += ChildFolder;
			mPokerResource[i][j] += szPoker[i];
			char Buff[4] = { 0 };
			sprintf_s(Buff, 4, "%d", j);
			mPokerResource[i][j] += Buff;
			mPokerResource[i][j] += ".jpg";
		}
	}
	mBlackJokerPath = mAppPath;
	mBlackJokerPath += ChildFolder;
	mBlackJokerPath += "Black_Joker.jpg";

	mRedJokerPath = mAppPath;
	mRedJokerPath += ChildFolder;
	mRedJokerPath += "Red_Joker.jpg";
}

std::string ResourceManager::GetPokerFullPath(PokerType PT, PokerClassify PC) 
{
	if (PT == Poker_Black_Joker)
	{
		return mBlackJokerPath;
	}
	else if (PT == Poker_Red_Joker)
	{
		return mRedJokerPath;
	}
	else
	{
		return mPokerResource[PC][PT];
	}
}