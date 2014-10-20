#include <cstdlib>
#include <iostream>
#include <time.h>
#include <stdio.h>

#include "test.h"

using namespace std;

int main()
{
    MainGame MG; FreeGame FG; BonusGame BG;
    MainGameInput MGIn; FreeGameInput FGIn; BonusGameInput BGIn;
    STestConfig ThisTestConfig;
    Initialize(&MGIn, &FGIn, &BGIn, &ThisTestConfig);

     //ReelPosition[Reel_Size_Row] is the position of column 0
    int ReelPosition[Reel_Size_Row];
    if (ThisTestConfig.Test_Mode == ETest_FullCycleMainGame) {
        MG.GetMainGameConfig();
        SetRound(MG.ReelLength[0]*MG.ReelLength[1]*MG.ReelLength[2]*MG.ReelLength[3]*MG.ReelLength[4]);
        for (int R0Idx = 0; R0Idx < MG.ReelLength[0]; R0Idx++) { ReelPosition[0] = R0Idx;
        for (int R1Idx = 0; R1Idx < MG.ReelLength[1]; R1Idx++) { ReelPosition[1] = R1Idx;
        for (int R2Idx = 0; R2Idx < MG.ReelLength[2]; R2Idx++) { ReelPosition[2] = R2Idx;
        for (int R3Idx = 0; R3Idx < MG.ReelLength[3]; R3Idx++) { ReelPosition[3] = R3Idx;
        for (int R4Idx = 0; R4Idx < MG.ReelLength[4]; R4Idx++) { ReelPosition[4] = R4Idx;
            MainGameOutput MGOut;
            MG.MainGameSpin(MGIn, &MGOut, ReelPosition);
            CalculateStatistic(MGOut);
        }}}}}
    }
    else if (ThisTestConfig.Test_Mode == ETest_RandomMainGame) {
        MG.GetMainGameConfig();
        SetRound(ThisTestConfig.Test_Round);
        for (int RoundIdx = 0; RoundIdx < ThisTestConfig.Test_Round; RoundIdx++) {
            if (RoundIdx%1000000 == 0 && RoundIdx > 0) {
                printf("#");
                if (RoundIdx%10000000 == 0) {
                    printf(" %d\n", RoundIdx);
                }
            }
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                ReelPosition[RowIdx] = RNG(0, MG.ReelLength[RowIdx]-1);
            }
            MainGameOutput MGOut;
            MG.MainGameSpin(MGIn, &MGOut, ReelPosition);
            CalculateStatistic(MGOut);
        }
    }
    else if (ThisTestConfig.Test_Mode == ETest_FullCycleFreeGame) {
        FG.GetFreeGameConfig();
        SetRound(FG.ReelLength[0]*FG.ReelLength[1]*FG.ReelLength[2]*FG.ReelLength[3]*FG.ReelLength[4]);
        for (int R0Idx = 0; R0Idx < FG.ReelLength[0]; R0Idx++) { ReelPosition[0] = R0Idx;
        for (int R1Idx = 0; R1Idx < FG.ReelLength[1]; R1Idx++) { ReelPosition[1] = R1Idx;
        for (int R2Idx = 0; R2Idx < FG.ReelLength[2]; R2Idx++) { ReelPosition[2] = R2Idx;
        for (int R3Idx = 0; R3Idx < FG.ReelLength[3]; R3Idx++) { ReelPosition[3] = R3Idx;
        for (int R4Idx = 0; R4Idx < FG.ReelLength[4]; R4Idx++) { ReelPosition[4] = R4Idx;
            FreeGameOutput FGOut;
            FG.FreeGameSpin(FGIn, &FGOut, ReelPosition);
            CalculateStatistic(FGOut);
        }}}}}
    }
    else if (ThisTestConfig.Test_Mode == ETest_RandomFreeGame) {
        FG.GetFreeGameConfig();
        BG.GetBonusGameConfig(); /*** In FG we can win BG ***/
        SetRound(ThisTestConfig.Test_Round);

        int ThisRoundSFWin = 0;
        for (int RoundIdx = 0; RoundIdx < ThisTestConfig.Test_Round; RoundIdx++) {
            if (RoundIdx%1000000 == 0 && RoundIdx > 0) {
                printf("#");
                if (RoundIdx%10000000 == 0) {
                    printf(" %d\n", RoundIdx);
                }
            }
            for (int SpinCount = FG_Spin; SpinCount > 0; SpinCount--) {
                for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                    ReelPosition[RowIdx] = RNG(0, FG.ReelLength[RowIdx]-1);
                }
                FreeGameOutput FGOut;
                FG.FreeGameSpin(FGIn, &FGOut, ReelPosition);
                CalculateStatistic(FGOut, &ThisRoundSFWin);

                if (FGOut.WinType & EWinType_WinFG) {
                    SpinCount = SpinCount + FG_Retrigger_Spin;
                }

                if (FGOut.WinType & EWinType_WinBG) {
                    BonusGameOutput BGOut;
                    BG.GetBonusGameCredit(BGIn, &BGOut);
                    CalculateStatistic(BGOut, &ThisRoundSFWin, true/*** for adding credit to SFWin***/);
                }
            }
            CalculateStatistic(ThisRoundSFWin);
            ThisRoundSFWin = 0;
        }
    }
    else if (ThisTestConfig.Test_Mode == ETest_RandomBonusGame) {
        BG.GetBonusGameConfig();
        SetRound(ThisTestConfig.Test_Round);
        for (int RoundIdx = 0; RoundIdx < ThisTestConfig.Test_Round; RoundIdx++) {
            if (RoundIdx%1000000 == 0 && RoundIdx > 0) {
                printf("#");
                if (RoundIdx%10000000 == 0) {
                    printf(" %d\n", RoundIdx);
                }
            }
            BonusGameOutput BGOut;
            BG.GetBonusGameCredit(BGIn, &BGOut);
            CalculateStatistic(BGOut);
        }
    }
    else if (ThisTestConfig.Test_Mode = ETest_RandomAllGame) {
        MG.GetMainGameConfig();
        FG.GetFreeGameConfig();
        BG.GetBonusGameConfig();
        SetRound(ThisTestConfig.Test_Round);

        int ThisRoundWin = 0;
        int ThisPeriodWin = 0;
        for (int RoundIdx = 0; RoundIdx < ThisTestConfig.Test_Round; RoundIdx++) {
            if (RoundIdx%1000000 == 0 && RoundIdx > 0) {
                printf("#");
                if (RoundIdx%10000000 == 0) {
                    printf(" %d\n", RoundIdx);
                }
            }
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                ReelPosition[RowIdx] = RNG(0, MG.ReelLength[RowIdx]-1);
            }
            MainGameOutput MGOut;
            MG.MainGameSpin(MGIn, &MGOut, ReelPosition);
            CalculateStatistic(MGOut, &ThisRoundWin);


            if (MGOut.WinType & EWinType_WinFG) {
                for (int SpinCount = FG_Spin; SpinCount > 0; SpinCount--) {
                    for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                        ReelPosition[RowIdx] = RNG(0, FG.ReelLength[RowIdx]-1);
                    }
                    FreeGameOutput FGOut;
                    FG.FreeGameSpin(FGIn, &FGOut, ReelPosition);
                    CalculateStatistic(FGOut, &ThisRoundWin);

                    if (FGOut.WinType & EWinType_WinFG) {
                        SpinCount = SpinCount + FG_Retrigger_Spin;
                    }

                    if (FGOut.WinType & EWinType_WinBG) {
                        BonusGameOutput BGOut;
                        BG.GetBonusGameCredit(BGIn, &BGOut);
                        CalculateStatistic(BGOut, &ThisRoundWin, true/*** for adding credit to SFWin***/);
                    }
                }
            }

            if (MGOut.WinType & EWinType_WinBG) {
                BonusGameOutput BGOut;
                BG.GetBonusGameCredit(BGIn, &BGOut);
                CalculateStatistic(BGOut, &ThisRoundWin);
            }
            CalculateStatistic(ThisRoundWin);
            ThisPeriodWin = ThisPeriodWin + ThisRoundWin;

            ThisRoundWin = 0;
            if (RoundIdx%ThisTestConfig.StatisticPeriod == (ThisTestConfig.StatisticPeriod-1) ) {
                CalculatePeriodStatistic(ThisPeriodWin);
                ThisPeriodWin = 0;
            }
        }
    }

    ReportStatistic();

    return 0;
}

void Initialize(MainGameInput *MGIn, FreeGameInput *FGIn, BonusGameInput *BGIn, STestConfig *ThisTestConfig)
{
    GetTestConfig(ThisTestConfig);
    InitializeLog();
    InitializeRNG();

    MGIn->TestMode      = ThisTestConfig->Test_Mode;
    MGIn->PayMode       = ThisTestConfig->MGPay_Mode;
    MGIn->WildMode      = ThisTestConfig->MGWild_Mode;
    MGIn->Lines         = ThisTestConfig->Play_Line;
    MGIn->LinesBet      = ThisTestConfig->Lines_Bet;

    FGIn->TestMode      = ThisTestConfig->Test_Mode;
    FGIn->PayMode       = ThisTestConfig->FGPay_Mode;
    FGIn->WildMode      = ThisTestConfig->FGWild_Mode;
    FGIn->Lines         = ThisTestConfig->Play_Line;
    FGIn->LinesBet      = ThisTestConfig->Lines_Bet;

    BGIn->Lines         = ThisTestConfig->Play_Line;
    BGIn->LinesBet      = ThisTestConfig->Lines_Bet;
}
