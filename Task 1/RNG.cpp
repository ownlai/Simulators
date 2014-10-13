#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

void InitializeRNG()
{
   unsigned int t;
   t = (unsigned int) time(NULL); /* 以時間當作亂數種子 */
   srand(t);
   t = t % 99; // 利用時間來隨機取得介於 0~98 的某個值
   while (t) { // 隨機拋棄前面 0~98 個亂數值, 以增加其隨機性
      rand();
      t--;
   } // 亂數產生器初始化完畢
}

int RNG(double small, double big) // input:下限值與上限值 output:範圍內的隨機數
{
 	double x;

  	x = small + ((big - small + 1) * rand() / (RAND_MAX + 1));
   // 若剛好 rand()是 32767 而 RAND_MAX 又不 +1 會造成傳回值是 small + big - small + 1 = big + 1 會超過 big 值
   // 所以 RAND_MAX 要額外 +1
   return (int) x; // 強制轉型使用去尾法, 恰好符合需求
}
