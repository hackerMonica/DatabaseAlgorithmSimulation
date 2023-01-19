#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "question4.h"
#include "basement.h"
#include "question5.h"
#include "question2.h"
void connect();
void write_2_entry(ENTRY S, ENTRY R);
int question4()
{
    classRest(701);
    //sort R
    TPMMS_sort(R_BEGIN, R_END, 101, false);
    //sort S
    TPMMS_sort(S_BEGIN, S_END, 201, false);
    //connect
    connect();
}
//Fixed S and moved R
void connect(){
    bool is_end = false;
    ENTRY R = getFromR();
    ENTRY S = getFromS();
    int output_count = 0;
    while (!is_end)
    {
        if (S.AC == -1)
        {
            is_end = true;
            break;
        }else if (R.AC == -1){
            rest_R();
            S = getFromS();
            R = getFromR();
        }else{
            if (R.AC == S.AC){
                write_2_entry(S, R);
                output_count++;
                R = getFromR();
            }else if (R.AC > S.AC){
                rest_R();
                S = getFromS();
                R = getFromR();
            }else{
                R = getFromR();
            }
        }
    }
    write_remain();
    printf("总共连接次数: %d\n", output_count);
}

void write_2_entry(ENTRY S, ENTRY R){
    write_entry(S);
    write_entry(R);
}