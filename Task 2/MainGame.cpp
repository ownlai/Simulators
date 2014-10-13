#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MainGame.h"

void MainGame::GetMainGameConfig()
{
    ReadMainGameReelStripe(ReelLength, (ESymbolList *)Reel);
    ReadOddsTable((int *)OddsTable);
    ReadLinesTable((int *)LinesTable);
}

void MainGame::MainGameSpin(const MainGameInput MGIn, MainGameOutput *MGOut, int ReelPosition[Reel_Size_Row])
{
    //Initialize MGOut
    {
        for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                MGOut->SymbolResult[ColumnIdx][RowIdx] = (ESymbolList)0;
            }
        }

        for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
            MGOut->LineSymbol[LineIdx] = (ESymbolList)0;
            MGOut->LineSymbolCount[LineIdx] = 0;
            MGOut->LineWin[LineIdx] = 0;
        }
        MGOut->SFWin = 0;
        MGOut->SFCount = 0;
        MGOut->SBWin = 0;
        MGOut->SBCount = 0;
        MGOut->WinType = EWinType_NoWin;
    }

    //Fill in Reel Position to Output
    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
		for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            MGOut->SymbolResult[ColumnIdx][RowIdx] = Reel[RowIdx][ReelPosition[RowIdx]+ColumnIdx];
		}
		//printf("{%d, %d, %d, %d, %d}\n", MGOut->SymbolResult[ColumnIdx][0], MGOut->SymbolResult[ColumnIdx][1], MGOut->SymbolResult[ColumnIdx][2], MGOut->SymbolResult[ColumnIdx][3], MGOut->SymbolResult[ColumnIdx][4]);
	}
    //printf("\n\n");

	if (MGIn.PayMode == EPayMode_Lines_Left || MGIn.PayMode == EPayMode_Lines_Left_Right) {
        GetLineWin(MGIn, MGOut);
	}
	else if (MGIn.PayMode == EPayMode_Ways_Left) {
        GetWayWin(MGIn, MGOut);
	}

    GetScatterWin(MGIn, MGOut);
}

void MainGame::GetLineWin(const MainGameInput MGIn, MainGameOutput *MGOut)
{
    ESymbolList ThisSymbolResult[Reel_Size_Column][Reel_Size_Row];
    memcpy(ThisSymbolResult, MGOut->SymbolResult, sizeof(ThisSymbolResult));

    if (MGIn.WildMode == EWildMode_Expend) {
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
                ESymbolList ThisSymbol = ThisSymbolResult[ColumnIdx][RowIdx];
                if (ThisSymbol == EW) {
                    ThisSymbolResult[0][RowIdx] = EW;
                    ThisSymbolResult[1][RowIdx] = EW;
                    ThisSymbolResult[2][RowIdx] = EW;
                    break;
                }
            }
        }
    }

    for (int LineIdx = 0; LineIdx < MGIn.Lines; LineIdx++) {
		int Line[Reel_Size_Row], SymbolCount = 0;
		memcpy(Line, LinesTable[LineIdx], sizeof(Line));

		ESymbolList FirstSymbol = ThisSymbolResult[Line[0]][0];

        /***** if FirstSymbol == WD, we need use another method to get line credit *****/
		if (JoudgeSymbolType(FirstSymbol) == ESymbolType_Normal) {
			SymbolCount++;
			if (/*ThisSymbolResult[Line[1]][1] == WD || */ThisSymbolResult[Line[1]][1] == FirstSymbol) {
				SymbolCount++;
				if (ThisSymbolResult[Line[2]][2] == EW || ThisSymbolResult[Line[2]][2] == MW || ThisSymbolResult[Line[2]][2] == FirstSymbol) {
					SymbolCount++;
					if (/*ThisSymbolResult[Line[3]][3] == WD || */ThisSymbolResult[Line[3]][3] == FirstSymbol) {
						SymbolCount++;
						if (/*ThisSymbolResult[Line[4]][4] == WD || */ThisSymbolResult[Line[4]][4] == FirstSymbol) {
							SymbolCount++;
						}
					}
				}
			}
			//printf("Line = {%d, %d, %d, %d, %d}, Pay = %d\n", Line[0], Line[1], Line[2], Line[3], Line[4], OddsTable[ThisSymbolResult[Line[0]][0]][SymbolCount-1]);
			MGOut->LineSymbol[LineIdx] = FirstSymbol;
            MGOut->LineSymbolCount[LineIdx] = SymbolCount;

            MGOut->LineWin[LineIdx] = OddsTable[(int)FirstSymbol][SymbolCount-1] * MGIn.LinesBet;

            if (ThisSymbolResult[Line[2]][2] == MW) {
                MGOut->LineWin[LineIdx] = MGOut->LineWin[LineIdx]*3;
            }
		}

        if (MGIn.PayMode == EPayMode_Lines_Left_Right) {
            /***** Here we assume that one line only get one credit *****/
            /*if (MGOut->LineWin[LineIdx] > 0 || SymbolCount == Reel_Size_Row) {
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

                MGOut->LineSymbol[LineIdx] = FirstSymbol;
                MGOut->LineSymbolCount[LineIdx] = SymbolCount;
                MGOut->LineWin[LineIdx] = OddsTable[(int)FirstSymbol][SymbolCount-1] * MGIn.LinesBet;
            }*/
        }
	}

	for (int LineIdx = 0; LineIdx < MGIn.Lines; LineIdx++) {
	    if (MGOut->LineWin[LineIdx] > 0) {
            MGOut->WinType = (EWinType)(MGOut->WinType | EWinType_WinMG);
            break;
	    }
	}
}

void MainGame::GetWayWin(const MainGameInput MGIn, MainGameOutput *MGOut)
{

}

void MainGame::GetScatterWin(const MainGameInput MGIn, MainGameOutput *MGOut)
{
    int SBCount = 0, SFCount = 0;
    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
		for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            ESymbolList ThisSymbol = MGOut->SymbolResult[ColumnIdx][RowIdx];
            if (ThisSymbol == SB) {
                SBCount++;
            }
            /*if (ThisSymbol == SF) {
                SFCount++;
            }*/
		}
	}

    if (SBCount > 0) {
        MGOut->SBWin = OddsTable[(int)SB][SBCount-1] * MGIn.Lines * MGIn.LinesBet;
        if (SBCount >= SB_Trigger_Require) {
            MGOut->WinType = (EWinType)(MGOut->WinType | EWinType_WinBG);
            MGOut->SBCount = SBCount;
        }
    }

    /*if (SFCount > 0) {
        MGOut->SFWin = OddsTable[(int)SF][SFCount-1] * MGIn.Lines * MGIn.LinesBet;
        if (SFCount >= SF_Trigger_Require) {
            MGOut->WinType = (EWinType)(MGOut->WinType | EWinType_WinFG);
            MGOut->SFCount = SFCount;
        }
    }*/
}

ESymbolType MainGame::JoudgeSymbolType(ESymbolList ThisSymbol)
{
    if (ThisSymbol == H1 || ThisSymbol == H2 || ThisSymbol == H3 || ThisSymbol ==  H4 || ThisSymbol == H5 ||
        ThisSymbol == LA || ThisSymbol == LK || ThisSymbol == LQ || ThisSymbol ==  LJ || ThisSymbol == H6 ) {
        return ESymbolType_Normal;
    }
    else if (ThisSymbol == MW || ThisSymbol == EW){
        return ESymbolType_Wild;
    }
    else if (/*ThisSymbol == SF || */ThisSymbol == SB) {
        return ESymbolList_Scatter;
    }
}
