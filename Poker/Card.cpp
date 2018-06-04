/***********************************************
 * File: Card.cpp
 *
 * Author: LYG
 * Date: ËÄÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/

#include "Card.h"

Card::Card(std::string Name, PokerType PT, PokerClassify PC)
{
	mName = Name;
	mPokerType = PT;
	mPokerClassify = PC;
	ResetAllStatus();
}

Card::~Card()
{

}

std::string Card::GetName() const
{
	return mName;
}

void Card::SetAsPrimary()
{
	mIsPrimary = true;
}

void Card::SetCurrentType(PokerType PT)
{
	mCurrentType = PT;
}

void Card::SetCurrentPrimaryClassify(PokerClassify PC)
{
	mCurrentPrimaryClassify = PC;
}

void Card::ResetAllStatus()
{
	mCurrentType = Poker_Type_Max;
	mCurrentPrimaryClassify = PC_Max;
	mIsPrimary = false;
}

PokerType Card::GetCardType() const
{
	return mPokerType;
}

PokerClassify Card::GetCardClassify() const
{
	return mPokerClassify;
}

bool Card::IsPrimary() const
{
	return mIsPrimary;
}

bool Card::IsCommonPrimary() const
{
	if (mPokerType == Poker_Red_Joker || mPokerType == Poker_Red_Joker || IsCurrentType())
	{
		return false;
	}
	return true;
}

bool Card::IsCurrentType() const
{
	return mPokerType == mCurrentType;
}

bool Card::IsCurrentPrimaryClassify() const
{
	return mPokerClassify == mCurrentPrimaryClassify;
}

bool Card::IsBlackJoker() const
{
	return mPokerType == Poker_Black_Joker;
}

bool Card::IsRedJoker() const
{
	return mPokerType == Poker_Red_Joker;
}

bool Card::IsJoker() const
{
	if (IsRedJoker() || IsBlackJoker())
	{
		return true;
	}
	return false;
}

bool Card::operator>(const Card& other) const
{
	// compare the classify first
	if (IsPrimary() && other.IsPrimary())
	{
		{
			// they all are the primary
			if (IsCommonPrimary() && other.IsCommonPrimary())
			{
				// they all are the common primary
				if (mPokerType > other.GetCardType())
				{
					return true;
				}
				return false;
			}
			else if(IsCommonPrimary() == false && other.IsCommonPrimary())
			{
				return true;
			}
			else if (IsCommonPrimary() && other.IsCommonPrimary() == false)
			{
				return false;
			}
			else
			{
				// all are the specific primary
				if (IsJoker() && other.IsJoker() == false)
				{
					return true;
				}
				else if(IsJoker() == false && other.IsJoker())
				{
					return false;
				}
				else if (IsJoker() == false && other.IsJoker() == false)
				{
					// all not the joker
					if (IsCurrentPrimaryClassify() && other.IsCurrentPrimaryClassify() == false)
					{
						return true;
					}
					return false;
				}
				else
				{
					// all are the joker
					if (IsRedJoker() && other.IsBlackJoker())
					{
						return true;
					}
					return false;
				}
			}
		}

	}
	else
	{
		// primary is greater than others. all others are the same priority
		if (IsPrimary() && other.IsPrimary() == false)
		{
			return true;
		}
		else if (other.IsPrimary())
		{
			return false;
		}
		else
		{
			// all are not primary
			if (mPokerClassify == other.GetCardClassify() && mPokerType > other.GetCardType())
			{
				return true;
			}
			return false;
		}
	}
}

bool Card::operator<(const Card& other) const 
{
	// compare the classify first
	if (IsPrimary() && other.IsPrimary())
	{
		{
			// they all are the primary
			if (IsCommonPrimary() && other.IsCommonPrimary())
			{
				// they all are the common primary
				if (mPokerType < other.GetCardType())
				{
					return true;
				}
				return false;
			}
			else if (IsCommonPrimary() == false && other.IsCommonPrimary())
			{
				return false;
			}
			else if (IsCommonPrimary() && other.IsCommonPrimary() == false)
			{
				return true;
			}
			else
			{
				// all are the specific primary
				if (IsJoker() && other.IsJoker() == false)
				{
					return false;
				}
				else if (IsJoker() == false && other.IsJoker())
				{
					return true;
				}
				else if (IsJoker() == false && other.IsJoker() == false)
				{
					// all not the joker
					if (IsCurrentPrimaryClassify() == false && other.IsCurrentPrimaryClassify())
					{
						return true;
					}
					return false;
				}
				else
				{
					// all are the joker
					if (IsBlackJoker() && other.IsRedJoker())
					{
						return true;
					}
					return false;
				}
			}
		}

	}
	else
	{
		// primary is greater than others. all others are the same priority
		if (IsPrimary() && other.IsPrimary() == false)
		{
			return false;
		}
		else if (other.IsPrimary())
		{
			return true;
		}
		else
		{
			// all are not primary
			if (mPokerClassify == other.GetCardClassify() && mPokerType < other.GetCardType())
			{
				return true;
			}
			return false;
		}
	}
}

bool Card::operator == (const Card& other) const
{
	if (mPokerType == other.GetCardType() && mPokerClassify == other.GetCardClassify())
	{
		return true;
	}
	return false;
}

CardManager::CardManager()
{

}

CardManager::~CardManager()
{
	std::map<std::string, Card*>::iterator it;
	for (it = mCardArray.begin(); it != mCardArray.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	mCardArray.clear();
}

Card* CardManager::CreateCard(std::string Name, PokerType PT, PokerClassify PC)
{
	if (mCardArray.find(Name) != mCardArray.end())
	{
		return mCardArray[Name];
	}
	Card* C = new Card(Name, PT, PC);
	mCardArray[Name] = C;

	return C;
}

Card* CardManager::GetCardByName(std::string Name)
{
	if (mCardArray.find(Name) != mCardArray.end())
	{
		return mCardArray[Name];
	}
	return nullptr;
}