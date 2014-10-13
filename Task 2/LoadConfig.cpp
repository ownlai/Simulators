#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LoadConfig.h"

int ReadDigitNumbers(char *FileName, int *GetNumberSeries)
{
    FILE *thisfp;
    thisfp = fopen(FileName, "r");

    int NumberIdx = 0;
    char GetDigit, DigitNumberTemp[ReadFileMaxDigit] = {'a','a','a','a','a','a','a','a','a','a'};
    int DigitCount = 0;

    if(thisfp == NULL) {
        printf("Open File Failed!!\n");
    }
    else {
        bool NotDigit = true;
        while ( (GetDigit = getc(thisfp)) != EOF) {
            if (GetDigit >= '0'  && GetDigit <= '9') {
                DigitNumberTemp[DigitCount] = GetDigit;
                DigitCount++;
                NotDigit = false;
            }
            else {
                if (NotDigit) {
                    continue;
                }
                NotDigit = true;

                GetNumberSeries[NumberIdx] = atoi(DigitNumberTemp);
                NumberIdx++;
                DigitCount = 0;
                for (int i = 0; i < ReadFileMaxDigit; i++) {
                    DigitNumberTemp[i]='a';
                }
            }
        }
        if (!NotDigit) {
            GetNumberSeries[NumberIdx] = atoi(DigitNumberTemp);
            NumberIdx++;
        }
    }
/*
    printf("GetNumberSeries : \n");
    for (int i = 0; i < NumberIdx; i++) {
        printf("%d, ", GetNumberSeries[i]);
    }
    printf("\n");

    system("pause");
*/
    fclose(thisfp);

    return NumberIdx;
}

void ReadMainGameReelStripe(int *ReelLengthNumbers, ESymbolList *ReelNumbers)
{
    int GetNumberSeries[1000];
    ReadDigitNumbers(MainGameReelStripeFileName, GetNumberSeries);

    int NumberIdx = 0;
    for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
        ReelLengthNumbers[RowIdx] = GetNumberSeries[NumberIdx];
        NumberIdx++;
    }

    for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
        /*** here use ReelLengthNumbers[RowIdx]+2 because reel stripe fill in two more number in the end***/
        for (int ReelPosIdx = 0; ReelPosIdx < ReelLengthNumbers[RowIdx]+2; ReelPosIdx++) {
            ReelNumbers[RowIdx*Reel_Length_Limit + ReelPosIdx] = (ESymbolList)GetNumberSeries[NumberIdx];
            NumberIdx++;
        }
    }
/*
    printf("ReelLength : ");
    for (int i = 0; i < Reel_Size_Row; i++) {
        printf("%d, ", ReelLengthNumbers[i]);
    }
    printf("\n");
    for (int i = 0; i < Reel_Size_Row; i++) {
        printf("R%d : \n", i+1);
        for (int j = 0; j < ReelLengthNumbers[i]+2; j++) {
            printf("%2d, ", ReelNumbers[i*Reel_Length_Limit+ j]);
            if (j%10 == 9) {
                printf("\n");
            }
        }
        printf("\n");
    }
    system("pause");
*/
}

void ReadFreeGameReelStripe(int *ReelLengthNumbers, ESymbolList *ReelNumbers)
{
    int GetNumberSeries[1000];
    ReadDigitNumbers(FreeGameReelStripeFileName, GetNumberSeries);

    int NumberIdx = 0;
    for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
        ReelLengthNumbers[RowIdx] = GetNumberSeries[NumberIdx];
        NumberIdx++;
    }

    for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
        /*** here use ReelLengthNumbers[RowIdx]+2 because reel stripe fill in two more number in the end***/
        for (int ReelPosIdx = 0; ReelPosIdx < ReelLengthNumbers[RowIdx]+2; ReelPosIdx++) {
            ReelNumbers[RowIdx*Reel_Length_Limit + ReelPosIdx] = (ESymbolList)GetNumberSeries[NumberIdx];
            NumberIdx++;
        }
    }
/*
    printf("ReelLength : ");
    for (int i = 0; i < Reel_Size_Row; i++) {
        printf("%d, ", ReelLengthNumbers[i]);
    }
    printf("\n");
    for (int i = 0; i < Reel_Size_Row; i++) {
        printf("R%d : \n", i+1);
        for (int j = 0; j < ReelLengthNumbers[i]+2; j++) {
            printf("%2d, ", ReelNumbers[i*Reel_Length_Limit+ j]);
            if (j%10 == 9) {
                printf("\n");
            }
        }
        printf("\n");
    }
    system("pause");
*/
}

void ReadOddsTable(int *OddsTable)
{
    int GetNumberSeries[1000];
    ReadDigitNumbers(OddsTableFileName, GetNumberSeries);

    int NumberIdx = 0;
    for (int SymbolIdx = 0; SymbolIdx < SYM_MAX; SymbolIdx++) {
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            OddsTable[SymbolIdx*Reel_Size_Row + RowIdx] = GetNumberSeries[NumberIdx];
            NumberIdx++;
        }
    }
/*
    printf("Odds Table : \n");
    for (int i = 0; i < SYM_MAX; i++) {
        for (int j = 0; j < Reel_Size_Row; j++) {
            printf("%7d, ", OddsTable[i*Reel_Size_Row + j]);
        }
        printf("\n");
    }
    system("pause");
*/
}

void ReadLinesTable(int *LinesTable)
{
    int GetNumberSeries[1000];
    ReadDigitNumbers(LinesTableFileName, GetNumberSeries);

    int NumberIdx = 0;
    for (int LineIdx = 0; LineIdx < Line_Number_Limit; LineIdx++) {
        for (int RowIdx = 0; RowIdx < Reel_Size_Row; RowIdx++) {
            LinesTable[LineIdx*Reel_Size_Row + RowIdx] = GetNumberSeries[NumberIdx];
            NumberIdx++;
        }
    }
/*
    printf("Lines Table : ");
    for (int i = 0; i < Max_Line; i++) {
        if(i%5 == 0) {
            printf("\n");
        }
        for (int j = 0; j < Reel_Size_Row; j++) {
            printf("%d, ", LinesTable[i*Reel_Size_Row + j]);
        }
        printf("\n");
    }
    system("pause");
*/
}

void ReadBonusGameConfig(int *CreditProbTable)
{
    int GetNumberSeries[1000];
    ReadDigitNumbers(BonusGameConfigFileName, GetNumberSeries);

    int NumberIdx = 0;
    for (int CreditProbIdx = 0; CreditProbIdx < 10; CreditProbIdx++) {
        CreditProbTable[CreditProbIdx] = GetNumberSeries[NumberIdx];
        NumberIdx++;
    }
/*
    printf("Bonus Game Credit Prob Table : \n");
    for (int i = 0; i < 10; i++) {
        printf("%d, ", CreditProbTable[i]);
    }
    printf("\n");
    system("pause");
*/
}

void ReadTestConfig(int *ConfigNumberArray)
{
    int GetNumberSeries[1000];
    ReadDigitNumbers(TestConfigFileName, GetNumberSeries);

    int NumberIdx = 0;
    for (int ConfigNumberIdx = 0; ConfigNumberIdx < TestConfigNumbers; ConfigNumberIdx++) {
        ConfigNumberArray[ConfigNumberIdx] = GetNumberSeries[NumberIdx];
        NumberIdx++;
    }
}
