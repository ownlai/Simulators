#ifndef BONUSGAME_H
#define BONUSGAME_H

#include "common.h"
#include "define.h"

#include "RNG.h"
#include "LoadConfig.h"

struct BonusGameInput
{
	int Lines;
	int LinesBet;
};

struct BonusGameOutput
{
	int BGWin;
};

class BonusGame
{
public:
	BonusGame(){};
	~BonusGame(){};

    void GetBonusGameConfig();
	void GetBonusGameCredit(const BonusGameInput BGIn, BonusGameOutput *BGOut);

private:
    int CreditProbTable[10];
};

#endif
