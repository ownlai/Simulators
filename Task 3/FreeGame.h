#ifndef FREEGAME_H
#define FREEGAME_H

#include "common.h"
#include "define.h"

#include "RNG.h"
#include "LoadConfig.h"

struct FreeGameInput
{
    ETestMode TestMode;
    EPayMode PayMode;
    EWildMode WildMode;
	int Lines;
	int LinesBet;
};

struct FreeGameOutput
{
	ESymbolList SymbolResult[Reel_Size_Column][Reel_Size_Row];
	ESymbolList LineSymbol[Max_Line];
	int LineSymbolCount[Max_Line];
	int LineWin[Max_Line];
	int SBWin;
	int SBCount;
	int SFWin;
	int SFCount;
	EWinType WinType;
};

class FreeGame
{
public:
	FreeGame(){};
	~FreeGame(){};
    void GetFreeGameConfig();
	void FreeGameSpin(const FreeGameInput FGIn, FreeGameOutput *FGOut, int ReelPosition[Reel_Size_Row]);

    int ReelLength[Reel_Size_Row];
    ESymbolList Reel[Reel_Size_Row][Reel_Length_Limit];
    int OddsTable[Symbol_Number_Limit][Reel_Size_Row];
    int LinesTable[Line_Number_Limit][Reel_Size_Row];

private:
    void GetLineWin(const FreeGameInput FGIn, FreeGameOutput *FGOut);
    void GetWayWin(const FreeGameInput FGIn, FreeGameOutput *FGOut);
    void GetScatterWin(const FreeGameInput FGIn, FreeGameOutput *FGOut);
    ESymbolType JoudgeSymbolType(ESymbolList ThisSymbol);
};

#endif
