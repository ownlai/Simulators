#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "Log.h"

#define MaxFeatureWinX  200

#define MaxRTPCount     (500/2)
#define MinRTPCenter    22
#define MaxRTPCenter    300
#define MinRTPIdx       (MinRTPCenter/2+1)
#define MaxRTPIdx       (MaxRTPCenter/2+1)

/***** globe variable *****/
static FILE *fptr;

static int RoundBet = 0;

static __int64 SymbolHit[SYM_MAX][Reel_Size_Row] = {0};

static __int64 Round = 0;

static __int64 TotalWin = 0;
static __int64 TotalLineWin = 0;
static __int64 TotalSBWin = 0;
static __int64 TotalSFWin = 0;

static __int64 LineHit = 0;
static __int64 SBHit = 0;
static __int64 SFHit = 0;

static __int64 SFWinX[MaxFeatureWinX] = {0};
static __int64 SBWinX[MaxFeatureWinX] = {0};

static int SFRetrigerHit = 0;

static STestConfig LogTestConfig;

static __int64 TotalWinTimes = 0;

static FILE *RoundFptr;
static FILE *PeriodFptr;

static __int64 RoundWinCount = 0;
static __int64 PeriodWinCount = 0;

static int PeriodRTPCount_Base2[MaxRTPCount] = {0};

/***** globe variable *****/


void InitializeLog()
{
    GetTestConfig(&LogTestConfig);

    RoundBet = LogTestConfig.Play_Line*LogTestConfig.Lines_Bet;
    //get time data
    time_t t1 = time(NULL);
    struct tm *nPtr = localtime(&t1);
    asctime(nPtr);

    //open log file
    char LogFileName[50];
    int LogFileNameLength = sprintf(LogFileName, LogFilePath[(int)LogTestConfig.Test_Mode]);
    sprintf(LogFileName+LogFileNameLength, "_%dm%dd%dh%dm.txt", nPtr->tm_mon+1, nPtr->tm_mday, nPtr->tm_hour, nPtr->tm_min);
    fptr = fopen(LogFileName,"w");

    if (LogTestConfig.Test_Mode == ETest_RandomAllGame) {
        RoundFptr = fopen(LogFilePath[6],"w");
        PeriodFptr = fopen(LogFilePath[7],"w");
    }
}

void ReportStatistic()
{
    int OddsTable[Symbol_Number_Limit][Reel_Size_Row];
    ReadOddsTable((int *)OddsTable);

    if (LogTestConfig.Test_Mode == ETest_FullCycleMainGame || LogTestConfig.Test_Mode == ETest_FullCycleFreeGame) {
        fprintf(fptr, "Play Line = %d, Lines Bet = %d, ", LogTestConfig.Play_Line, LogTestConfig.Lines_Bet);
        TotalWin = TotalLineWin + TotalSFWin + TotalSBWin;
        fprintf(fptr, "Round = %I64d \n", Round);
        fprintf(fptr, "Total Win = %I64d \n", TotalWin);

        double RTP      = (double)(TotalWin)        / (double)(Round*RoundBet);
        double LineRTP  = (double)(TotalLineWin)    / (double)(Round*RoundBet);
        double SFRTP    = (double)(TotalSFWin)      / (double)(Round*RoundBet);
        double SBRTP    = (double)(TotalSBWin)      / (double)(Round*RoundBet);
        fprintf(fptr, "RTP = %10.4f, Line Win RTP = %10.4f, SF RTP = %10.4f, SB RTP = %10.4f\n", RTP*100, LineRTP*100, SFRTP*100, SBRTP*100);

        fprintf(fptr, "Line Hits = %I64d, SB Hits = %I64d, SF Hits = %I64d\n", LineHit, SBHit, SFHit);
        double HitRatio = (double)(LineHit) / (double)(Round);
        double SBFreq = (double)(Round) / (double)(SBHit);
        double SFFreq = (double)(Round) / (double)(SFHit);
        fprintf(fptr, "Hit%c = %.4f, SB Freq = %.4f, SF Freq = %.4f\n", '%', HitRatio*100, SBFreq, SFFreq);

        for (int SymbolCount = Reel_Size_Row-1; SymbolCount >= 2; SymbolCount--) {
            fprintf(fptr, "\n%d in a row : \n", SymbolCount+1);
            for (int SymbolIdx = 0; SymbolIdx <= LowestPaySymbol; SymbolIdx++) {
                fprintf(fptr, "    %s : Hit = %I64d\n", SymbolName[SymbolIdx], SymbolHit[SymbolIdx][SymbolCount]);
            }
        }
    }
    else if (LogTestConfig.Test_Mode == ETest_RandomMainGame) {
        fprintf(fptr, "Play Line = %d, Lines Bet = %d, ", LogTestConfig.Play_Line, LogTestConfig.Lines_Bet);
        TotalWin = TotalLineWin + TotalSFWin + TotalSBWin;
        fprintf(fptr, "Round = %I64d \n", Round);
        fprintf(fptr, "Total Win = %I64d \n", TotalWin);

        double RTP      = (double)(TotalWin)        / (double)(Round*RoundBet);
        double LineRTP  = (double)(TotalLineWin)    / (double)(Round*RoundBet);
        double SFRTP    = (double)(TotalSFWin)      / (double)(Round*RoundBet);
        double SBRTP    = (double)(TotalSBWin)      / (double)(Round*RoundBet);
        fprintf(fptr, "RTP = %10.4f, Line Win RTP = %10.4f, SF RTP = %10.4f, SB RTP = %10.4f\n", RTP*100, LineRTP*100, SFRTP*100, SBRTP*100);

        double HitRatio = (double)(LineHit) / (double)(Round);
        double SBFreq = (double)(Round) / (double)(SBHit);
        double SFFreq = (double)(Round) / (double)(SFHit);
        fprintf(fptr, "Hit%c = %.4f, SB Freq = %.4f, SF Freq = %.4f\n", '%', HitRatio*100, SBFreq, SFFreq);

        for (int SymbolCount = Reel_Size_Row-1; SymbolCount >= 2; SymbolCount--) {
            fprintf(fptr, "\n%d in a row : \n", SymbolCount+1);
            for (int SymbolIdx = 0; SymbolIdx <= LowestPaySymbol; SymbolIdx++) {
                fprintf(fptr, "    %s : Hit = %11.8f%c, RTP = %11.8f%c\n", SymbolName[SymbolIdx], (double)SymbolHit[SymbolIdx][SymbolCount]*100/(double)(Round), '%', SymbolHit[SymbolIdx][SymbolCount]*OddsTable[SymbolIdx][SymbolCount]*100/(double)(Round*RoundBet), '%');
            }
        }
    }
    else if (LogTestConfig.Test_Mode == ETest_RandomFreeGame) {
        __int64 SpinRound = Round*FG_Spin + SFRetrigerHit*FG_Retrigger_Spin;
        fprintf(fptr, "Play Line = %d, Lines Bet = %d, ", LogTestConfig.Play_Line, LogTestConfig.Lines_Bet);
        TotalWin = TotalLineWin + TotalSFWin + TotalSBWin;
        fprintf(fptr, "Round = %I64d \n", Round);
        fprintf(fptr, "Total Win = %I64d \n", TotalWin);

        double RTP      = (double)(TotalWin)        / (double)(Round*RoundBet);
        double LineRTP  = (double)(TotalLineWin)    / (double)(Round*RoundBet);
        double SFRTP    = (double)(TotalSFWin)      / (double)(Round*RoundBet);
        double SBRTP    = (double)(TotalSBWin)      / (double)(Round*RoundBet);
        fprintf(fptr, "RTP = %10.4f, Line Win RTP = %10.4f, SF RTP = %10.4f, SB RTP = %10.4f\n", RTP*100, LineRTP*100, SFRTP*100, SBRTP*100);

        double HitRatio = (double)(LineHit) / (double)(SpinRound);
        double SBFreq = (double)(SpinRound) / (double)(SBHit);
        double SFFreq = (double)(SpinRound) / (double)(SFHit);
        fprintf(fptr, "Hit%c = %.4f, SB Freq = %.4f, SF Freq = %.4f\n", '%', HitRatio*100, SBFreq, SFFreq);

        for (int SymbolCount = Reel_Size_Row-1; SymbolCount >= 2; SymbolCount--) {
            fprintf(fptr, "\n%d in a row : \n", SymbolCount+1);
            for (int SymbolIdx = 0; SymbolIdx <= LowestPaySymbol; SymbolIdx++) {
                fprintf(fptr, "    %s : Hit = %11.8f%c, RTP = %11.8f%c\n", SymbolName[SymbolIdx], (double)SymbolHit[SymbolIdx][SymbolCount]*100/(double)(Round), '%', SymbolHit[SymbolIdx][SymbolCount]*OddsTable[SymbolIdx][SymbolCount]*100/(double)(Round*RoundBet), '%');
            }
        }

        fprintf(fptr, "\nSF Win X : \n");
        for (int SFWinXIdx = 0; SFWinXIdx < MaxFeatureWinX; SFWinXIdx++) {
            fprintf(fptr, " %3d <= X < %3d : %10.4f%c\n", SFWinXIdx, SFWinXIdx+1, (double)(SFWinX[SFWinXIdx]*100)/(double)(Round), '%' );
        }

    }
    else if (LogTestConfig.Test_Mode == ETest_RandomBonusGame) {
        fprintf(fptr, "Play Line = %d, Lines Bet = %d, ", LogTestConfig.Play_Line, LogTestConfig.Lines_Bet);
        fprintf(fptr, "Round = %I64d \n", Round);
        fprintf(fptr, "Total SB Win = %I64d \n", TotalSBWin);

        double AvgSBCredit = (double)(TotalSBWin) / (double)(Round*RoundBet);
        fprintf(fptr, "Avg SB Credit = %10.4f\n", AvgSBCredit);

        fprintf(fptr, "SB Win X : \n");
        for (int SBWinXIdx = 0; SBWinXIdx < MaxFeatureWinX; SBWinXIdx++) {
            fprintf(fptr, " %3d <= X < %3d : %10.4f%c\n", SBWinXIdx, SBWinXIdx+1, (double)(SBWinX[SBWinXIdx]*100)/(double)(Round), '%' );
        }
    }
    else if (LogTestConfig.Test_Mode == ETest_RandomAllGame) {
        fprintf(fptr, "Play Line = %d, Lines Bet = %d, ", LogTestConfig.Play_Line, LogTestConfig.Lines_Bet);
        TotalWin = TotalLineWin + TotalSFWin + TotalSBWin;
        fprintf(fptr, "Round = %I64d, Period = %d \n", Round, LogTestConfig.StatisticPeriod);
        fprintf(fptr, "Total Win = %I64d \n", TotalWin);

        double RTP      = (double)(TotalWin)        / (double)(Round*RoundBet);
        double MGRTP    = (double)(TotalLineWin)    / (double)(Round*RoundBet);
        double FGRTP    = (double)(TotalSFWin)      / (double)(Round*RoundBet);
        double BGRTP    = (double)(TotalSBWin)      / (double)(Round*RoundBet);
        fprintf(fptr, "RTP = %10.4f, MG RTP = %10.4f, FG RTP = %10.4f, BG RTP = %10.4f\n", RTP*100, MGRTP*100, FGRTP*100, BGRTP*100);

        double HitRatio = (double)(LineHit) / (double)(Round);
        double WinRatio = (double)(TotalWinTimes) / (double)(Round);
        double SBFreq = (double)(Round) / (double)(SBHit);
        double SFFreq = (double)(Round) / (double)(SFHit);
        fprintf(fptr, "Hit%c = %.4f, Win%c = %.4f, SB Freq = %.4f, SF Freq = %.4f\n", '%', HitRatio*100, '%', WinRatio*100, SBFreq, SFFreq);


        for (int SymbolCount = Reel_Size_Row-1; SymbolCount >= 2; SymbolCount--) {
            fprintf(fptr, "\n%d in a row : \n", SymbolCount+1);
            for (int SymbolIdx = 0; SymbolIdx <= LowestPaySymbol; SymbolIdx++) {
                fprintf(fptr, "    %s : Hit = %11.8f%c, RTP = %11.8ff%c\n", SymbolName[SymbolIdx], (double)SymbolHit[SymbolIdx][SymbolCount]*100/(double)(Round), '%', SymbolHit[SymbolIdx][SymbolCount]*OddsTable[SymbolIdx][SymbolCount]*100/(double)(Round*RoundBet), '%');
            }
        }

        fprintf(fptr, "\nPeriod RTP Counts : From %d to %d\n", MinRTPCenter, MaxRTPCenter);
        for (int RTPIdx = MinRTPIdx; RTPIdx <= MaxRTPIdx; RTPIdx++) {
            fprintf(fptr, "%d\n", PeriodRTPCount_Base2[RTPIdx]);
        }

        fclose(RoundFptr);
        fclose(PeriodFptr);
    }
    PrintOddsTable();
    PrintReelStripe();

    fclose(fptr);
}

void CalculateStatistic(MainGameOutput MGOut)
{
    if (LogTestConfig.Test_Mode == ETest_FullCycleMainGame || LogTestConfig.Test_Mode == ETest_RandomMainGame || LogTestConfig.Test_Mode == ETest_RandomAllGame) {
        for (int LineIdx = 0; LineIdx < LogTestConfig.Play_Line; LineIdx++) {
            AddTotalLineWin(MGOut.LineWin[LineIdx]);
            AddSymbolHit(MGOut.LineSymbol[LineIdx], MGOut.LineSymbolCount[LineIdx]-1);
        }
        AddTotalSFWin(MGOut.SFWin);
        AddTotalSBWin(MGOut.SBWin);

        if (MGOut.WinType & EWinType_WinMG) {
            LineHit++;
        }
        if (MGOut.WinType & EWinType_WinBG) {
            SBHit++;
        }
        if (MGOut.WinType & EWinType_WinFG) {
            SFHit++;
        }
    }
}

void CalculateStatistic(MainGameOutput MGOut, int *ThisRoundWin)
{
    CalculateStatistic(MGOut);
    for (int LineIdx = 0; LineIdx < LogTestConfig.Play_Line; LineIdx++) {
        *ThisRoundWin = *ThisRoundWin + MGOut.LineWin[LineIdx];
    }
}

void CalculateStatistic(FreeGameOutput FGOut)
{
    if (LogTestConfig.Test_Mode == ETest_FullCycleFreeGame || LogTestConfig.Test_Mode == ETest_RandomFreeGame) {
        for (int LineIdx = 0; LineIdx < LogTestConfig.Play_Line; LineIdx++) {
            AddTotalLineWin(FGOut.LineWin[LineIdx]);
            AddSymbolHit(FGOut.LineSymbol[LineIdx], FGOut.LineSymbolCount[LineIdx]-1);
        }
        AddTotalSFWin(FGOut.SFWin);

        if (FGOut.WinType & EWinType_WinMG) {
            LineHit++;
        }
        if (FGOut.WinType & EWinType_WinBG) {
            SBHit++;
        }
        if (FGOut.WinType & EWinType_WinFG) {
            SFHit++;
            SFRetrigerHit++;
        }
    }
    else if (LogTestConfig.Test_Mode == ETest_RandomAllGame) {
        for (int LineIdx = 0; LineIdx < LogTestConfig.Play_Line; LineIdx++) {
            AddTotalSFWin(FGOut.LineWin[LineIdx]);
        }
        AddTotalSFWin(FGOut.SFWin);
    }
}

void CalculateStatistic(FreeGameOutput FGOut, int *ThisRoundWin)
{
    CalculateStatistic(FGOut);
    for (int LineIdx = 0; LineIdx < LogTestConfig.Play_Line; LineIdx++) {
        *ThisRoundWin = *ThisRoundWin + FGOut.LineWin[LineIdx];
    }
    *ThisRoundWin = *ThisRoundWin + FGOut.SFWin;
}

void CalculateStatistic(BonusGameOutput BGOut)
{
    if (LogTestConfig.Test_Mode == ETest_RandomBonusGame) {
        AddTotalSBWin(BGOut.BGWin);
    }
}

void CalculateStatistic(BonusGameOutput BGOut, int *ThisRoundWin, bool FromFG)
{
    if (LogTestConfig.Test_Mode == ETest_RandomAllGame || LogTestConfig.Test_Mode == ETest_RandomFreeGame) {
        if (FromFG) {
            AddTotalSFWin(BGOut.BGWin);
        }
        else {
            AddTotalSBWin(BGOut.BGWin);
        }
        *ThisRoundWin = *ThisRoundWin + BGOut.BGWin;
    }
}

void CalculateStatistic(int ThisRoundWin)
{
    if (LogTestConfig.Test_Mode == ETest_RandomAllGame) {
        if (RoundWinCount < MaxFileWinRecord) {
            fprintf(RoundFptr, "%d\n", ThisRoundWin);
            RoundWinCount++;
        }
        if (ThisRoundWin > RoundBet) {
            TotalWinTimes++;
        }
    }
    else if (LogTestConfig.Test_Mode == ETest_RandomFreeGame) {
        int ThisSFWinX = ThisRoundWin/RoundBet;
        if (ThisSFWinX < MaxFeatureWinX) {
            SFWinX[ThisSFWinX]++;
        }
    }
}

void CalculatePeriodStatistic(int ThisPeriodWin)
{
    if (PeriodWinCount < MaxFileWinRecord) {
        fprintf(PeriodFptr, "%d\n", ThisPeriodWin);
        PeriodWinCount++;
    }

    /***
         here we add RTP count record,
         ex: this period RTP = 95.00~96.99,
             first int PeriodRTP = (PeriodWin*100)/(RoundBet) = 95~96, then +1 => 96~97, then /2 = 48
             we do PeriodRTPCount_Base2[48]++,
         So  PeriodRTPCount_Base2[0] = -1~1 RTP count (center 0), PeriodRTPCount_Base2[1] = 1~3 RTP count (center 2).........
    ***/
    int PeriodRTP = (ThisPeriodWin*100)/(LogTestConfig.StatisticPeriod*RoundBet);
    int PeriodRTPIdx = (PeriodRTP+1)/2;
    if (PeriodRTPIdx < MaxRTPCount) {
        PeriodRTPCount_Base2[PeriodRTPIdx]++;
    }

}

void AddSymbolHit(ESymbolList SymbolIdx, int SymbolCount)
{
    SymbolHit[SymbolIdx][SymbolCount]++;
}

void AddTotalLineWin(int Credit)
{
    TotalLineWin = TotalLineWin + Credit;
}

void AddTotalSFWin(int Credit)
{
    TotalSFWin = TotalSFWin + Credit;
}

void AddTotalSBWin(int Credit)
{
    TotalSBWin = TotalSBWin + Credit;

    int ThisSBWinX = Credit/RoundBet;
    if (ThisSBWinX < MaxFeatureWinX) {
        SBWinX[ThisSBWinX]++;
    }
}

void SetRound(int PlayRound)
{
    Round = PlayRound;
}

void PrintOddsTable()
{
    int OddsTable[Symbol_Number_Limit][Reel_Size_Row];
    ReadOddsTable((int *)OddsTable);

    fprintf(fptr, "\n\nOdds Table : \n");
    for (int SymIdx = 0; SymIdx < SYM_MAX; SymIdx++) {
        fprintf(fptr, "%s : ", SymbolName[SymIdx]);
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            fprintf(fptr, "%5d ", OddsTable[SymIdx][RowIdx]);
        }
        fprintf(fptr, "\n");
    }
}

void PrintReelStripe()
{
    int ReelLength[Reel_Size_Row];
    ESymbolList Reel[Reel_Size_Row][Reel_Length_Limit];

    fprintf(fptr, "\n\n");
    if (LogTestConfig.Test_Mode == ETest_FullCycleMainGame || LogTestConfig.Test_Mode == ETest_RandomMainGame || LogTestConfig.Test_Mode == ETest_RandomAllGame) {

        ReadMainGameReelStripe(ReelLength, (ESymbolList *)Reel);
        fprintf(fptr, "Main Game Reel Stripe : \n");
        //fprintf(fptr, "R1 R2 R3 R4 R5\n");

        int MaxReelLength = 0;
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            if (MaxReelLength < ReelLength[RowIdx]) {
                MaxReelLength = ReelLength[RowIdx];
            }
        }

        for (int ReelIdx = 0; ReelIdx < MaxReelLength; ReelIdx++) {
            if (ReelIdx%5 == 0) {
                    fprintf(fptr, "\n");
            }
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                if (ReelIdx < ReelLength[RowIdx]) {
                    fprintf(fptr, "%s  ", SymbolName[Reel[RowIdx][ReelIdx]]);
                }
                else {
                    fprintf(fptr, "    ");
                }
            }
            fprintf(fptr, "\n");
        }
    }

    if (LogTestConfig.Test_Mode == ETest_FullCycleFreeGame || LogTestConfig.Test_Mode == ETest_RandomFreeGame || LogTestConfig.Test_Mode == ETest_RandomAllGame) {
        ReadFreeGameReelStripe(ReelLength, (ESymbolList *)Reel);
        fprintf(fptr, "Free Game Reel Stripe : \n");
        //fprintf(fptr, "R1 R2 R3 R4 R5\n");

        int MaxReelLength = 0;
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            if (MaxReelLength < ReelLength[RowIdx]) {
                MaxReelLength = ReelLength[RowIdx];
            }
        }

        for (int ReelIdx = 0; ReelIdx < MaxReelLength; ReelIdx++) {
            if (ReelIdx%5 == 0) {
                    fprintf(fptr, "\n");
            }
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                if (ReelIdx < ReelLength[RowIdx]) {
                    fprintf(fptr, "%s  ", SymbolName[Reel[RowIdx][ReelIdx]]);
                }
                else {
                    fprintf(fptr, "    ");
                }
            }
            fprintf(fptr, "\n");
        }
    }
}

void GetTestConfig(STestConfig *TestConfig)
{
    int ConfigNumberArray[TestConfigNumbers];
    ReadTestConfig(ConfigNumberArray);

    TestConfig->Test_Mode       = (ETestMode)ConfigNumberArray[0];
    TestConfig->MGPay_Mode      = (EPayMode)ConfigNumberArray[1];
    TestConfig->FGPay_Mode      = (EPayMode)ConfigNumberArray[2];
    TestConfig->MGWild_Mode     = (EWildMode)ConfigNumberArray[3];
    TestConfig->FGWild_Mode     = (EWildMode)ConfigNumberArray[4];
    TestConfig->Play_Line       = ConfigNumberArray[5];
    TestConfig->Lines_Bet       = ConfigNumberArray[6];
    TestConfig->Test_Round      = ConfigNumberArray[7];
    TestConfig->StatisticPeriod = ConfigNumberArray[8];
/*
    printf("Test_Mode = %d\n", (int)Test_Mode);
    printf("MGPay_Mode = %d\n", (int)MGPay_Mode);
    printf("FGPay_Mode = %d\n", (int)FGPay_Mode);
    printf("MGWild_Mode = %d\n", (int)MGWild_Mode);
    printf("FGWild_Mode = %d\n", (int)FGWild_Mode);
    printf("Play_Line = %d\n", (int)Play_Line);
    printf("Lines_Bet = %d\n", (int)Lines_Bet);
    printf("Test_Round = %d\n", (int)Test_Round);
    system("pause");
*/
}
