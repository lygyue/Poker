/***********************************************
 * File: Card.h
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
#include <map>

class CardManager;
class Card
{
	friend class CardManager;
public:
	std::string GetName() const;
	void SetAsPrimary();
	void SetCurrentType(PokerType PT);
	void SetCurrentPrimaryClassify(PokerClassify PC);
	// been called before start a new game.
	void ResetAllStatus();

	PokerType GetCardType() const;
	PokerClassify GetCardClassify() const;

	bool IsPrimary() const;
	// common primary is not joker, not current grade
	bool IsCommonPrimary() const;
	bool IsCurrentType() const;
	bool IsCurrentPrimaryClassify() const;
	bool IsBlackJoker() const;
	bool IsRedJoker() const;
	bool IsJoker() const;

	bool operator > (const Card& other) const;
	bool operator < (const Card& other) const;
	bool operator == (const Card& other) const;
protected:
	Card(std::string Name, PokerType PT, PokerClassify PC);
	~Card();
private:
	PokerType mPokerType;
	PokerClassify mPokerClassify;
	bool mIsPrimary;
	PokerType mCurrentType;
	PokerClassify mCurrentPrimaryClassify;
	std::string mName;
};

class CardManager
{
	friend class PokerLogicManager;
public:
	Card* CreateCard(std::string Name, PokerType PT, PokerClassify PC);
	Card* GetCardByName(std::string Name);
protected:
	CardManager();
	~CardManager();
private:
	std::map<std::string, Card*> mCardArray;
};
