/***********************************************
 * File: Player.h
 *
 * Author: LYG
 * Date: ËÄÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#pragma once
#include "Common.h"
#include "PokerDef.h"
#include <list>
#include "Card.h"

enum PlayerOrder
{
	Human,
	Robot1,
	Robot2,
	Robot3,
	PlayerMax,
};

typedef std::list<Card*> CardList;
class Player
{
	friend class PokerLogicManager;
public:
	virtual void ReceiveCard(Card* C);
	virtual void SetCurrentGrade(PokerType PT);
protected:
	Player();
	~Player();

	// notify function
	void NotifyReceiveGradeCard(Card* C);

	PokerType mCurrentPlayingGrade;
	CardList mCards[PC_Max];
};