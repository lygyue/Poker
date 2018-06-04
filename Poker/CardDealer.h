/***********************************************
 * File: CardDealer.h
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
#include <vector>

class CardDealer
{
	friend class PokerLogicManager;
public:
	virtual Card* GetCard();
	virtual void PushCard(Card* C);
	// update all cards before start a new game
	virtual void Update();
protected:
	CardDealer();
	virtual ~CardDealer();

	std::vector<Card*> mCardsArray;
};

class RamdomDealer : public CardDealer
{
	friend class PokerLogicManager;
public:
	virtual Card* GetCard();
protected:
	RamdomDealer();
	virtual ~RamdomDealer();
private:

};
