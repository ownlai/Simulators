#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

void InitializeRNG()
{
   unsigned int t;
   t = (unsigned int) time(NULL); /* �H�ɶ���@�üƺؤl */
   srand(t);
   t = t % 99; // �Q�ήɶ����H�����o���� 0~98 ���Y�ӭ�
   while (t) { // �H���߱�e�� 0~98 �Ӷüƭ�, �H�W�[���H����
      rand();
      t--;
   } // �üƲ��;���l�Ƨ���
}

int RNG(double small, double big) // input:�U���ȻP�W���� output:�d�򤺪��H����
{
 	double x;

  	x = small + ((big - small + 1) * rand() / (RAND_MAX + 1));
   // �Y��n rand()�O 32767 �� RAND_MAX �S�� +1 �|�y���Ǧ^�ȬO small + big - small + 1 = big + 1 �|�W�L big ��
   // �ҥH RAND_MAX �n�B�~ +1
   return (int) x; // �j���૬�ϥΥh���k, ��n�ŦX�ݨD
}
