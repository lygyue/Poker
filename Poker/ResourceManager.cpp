/*!
 * \file ResourceManager.cpp
 *
 * \author LYG
 * \date ËÄÔÂ 2018
 *
 * 
 */

#include "ResourceManager.h"
#include "Scene.h"
#include "SimpleMath.h"
#include <windows.h>

#define POKER_RELATIVE_PATH			"Resource\\Card\\"
#define POKER_BACKGROUND_PATH		"Resource\\BackGround\\"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

bool ResourceManager::Initialise()
{
	std::string ApplicationPath = Scene::GetCurrentScene()->GetApplicationPath();
	strcpy_s(mAppPath, MAX_PATH, ApplicationPath.c_str());
	InitialiseJokerResource();
	InitialiseBackGroundImages();
	return true;
}

void ResourceManager::InitialiseJokerResource()
{
	std::string ChildFolder = POKER_RELATIVE_PATH;
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

void ResourceManager::InitialiseBackGroundImages()
{
	BOOL bFind = TRUE;
	WIN32_FIND_DATAA fileData;
	char szOldCurDir[MAX_PATH];
	GetCurrentDirectoryA(sizeof(szOldCurDir), szOldCurDir);
	char szDirPath[MAX_PATH];
	std::string AppPath = Scene::GetCurrentScene()->GetApplicationPath();
	memset(szDirPath, 0, sizeof(szDirPath));
	strcpy_s(szDirPath, MAX_PATH, AppPath.c_str());
	strcat_s(szDirPath, MAX_PATH, POKER_BACKGROUND_PATH);
	SetCurrentDirectoryA(szDirPath);
	HANDLE hFind = FindFirstFileA("*", &fileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}
	while (bFind) 
	{
		// don't consider as a folder
		std::string Name = szDirPath;
		int nLen = strlen(fileData.cFileName);
		if (nLen > 4)
		{
			char szExtern[5];
			memset(szExtern, 0, 5);
			strcpy_s(szExtern, 5, fileData.cFileName + nLen - 3);
			if (_stricmp(szExtern, "jpg") == 0)
			{
				Name += fileData.cFileName;
				mBackgroundImageArray.push_back(Name);
			}
		}
		bFind = FindNextFileA(hFind, &fileData);
	}
	SetCurrentDirectoryA(szOldCurDir);
	FindClose(hFind);
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

std::string ResourceManager::GetRandomBackGroundImagePath() const
{
	static int LastIndex = 0;
	int Index = 0;
	while (true)
	{
		Index = (int)RangeRandom(0.0f, float(mBackgroundImageArray.size() - 0.001));
		if (LastIndex != Index)
		{
			LastIndex = Index;
			break;
		}
	}
	return mBackgroundImageArray[Index];
}