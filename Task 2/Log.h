#ifndef LOG_H
#define LOG_H

#include "common.h"
#include "define.h"

#include "MainGame.h"
#include "FreeGame.h"
#include "BonusGame.h"
#include "LoadConfig.h"

struct STestConfig
{
    ETestMode Test_Mode;

    EPayMode MGPay_Mode;
    EPayMode FGPay_Mode;

    EWildMode MGWild_Mode;
    EWildMode FGWild_Mode;

    int Play_Line;
    int Lines_Bet;
    int Test_Round;
    int StatisticPeriod;
};

void InitializeLog();
void ReportStatistic();

void CalculateStatistic(MainGameOutput MGOut);
void CalculateStatistic(MainGameOutput MGOut, int *ThisRoundWin);
void CalculateStatistic(FreeGameOutput FGOut);
void CalculateStatistic(FreeGameOutput FGOut, int *ThisRoundWin);
void CalculateStatistic(BonusGameOutput BGOut);
void CalculateStatistic(BonusGameOutput BGOut, int *ThisRoundWin, bool FromFG = false);

void CalculateStatistic(int ThisRoundWin);
void CalculatePeriodStatistic(int ThisPeriodWin);

void AddSymbolHit(ESymbolList SymbolIdx, int SymbolCount);
void AddTotalLineWin(int Credit);
void AddTotalSFWin(int Credit);
void AddTotalSBWin(int Credit);
void SetRound(int PlayRound);

void PrintOddsTable();
void PrintReelStripe();

void GetTestConfig(STestConfig *TestConfig);

#endif
