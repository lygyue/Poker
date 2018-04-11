/*!
 * \file PokerDef.h
 *
 * \author LYG
 * \date 四月 2018
 *
 * 
 */
#pragma once

// Four pokers
#define POKER_COUNT			4
// 类型，带单个牌的大小关系，从小到大。
enum PokerType
{
	Poker_2,
	Poker_3,
	Poker_4,
	Poker_5,
	Poker_6,
	Poker_7,
	Poker_8,
	Poker_9,
	Poker_10,
	Poker_11,
	Poker_12,
	Poker_13,
	Poker_1,
	Poker_Black_Joker,
	Poker_Red_Joker,
	Poker_Type_Max,
};

// 四种花色
enum PokerClassify
{
	Diamond,
	CLub,
	Heart,
	Spade,
	Primary,			// Joker must in this group
};