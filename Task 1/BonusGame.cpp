#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BonusGame.h"

void BonusGame::GetBonusGameConfig()
{
    ReadBonusGameConfig(CreditProbTable);
}

void BonusGame::GetBonusGameCredit(const BonusGameInput BGIn, BonusGameOutput *BGOut)
{
    BGOut->BGWin = 0;
    int RndNumber = RNG(1, 100);
    int Credit = 0;
    for (int CreditProbIdx = 0; CreditProbIdx < 10; CreditProbIdx++) {
        if (RndNumber <= CreditProbTable[CreditProbIdx]) {
            Credit = CreditProbIdx;
            break;
        }
        else {
            RndNumber = RndNumber - CreditProbTable[CreditProbIdx];
        }
    }
    RndNumber = RNG(1, 10);
    Credit = Credit*10 + RndNumber;

    BGOut->BGWin = Credit*BGIn.Lines*BGIn.LinesBet;
}
