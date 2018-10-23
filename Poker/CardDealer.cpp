/***********************************************
 * File: CardDealer.cpp
 *
 * Author: LYG
 * Date: ËÄÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#include "CardDealer.h"
#include <time.h>

CardDealer::CardDealer()
{

}

CardDealer::~CardDealer()
{
	mCardsArray.clear();
}

Card* CardDealer::GetCard()
{
	return nullptr;
}

void CardDealer::PushCard(Card* C)
{
	mCardsArray.push_back(C);
}

void CardDealer::Update()
{

}

RamdomDealer::RamdomDealer()
{
	
}

RamdomDealer::~RamdomDealer()
{

}

Card* RamdomDealer::GetCard()
{
	int Size = mCardsArray.size();
	if (Size == 0)
	{
		return nullptr;
	}
	int pos = rand() % Size;
	Card* C = mCardsArray[pos];

	mCardsArray.erase(mCardsArray.begin() + pos);

	return C;
}