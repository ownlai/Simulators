#ifndef DEFINE_H
#define DEFINE_H

/***** base define *****/
#define Max_Line                25
#define Max_Line_Bet            10

#define Reel_Size_Column        3
#define Reel_Size_Row           5

#define SB_Trigger_Require      3
#define SF_Trigger_Require      1
#define SF_ReTrigger_Require    1

#define FG_Spin                 1
#define FG_Multiplier           1
#define FG_Retrigger_Spin       1

#define LowestPaySymbol         LT


#define Symbol_Number_Limit     20
#define Reel_Length_Limit       200
#define Line_Number_Limit       100

#define ReadFileMaxDigit        10
#define MaxFileWinRecord        1000000


#define MainGameReelStripeFileName  ".\\config\\MGReelStripe.txt"
#define FreeGameReelStripeFileName  ".\\config\\FGReelStripe.txt"
#define OddsTableFileName           ".\\config\\OddsTable.txt"
#define LinesTableFileName          ".\\config\\LinesTable.txt"
#define BonusGameConfigFileName     ".\\config\\BGConfig.txt"

#define TestConfigFileName          ".\\config\\TestConfig.txt"
#define TestConfigNumbers           9

#endif
