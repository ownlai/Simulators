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

    int MaxScore = 0;
    for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
            MaxScore = MaxScore + MGOut->LineWin[LineIdx];
    }
    MaxScore = MaxScore + MGOut->SFWin;
    MaxScore = MaxScore + MGOut->SBWin;

    MainGameOutput MGOutMax;
    {
        for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                MGOutMax.SymbolResult[ColumnIdx][RowIdx] = MGOut->SymbolResult[ColumnIdx][RowIdx];
            }
        }
        for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
            MGOutMax.LineSymbol[LineIdx] = MGOut->LineSymbol[LineIdx];
            MGOutMax.LineSymbolCount[LineIdx] = MGOut->LineSymbolCount[LineIdx];
            MGOutMax.LineWin[LineIdx] = MGOut->LineWin[LineIdx];
        }
        MGOutMax.SFWin = MGOut->SFWin;
        MGOutMax.SFCount = MGOut->SFCount;
        MGOutMax.SBWin = MGOut->SBWin;
        MGOutMax.SBCount = MGOut->SBCount;
        MGOutMax.WinType = MGOut->WinType;
    }

    //Fill in Reel Position to Output
    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            MGOutMax.SymbolResult[ColumnIdx][RowIdx] = Reel[RowIdx][ReelPosition[RowIdx]+ColumnIdx];
        }
        //printf("{%d, %d, %d, %d, %d}\n", MGOut->SymbolResult[ColumnIdx][0], MGOut->SymbolResult[ColumnIdx][1], MGOut->SymbolResult[ColumnIdx][2], MGOut->SymbolResult[ColumnIdx][3], MGOut->SymbolResult[ColumnIdx][4]);
    }

    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
		for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            if (ColumnIdx != Reel_Size_Column-1) {
                MainGameOutput MGOutCopy;
                {
                    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
                        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                            MGOutCopy.SymbolResult[ColumnIdx][RowIdx] = (ESymbolList)0;
                        }
                    }

                    for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
                        MGOutCopy.LineSymbol[LineIdx] = (ESymbolList)0;
                        MGOutCopy.LineSymbolCount[LineIdx] = 0;
                        MGOutCopy.LineWin[LineIdx] = 0;
                    }
                    MGOutCopy.SFWin = 0;
                    MGOutCopy.SFCount = 0;
                    MGOutCopy.SBWin = 0;
                    MGOutCopy.SBCount = 0;
                    MGOutCopy.WinType = EWinType_NoWin;
                }

                //Fill in Reel Position to Output
                for (int ColumnIdx2 = 0; ColumnIdx2 < Reel_Size_Column; ColumnIdx2++) {
                    for (int RowIdx2 = 0; RowIdx2 < Reel_Size_Row; RowIdx2++) {
                        MGOutCopy.SymbolResult[ColumnIdx2][RowIdx2] = Reel[RowIdx2][ReelPosition[RowIdx2]+ColumnIdx2];
                    }
                    //printf("{%d, %d, %d, %d, %d}\n", MGOut->SymbolResult[ColumnIdx][0], MGOut->SymbolResult[ColumnIdx][1], MGOut->SymbolResult[ColumnIdx][2], MGOut->SymbolResult[ColumnIdx][3], MGOut->SymbolResult[ColumnIdx][4]);
                }

                ESymbolList tempSymbol = MGOutCopy.SymbolResult[ColumnIdx][RowIdx];
                MGOutCopy.SymbolResult[ColumnIdx][RowIdx] = MGOutCopy.SymbolResult[ColumnIdx+1][RowIdx];
                MGOutCopy.SymbolResult[ColumnIdx+1][RowIdx] = tempSymbol;

                if (MGIn.PayMode == EPayMode_Lines_Left || MGIn.PayMode == EPayMode_Lines_Left_Right) {
                    GetLineWin(MGIn, &MGOutCopy);
                }
                else if (MGIn.PayMode == EPayMode_Ways_Left) {
                    GetWayWin(MGIn, &MGOutCopy);
                }

                GetScatterWin(MGIn, &MGOutCopy);

                int ThisScore = 0;
                for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
                        ThisScore = ThisScore + MGOutCopy.LineWin[LineIdx];
                }
                ThisScore = ThisScore + MGOutCopy.SFWin;
                ThisScore = ThisScore + MGOutCopy.SBWin;

                if (ThisScore > MaxScore) {
                    MaxScore = ThisScore;
                    memcpy(&MGOutMax, &MGOutCopy, sizeof(MainGameOutput));
                }

            }
            if (RowIdx != Reel_Size_Row -1) {
                MainGameOutput MGOutCopy;
                {
                    for (int ColumnIdx = 0; ColumnIdx < Reel_Size_Column; ColumnIdx++) {
                        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                            MGOutCopy.SymbolResult[ColumnIdx][RowIdx] = (ESymbolList)0;
                        }
                    }

                    for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
                        MGOutCopy.LineSymbol[LineIdx] = (ESymbolList)0;
                        MGOutCopy.LineSymbolCount[LineIdx] = 0;
                        MGOutCopy.LineWin[LineIdx] = 0;
                    }
                    MGOutCopy.SFWin = 0;
                    MGOutCopy.SFCount = 0;
                    MGOutCopy.SBWin = 0;
                    MGOutCopy.SBCount = 0;
                    MGOutCopy.WinType = EWinType_NoWin;
                }

                //Fill in Reel Position to Output
                for (int ColumnIdx2 = 0; ColumnIdx2 < Reel_Size_Column; ColumnIdx2++) {
                    for (int RowIdx2 = 0; RowIdx2 < Reel_Size_Row; RowIdx2++) {
                        MGOutCopy.SymbolResult[ColumnIdx2][RowIdx2] = Reel[RowIdx2][ReelPosition[RowIdx2]+ColumnIdx2];
                    }
                    //printf("{%d, %d, %d, %d, %d}\n", MGOut->SymbolResult[ColumnIdx][0], MGOut->SymbolResult[ColumnIdx][1], MGOut->SymbolResult[ColumnIdx][2], MGOut->SymbolResult[ColumnIdx][3], MGOut->SymbolResult[ColumnIdx][4]);
                }

                ESymbolList tempSymbol = MGOutCopy.SymbolResult[ColumnIdx][RowIdx];
                MGOutCopy.SymbolResult[ColumnIdx][RowIdx] = MGOutCopy.SymbolResult[ColumnIdx][RowIdx+1];
                MGOutCopy.SymbolResult[ColumnIdx][RowIdx+1] = tempSymbol;

                if (MGIn.PayMode == EPayMode_Lines_Left || MGIn.PayMode == EPayMode_Lines_Left_Right) {
                    GetLineWin(MGIn, &MGOutCopy);
                }
                else if (MGIn.PayMode == EPayMode_Ways_Left) {
                    GetWayWin(MGIn, &MGOutCopy);
                }

                GetScatterWin(MGIn, &MGOutCopy);

                int ThisScore = 0;
                for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
                        ThisScore = ThisScore + MGOutCopy.LineWin[LineIdx];
                }
                ThisScore = ThisScore + MGOutCopy.SFWin;
                ThisScore = ThisScore + MGOutCopy.SBWin;

                if (ThisScore > MaxScore) {
                    MaxScore = ThisScore;
                    memcpy(&MGOutMax, &MGOutCopy, sizeof(MainGameOutput));
                }
            }
		}
    }

    for (int LineIdx = 0; LineIdx < Max_Line; LineIdx++) {
            MGOut->LineWin[LineIdx] = MGOutMax.LineWin[LineIdx];
    }
    MGOut->SFWin = MGOutMax.SFWin;
    MGOut->SBWin = MGOutMax.SBWin;
}

void MainGame::GetLineWin(const MainGameInput MGIn, MainGameOutput *MGOut)
{
    ESymbolList ThisSymbolResult[Reel_Size_Column][Reel_Size_Row];
    memcpy(ThisSymbolResult, MGOut->SymbolResult, sizeof(ThisSymbolResult));

    if (MGIn.WildMode == EWildMode_Expend) {
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

    for (int LineIdx = 0; LineIdx < MGIn.Lines; LineIdx++) {
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
			MGOut->LineSymbol[LineIdx] = FirstSymbol;
            MGOut->LineSymbolCount[LineIdx] = SymbolCount;
            MGOut->LineWin[LineIdx] = OddsTable[(int)FirstSymbol][SymbolCount-1] * MGIn.LinesBet;
		}
		else if (JoudgeSymbolType(FirstSymbol) == ESymbolType_Wild) {
            int WildCount = 0;
            for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
                if (ThisSymbolResult[Line[RowIdx]][RowIdx] == WD) {
                    WildCount++;
                }
                else {
                    break;
                }
            }

            ESymbolList LineSymbol;
            if (WildCount < 5) {
                LineSymbol = ThisSymbolResult[Line[WildCount]][WildCount];
                if (JoudgeSymbolType(LineSymbol) == ESymbolList_Scatter) {
                    MGOut->LineSymbol[LineIdx] = WD;
                    MGOut->LineSymbolCount[LineIdx] = WildCount;
                    MGOut->LineWin[LineIdx] = OddsTable[(int)WD][WildCount-1] * MGIn.LinesBet;
                }
                else {
                    SymbolCount++;
                    if (ThisSymbolResult[Line[1]][1] == WD || ThisSymbolResult[Line[1]][1] == LineSymbol) {
                        SymbolCount++;
                        if (ThisSymbolResult[Line[2]][2] == WD || ThisSymbolResult[Line[2]][2] == LineSymbol) {
                            SymbolCount++;
                            if (ThisSymbolResult[Line[3]][3] == WD || ThisSymbolResult[Line[3]][3] == LineSymbol) {
                                SymbolCount++;
                                if (ThisSymbolResult[Line[4]][4] == WD || ThisSymbolResult[Line[4]][4] == LineSymbol) {
                                    SymbolCount++;
                                }
                            }
                        }
                    }
                    if (OddsTable[(int)WD][WildCount-1] * MGIn.LinesBet >=  OddsTable[(int)LineSymbol][SymbolCount-1] * MGIn.LinesBet) {
                    MGOut->LineSymbol[LineIdx] = WD;
                    MGOut->LineSymbolCount[LineIdx] = WildCount;
                    MGOut->LineWin[LineIdx] = OddsTable[(int)WD][WildCount-1] * MGIn.LinesBet;
                    }
                    else {
                        MGOut->LineSymbol[LineIdx] = LineSymbol;
                        MGOut->LineSymbolCount[LineIdx] = SymbolCount;
                        MGOut->LineWin[LineIdx] = OddsTable[(int)LineSymbol][SymbolCount-1] * MGIn.LinesBet;
                    }
                }
            }
            else {
                MGOut->LineSymbol[LineIdx] = WD;
                MGOut->LineSymbolCount[LineIdx] = WildCount;
                MGOut->LineWin[LineIdx] = OddsTable[(int)WD][WildCount-1] * MGIn.LinesBet;
            }


		}

        if (MGIn.PayMode == EPayMode_Lines_Left_Right) {
            /***** Here we assume that one line only get one credit *****/
            if (MGOut->LineWin[LineIdx] > 0 || SymbolCount == Reel_Size_Row) {
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
            }
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
            if (ThisSymbol == SF) {
                SFCount++;
            }
		}
	}

    ESymbolList ThisSymbolResult[Reel_Size_Column][Reel_Size_Row];
    memcpy(ThisSymbolResult, MGOut->SymbolResult, sizeof(ThisSymbolResult));

    for (int LineIdx = 0; LineIdx < MGIn.Lines; LineIdx++) {
        SBCount = 0;
        int Line[Reel_Size_Row], SymbolCount = 0;
		memcpy(Line, LinesTable[LineIdx], sizeof(Line));

        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            if (ThisSymbolResult[Line[RowIdx]][RowIdx] == SB) {
                SBCount++;
            }
        }
        if (SBCount == SB_Trigger_Require) {
            break;
        }
    }

    if (SBCount > 0) {
        if (SBCount == SB_Trigger_Require) {
            MGOut->SBWin = OddsTable[(int)SB][SBCount-1] * MGIn.LinesBet;
            MGOut->WinType = (EWinType)(MGOut->WinType | EWinType_WinBG);
            MGOut->SBCount = SBCount;
        }
    }

    if (SFCount > 0) {
        MGOut->SFWin = OddsTable[(int)SF][SFCount-1] * MGIn.Lines * MGIn.LinesBet;
        if (SFCount >= SF_Trigger_Require) {
            MGOut->WinType = (EWinType)(MGOut->WinType | EWinType_WinFG);
            MGOut->SFCount = SFCount;
        }
    }
}

ESymbolType MainGame::JoudgeSymbolType(ESymbolList ThisSymbol)
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
