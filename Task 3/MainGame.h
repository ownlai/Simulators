#ifndef MAINGAME_H
#define MAINGAME_H

#include "common.h"
#include "define.h"

#include "RNG.h"
#include "LoadConfig.h"

struct MainGameInput
{
    ETestMode TestMode;
    EPayMode PayMode;
    EWildMode WildMode;
	int Lines;
	int LinesBet;
};

struct MainGameOutput
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

class MainGame
{
public:
	MainGame(){};
	~MainGame(){};
	void GetMainGameConfig();
	void MainGameSpin(const MainGameInput MGIn, MainGameOutput *MGOut, int ReelPosition[Reel_Size_Row]);

    int ReelLength[Reel_Size_Row];
    ESymbolList Reel[Reel_Size_Row][Reel_Length_Limit];
    int OddsTable[Symbol_Number_Limit][Reel_Size_Row];
    int LinesTable[Line_Number_Limit][Reel_Size_Row];

private:
    void GetLineWin(const MainGameInput MGIn, MainGameOutput *MGOut);
    void GetWayWin(const MainGameInput MGIn, MainGameOutput *MGOut);
    void GetScatterWin(const MainGameInput MGIn, MainGameOutput *MGOut);
    ESymbolType JoudgeSymbolType(ESymbolList ThisSymbol);
};

#endif
