#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeGame.h"

void FreeGame::GetFreeGameConfig()
{
    ReadFreeGameReelStripe(ReelLength, (ESymbolList *)Reel);
    ReadOddsTable((int *)OddsTable);
    ReadLinesTable((int *)LinesTable);
}

void FreeGame::FreeGameSpin(const FreeGameInput FGIn, FreeGameOutput *FGOut, int ReelPosition[Reel_Size_Row])
{
    //Initialize FGOut
    {
        for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                FGOut->SymbolResult[ColumnIdx][RowIdx] = (ESymbolList)0;
            }
        }

        for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
            FGOut->LineSymbol[LineIdx] = (ESymbolList)0;
            FGOut->LineSymbolCount[LineIdx] = 0;
            FGOut->LineWin[LineIdx] = 0;
        }
        FGOut->SFWin = 0;
        FGOut->SFCount = 0;
        FGOut->SBWin = 0;
        FGOut->SBCount = 0;
        FGOut->WinType = EWinType_NoWin;
    }

    //Fill in Reel Position to Output
    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
		for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            FGOut->SymbolResult[ColumnIdx][RowIdx] = Reel[RowIdx][ReelPosition[RowIdx]+ColumnIdx];
		}
		//printf("{%d, %d, %d, %d, %d}\n", FGOut->SymbolResult[ColumnIdx][0], FGOut->SymbolResult[ColumnIdx][1], FGOut->SymbolResult[ColumnIdx][2], FGOut->SymbolResult[ColumnIdx][3], FGOut->SymbolResult[ColumnIdx][4]);
	}
    //printf("\n\n");

	if (FGIn.PayMode == EPayMode_Lines_Left || FGIn.PayMode == EPayMode_Lines_Left_Right) {
        GetLineWin(FGIn, FGOut);
	}
	else if (FGIn.PayMode == EPayMode_Ways_Left) {
        GetWayWin(FGIn, FGOut);
	}

    GetScatterWin(FGIn, FGOut);
}

void FreeGame::GetLineWin(const FreeGameInput FGIn, FreeGameOutput *FGOut)
{
    ESymbolList ThisSymbolResult[Reel_Size_Column][Reel_Size_Row];
    memcpy(ThisSymbolResult, FGOut->SymbolResult, sizeof(ThisSymbolResult));

    if (FGIn.WildMode == EWildMode_Expend) {
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
                ESymbolList ThisSymbol = ThisSymbolResult[ColumnIdx][RowIdx];
                if (ThisSymbol == WD) {
                    ThisSymbolResult[0][RowIdx] = WD;
                    ThisSymbolResult[1][RowIdx] = WD;
                    ThisSymbolResult[2][RowIdx] = WD;
                    break;
                }
            }
        }
    }

    for (int LineIdx = 0; LineIdx < FGIn.Lines; LineIdx++) {
		int Line[Reel_Size_Row], SymbolCount = 0;
		memcpy(Line, LinesTable[LineIdx], sizeof(Line));

		ESymbolList FirstSymbol = ThisSymbolResult[Line[0]][0];

        /***** if FirstSymbol == WD, we need use another method to get line credit *****/
		if (JoudgeSymbolType(FirstSymbol) == ESymbolType_Normal) {
			SymbolCount++;
			if (ThisSymbolResult[Line[1]][1] == WD || ThisSymbolResult[Line[1]][1] == FirstSymbol) {
				SymbolCount++;
				if (ThisSymbolResult[Line[2]][2] == WD || ThisSymbolResult[Line[2]][2] == FirstSymbol) {
					SymbolCount++;
					if (ThisSymbolResult[Line[3]][3] == WD || ThisSymbolResult[Line[3]][3] == FirstSymbol) {
						SymbolCount++;
						if (ThisSymbolResult[Line[4]][4] == WD || ThisSymbolResult[Line[4]][4] == FirstSymbol) {
							SymbolCount++;
						}
					}
				}
			}
			//printf("Line = {%d, %d, %d, %d, %d}, Pay = %d\n", Line[0], Line[1], Line[2], Line[3], Line[4], OddsTable[ThisSymbolResult[Line[0]][0]][SymbolCount-1]);
			FGOut->LineSymbol[LineIdx] = FirstSymbol;
            FGOut->LineSymbolCount[LineIdx] = SymbolCount;
            FGOut->LineWin[LineIdx] = OddsTable[(int)FirstSymbol][SymbolCount-1] * FGIn.LinesBet * FG_Multiplier;
		}

        if (FGIn.PayMode == EPayMode_Lines_Left_Right) {
            /***** Here we assume that one line only get one credit *****/
            if (FGOut->LineWin[LineIdx] > 0 || SymbolCount == Reel_Size_Row) {
                continue;
            }

            SymbolCount = 0;
            FirstSymbol = ThisSymbolResult[Line[4]][4];

            if (JoudgeSymbolType(FirstSymbol) == ESymbolType_Normal) {
                SymbolCount++;
                if (ThisSymbolResult[Line[3]][3] == WD || ThisSymbolResult[Line[3]][3] == FirstSymbol) {
                    SymbolCount++;
                    if (ThisSymbolResult[Line[2]][2] == WD || ThisSymbolResult[Line[2]][2] == FirstSymbol) {
                        SymbolCount++;
                        if (ThisSymbolResult[Line[1]][1] == WD || ThisSymbolResult[Line[1]][1] == FirstSymbol) {
                            SymbolCount++;
                        }
                    }
                }

                FGOut->LineSymbol[LineIdx] = FirstSymbol;
                FGOut->LineSymbolCount[LineIdx] = SymbolCount;
                FGOut->LineWin[LineIdx] = OddsTable[(int)FirstSymbol][SymbolCount-1] * FGIn.LinesBet * FG_Multiplier;
            }
        }
	}

	for (int LineIdx = 0; LineIdx < FGIn.Lines; LineIdx++) {
	    if (FGOut->LineWin[LineIdx] > 0) {
            FGOut->WinType = (EWinType)(FGOut->WinType | EWinType_WinMG);
            break;
	    }
	}
}

void FreeGame::GetWayWin(const FreeGameInput FGIn, FreeGameOutput *FGOut)
{

}

void FreeGame::GetScatterWin(const FreeGameInput FGIn, FreeGameOutput *FGOut)
{
    int SBCount = 0, SFCount = 0;
    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
		for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            ESymbolList ThisSymbol = FGOut->SymbolResult[ColumnIdx][RowIdx];
            if (ThisSymbol == SB) {
                SBCount++;
            }
            if (ThisSymbol == SF) {
                SFCount++;
            }
		}
	}

    if (SBCount > 0) {
        FGOut->SBWin = OddsTable[(int)SB][SBCount-1] * FGIn.Lines * FGIn.LinesBet * FG_Multiplier;
        if (SBCount >= SB_Trigger_Require) {
            FGOut->WinType = (EWinType)(FGOut->WinType | EWinType_WinBG);
            FGOut->SBCount = SBCount;
        }
    }

    if (SFCount > 0) {
        //FGOut->SFWin = OddsTable[(int)SF][SFCount-1] * FGIn.Lines * FGIn.LinesBet * FG_Multiplier;
        /*** here I use special method, because I do not want to create FG_OddsTable ***/
        FGOut->SFWin = 1 * FGIn.Lines * FGIn.LinesBet * FG_Multiplier;
        if (SFCount >= SF_ReTrigger_Require) {
            FGOut->WinType = (EWinType)(FGOut->WinType | EWinType_WinFG);
            FGOut->SFCount = SFCount;
        }
    }
}

ESymbolType FreeGame::JoudgeSymbolType(ESymbolList ThisSymbol)
{
    if (ThisSymbol == H1 || ThisSymbol == H2 || ThisSymbol == H3 || ThisSymbol ==  H4 || ThisSymbol == H5 ||
        ThisSymbol == LA || ThisSymbol == LK || ThisSymbol == LQ ) {
        return ESymbolType_Normal;
    }
    else if (ThisSymbol == WD){
        return ESymbolType_Wild;
    }
    else if (ThisSymbol == SF || ThisSymbol == SB) {
        return ESymbolList_Scatter;
    }
}
