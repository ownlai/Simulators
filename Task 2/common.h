#ifndef COMMON_H
#define COMMON_H

#include "define.h"

/***** enumeration define *****/
enum ETestMode
{
    //Test Full Cycle
    ETest_FullCycleMainGame     = 0,
    ETest_FullCycleFreeGame     = 1,

    //Test with RNG
    ETest_RandomMainGame        = 2,
    ETest_RandomFreeGame        = 3, //主要用來測試 完整1次Free game 得獎分布
    ETest_RandomBonusGame       = 4,//主要用來測試 完整1次Bonus game 得獎分布
    ETest_RandomAllGame         = 5,
};

enum EPayMode
{
    //Lines
    EPayMode_Lines_Left         = 0,
    //Two directions Pay-Line
    EPayMode_Lines_Left_Right   = 1,


    //243 Ways, 1024 ways, ...
    EPayMode_Ways_Left          = 2,
    //Two directions Pay_Way
    EPayMode_Ways_Left_Right    = 3,
};

enum EWildMode
{
    EWildMode_Normal            = 0,
    EWildMode_Multiplier        = 1,
    EWildMode_Expend            = 2,
    EWildMode_Lock              = 3,
};

enum ESymbolType
{
    ESymbolType_Normal          = 0,
    ESymbolType_Wild            = 1,
    ESymbolList_Scatter         = 2,
    ESymbolType_Consecutive     = 3,
};

enum ESymbolList
{
    //Special Symbol
	MW = 0,
	EW = 1,

	//Big Symbol
    H1 = 2,
	H2 = 3,
	H3 = 4,
	H4 = 5,
	H5 = 6,
    H6 = 7,

	//Small Symbol
	LA = 8,
	LK = 9,
	LQ = 10,
	LJ = 11,
	//LT = 10,
    //L9,
    //L8,

	//Scatter
	SB = 12, //bonus game
	//SF = 12, //free game

	//Info
	SYM_MAX = 13,
};

//spin result
enum EWinType
{
	EWinType_NoWin	= 0X00,
	EWinType_WinMG	= 0X01,
	EWinType_WinBG  = 0X02,
	EWinType_WinFG	= 0X04,
};

const char LogFilePath[8][50] =
{
    ".\\log\\FullCycleMG",
    ".\\log\\FullCycleFG",
    ".\\log\\RandomMG",
    ".\\log\\RandomFG",
    ".\\log\\RandomBG",
    ".\\log\\RandomAG",
    ".\\log\\RoundWin.txt",
    ".\\log\\PeriodWin.txt",
};

const char SymbolName[Symbol_Number_Limit][5] =
{
    "MW",
    "EW",
    "H1",
    "H2",
    "H3",
    "H4",
    "H5",
    "H6",
    "LA",
    "LK",
    "LQ",
    "LJ",
    "SB",
};

#endif
