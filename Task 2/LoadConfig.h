#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include "common.h"
#include "define.h"


int ReadDigitNumbers(char *FileName, int *GetNumberSeries);

void ReadMainGameReelStripe(int *ReelLengthNumbers, ESymbolList *ReelNumbers);
void ReadFreeGameReelStripe(int *ReelLengthNumbers, ESymbolList *ReelNumbers);
void ReadLinesTable(int *LinesTable);
void ReadOddsTable(int *OddsTable);
void ReadBonusGameConfig(int *CreditProbTable);
void ReadTestConfig(int *ConfigNumberArray);

#endif
