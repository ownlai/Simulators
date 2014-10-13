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
    //0 = strike, 1 = ball, 2 = hit
    int CardContent[8] = {0, 0, 0, 1, 1, 1, 1, 2};
    bool CardRand[8] = {false, false, false, false, false, false, false, false};
    BGOut->BGWin = 0;

    int Credit = 0;
    int StrikeNumber = 0;
    int BallNumber = 0;
    bool BB = false;

    for (int CardRound = 0; CardRound < 8; CardRound++) {
        int RndCard = 0;
        do {
            RndCard = RNG(0, 7);
        } while (CardRand[RndCard] == true);
        CardRand[RndCard] = true;

        Credit++;
        if (CardContent[RndCard] == 0) {
            StrikeNumber++;
            if (StrikeNumber == 3 && BB == false) {
                break;
            }
            else if (StrikeNumber == 3 && BB == true) {
                Credit = Credit + 20;
                break;
            }
        }
        else if (CardContent[RndCard] == 1) {
            BallNumber++;
            if (BallNumber == 4 && BB == false) {
                CardRound = 0;
                StrikeNumber = 0;
                BallNumber = 0;
                BB = true;
                for (int i = 0; i < 8; i++) {
                    CardRand[i] = false;
                }
            }
            else if (BallNumber == 4 && BB == true) {
                Credit = Credit + 50;
                break;
            }
        }
        else if (CardContent[RndCard] == 2) {
            int HitResult = RNG(1, 100);
            if (HitResult <= 60 && BB == false) {
                Credit = Credit + 50;
            }
            else if (HitResult > 60 && BB == false) {
                Credit = Credit + 100;
            }
            else if (HitResult <= 60 && BB == true) {
                Credit = Credit + 50;
            }
            else if (HitResult > 60 && BB == true) {
                Credit = Credit + 500;
            }
            break;
        }
    }

    BGOut->BGWin = Credit*BGIn.Lines*BGIn.LinesBet;
}
