/***********************************************
 * File: PokerLogicManager.h
 *
 * Author: LYG
 * Date: ËÄÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#pragma once
#include "PokerDef.h"
#include "Common.h"
#include "Card.h"
#include "CardDealer.h"

class PokerLogicManager
{
public:
	PokerLogicManager();
	~PokerLogicManager();

	bool Initialise(HWND hWnd);

	void StartNewGame();
	void Update();
public:
	void OnKeyDown(unsigned char Key);
	void OnKeyUp(unsigned char Key);
	void OnLButtonDown(int x, int y, unsigned int wParam);
	void OnLButtonUp(int x, int y, unsigned int wParam);
	void OnRButtonDown(int x, int y, unsigned int wParam);
	void OnRButtonUp(int x, int y, unsigned int wParam);
	void OnMouseMove(int x, int y, unsigned int wParam);
	void OnLButtonDbclk(int x, int y, unsigned int wParam);
	void OnRButtonDbclk(int x, int y, unsigned int wParam);
protected:
	void BuildPokerName();
	void InitialiseAllResource();
	std::string GetPokerName(PokerType PT, PokerClassify PC) const;
private:
	std::string mPokerResourceName[Primary][Poker_Black_Joker];
	std::string mBlackJokerName;
	std::string mRedJokerName;

	CardManager* mCardManager;
	CardDealer* mCardDealer;

};