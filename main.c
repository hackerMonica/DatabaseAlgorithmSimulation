#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "extmem.h"
#include "question1.h"
#include "question2.h"
#include "question3.h"
#include "question4.h"
#include "question5.h"
Buffer buf;
int main(int argc, char **argv)
{
//    SetConsoleOutputCP(CP_UTF8);
    system("chcp 65001 & cls");

    init_buffer(&buf);
    printf("---------------------------------\n");
    printf("    基于线性搜索算法 S.C = 128;    \n");
    printf("---------------------------------\n");
    question1();

    init_buffer(&buf);
    printf("---------------------------------\n");
    printf("      利用 TPMMS 算法进行排序:      \n");
    printf("---------------------------------\n");
    question2();

    init_buffer(&buf);
    printf("---------------------------------\n");
    printf("   基于索引的关系选择算法 S.C=128:   \n");
    printf("---------------------------------\n");
    question3();

    init_buffer(&buf);
    printf("---------------------------------\n");
    printf("   基于排序的连接操作算法 S join R:   \n");
    printf("---------------------------------\n");
    question4();

    init_buffer(&buf);
    printf("---------------------------------\n");
    printf("   基于排序的两趟扫描算法，二元运算:   \n");
    printf("---------------------------------\n");
    question5();
}
