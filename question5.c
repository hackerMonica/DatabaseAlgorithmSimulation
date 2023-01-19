#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "question5.h"
#include "question3.h"
#include "question2.h"
#include "basement.h"

extern Buffer buf;                   // main.c
void intersections();
void unions();
void differences();
int question5()
{
    
    //sort R
    TPMMS_sort(R_BEGIN, R_END, 101, false);
    //sort S
    TPMMS_sort(S_BEGIN, S_END, 201, false);
    //build index
    build_index(117, 132, 301, false);
    build_index(233, 264, 401, false);
    //intersections
    classRest(1101);
    intersections();
    //unions
    classRest(1201);
    unions();
    //differences
    classRest(1301);
    differences();
}
//differences of R and S
void differences(){
    printf("---------------------------------\n");
    printf("    基于排序的集合差操作算法 S-R:    \n");
    printf("---------------------------------\n");
    bool is_end = false;
    ENTRY R = getFromR();
    ENTRY S = getFromS();
    ENTRY last;
    last.AC = -1;
    last.BD = -1;
    int output_count = 0;
    while (!is_end)
    {
        if (R.AC == -1 && S.AC == -1)
        {
            is_end = true;
            break;
        }else if (R.AC == -1)
        {
            if (compare(&S, &last) != 0) {
                write_entry(S);
                output_count++;
            }
            S = getFromS();
        }else if (S.AC == -1){
//            if (compare(&R, &last) != 0) {
//                write_entry(R);
//                output_count++;
//            }
            R = getFromR();
        }else if (compare(&R, &last) == 0){
            R = getFromR();
        }else if (compare(&S, &last) == 0){
            S = getFromS();
        }else
            switch (compare(&R, &S))
            {
                case 0:
                    last = R;
                    R = getFromR();
                    S = getFromS();
                    break;
                case 1:
                    write_entry(S);
                    S = getFromS();
                    output_count++;
                    break;
                case -1:
//                    write_entry(R);
                    R = getFromR();
//                    output_count++;
                    break;
                default:
                    break;
            }
    }
    write_remain();
    printf("差集共有%d个元组\n", output_count);
}
//union of R and S
void unions(){
    printf("---------------------------------\n");
    printf("     基于排序的集合并操作算法S∪R:     \n");
    printf("---------------------------------\n");
    bool is_end = false;
    ENTRY R = getFromR();
    ENTRY S = getFromS();
    int output_count = 0;
    while (!is_end)
    {
        if (R.AC == -1 && S.AC == -1)
        {
            is_end = true;
            break;
        }else if (R.AC == -1)
        {
            write_entry(S);
            S = getFromS();
            output_count++;
        }else if (S.AC == -1){
            write_entry(R);
            R = getFromR();
            output_count++;
        }else
            switch (compare(&R, &S))
            {
                case 0:
                    R = getFromR();
                    break;
                case 1:
                    write_entry(S);
                    S = getFromS();
                    output_count++;
                    break;
                case -1:
                    write_entry(R);
                    R = getFromR();
                    output_count++;
                    break;
                default:
                    break;
            }
    }
    write_remain();
    printf("并集共有%d个元组\n", output_count);
}
//Intersections of R and S
void intersections(){
    printf("---------------------------------\n");
    printf("     基于排序的集合交操作算法S∩R:     \n");
    printf("---------------------------------\n");
    bool is_end = false;
    ENTRY R = getFromR();
    ENTRY S = getFromS();
    int output_count = 0;
    while (!is_end)
    {
        if (R.AC == -1 || S.AC == -1)
        {
            is_end = true;
            break;
        }
        switch (compare(&R, &S))
        {
        case 0:
            printf("(%d, %d)\n", R.AC, R.BD);
            write_entry(R);
            R = getFromR();
            S = getFromS();
            output_count++;
            break;
        case 1:
            S = getFromS();
            break;
        case -1:
            R = getFromR();
            break;
        default:
            break;
        }
    }
    write_remain();
    printf("交集共有%d个元组\n", output_count);
}
unsigned char* dataR= (unsigned char *) -1;
int indexR = 0, index_blk_R = 117;
ENTRY getFromR(){
    ENTRY ans;
    if (dataR == (unsigned char *) -1)
    {
        dataR = readBlockFromDisk(117, &buf);
    }
    //boundary handle
    if (indexR == 7)
    {
        indexR = 0;
        index_blk_R ++;
        freeBlockInBuffer(dataR, &buf);
        if (index_blk_R == 133)
        {
            ans.AC = -1;
            return ans;
        }
        dataR = readBlockFromDisk(index_blk_R, &buf);
    }
    //give numbers from dataR
    ans.AC = string_int_1(dataR, indexR);
    ans.BD = string_int_2(dataR, indexR);
    indexR ++; 
    //if indexR is 8, then read next block
    
    return ans;
}
unsigned char* dataS= (unsigned char *) -1;
int indexS = 0, index_blk_S = 233;
ENTRY getFromS(){
    ENTRY ans;
    if (dataS == (unsigned char *) -1)
    {
        dataS = readBlockFromDisk(233, &buf);
    }
    //boundary handle
    if (indexS == 7)
    {
        indexS = 0;
        index_blk_S ++;
        freeBlockInBuffer(dataS, &buf);
        if (index_blk_S == 265)
        {
            ans.AC = -1;
            return ans;
        }
        dataS = readBlockFromDisk(index_blk_S, &buf);
    }
    //give numbers from dataS
    ans.AC = string_int_1(dataS, indexS);
    ans.BD = string_int_2(dataS, indexS);
    indexS ++;
    
    return ans;
}
//write ENTRY into disk
unsigned char* data_write = (unsigned char *) -1;
int write2block = 1101, write2count = 0;
void write_entry(ENTRY entry){
    if (data_write == (unsigned char *) -1)
    {
        data_write = getNewBlockInBuffer(&buf);
    }
    save_data(data_write,entry.AC, entry.BD, write2count);
    write2count ++;
    if (write2count == 7)
    {
        fill_next_index(data_write, write2block, write2count);
        writeBlockToDisk(data_write, write2block, &buf);
        printf("注：结果写入磁盘: %d\n", write2block);
        write2count = 0;
        write2block ++;
        freeBlockInBuffer(data_write, &buf);
        data_write = getNewBlockInBuffer(&buf);
    }
}
//write remain data into disk
void write_remain(){
    if (write2count != 0){
        fill_next_index(data_write, write2block, write2count);
        writeBlockToDisk(data_write, write2block, &buf);
        printf("注：结果写入磁盘: %d\n", write2block);
        freeBlockInBuffer(data_write, &buf);
        data_write = (unsigned char *) -1;
        write2count = 0;
    }
    freeBlockInBuffer(dataS, &buf);
    dataS = (unsigned char *) -1;
    indexS = 0;
    freeBlockInBuffer(dataR, &buf);
    dataR = (unsigned char *) -1;
    indexR = 0;
}
//rest the class abstract data
void classRest(int write2Block){
    index_blk_R = 117;
    index_blk_S = 233;
    write2block = write2Block;
    if (data_write != (unsigned char *) -1)
    {
        freeBlockInBuffer(data_write, &buf);
        data_write = (unsigned char *) -1;
        write2count = 0;
    }
    if (dataS != (unsigned char *) -1)
    {
        freeBlockInBuffer(dataS, &buf);
        dataS = (unsigned char *) -1;
        indexS = 0;
    }
    if (dataR != (unsigned char *) -1)
    {
        freeBlockInBuffer(dataR, &buf);
        dataR = (unsigned char *) -1;
        indexR = 0;
    }
}
//rest S
void rest_S(){
    index_blk_S = 233;
    if (dataS != (unsigned char *) -1)
    {
        freeBlockInBuffer(dataS, &buf);
        dataS = (unsigned char *) -1;
        indexS = 0;
    }
}
//rest R
void rest_R(){
    index_blk_R = 117;
    if (dataR != (unsigned char *) -1)
    {
        freeBlockInBuffer(dataR, &buf);
        dataR = (unsigned char *) -1;
        indexR = 0;
    }
}