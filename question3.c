#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "question3.h"
#include "question2.h"
extern Buffer buf;                   // main.c

void build_index(int start, int end, int write2blk, bool is_print);
//find index block
int find_index_128(int start, int end);
//select data from R from block start to end
void select_data(int start, int end, int write2blk, bool is_print);

//基于索引的关系选择算法
int question3()
{
    //sort R
    TPMMS_sort(R_BEGIN, R_END, 101, false);
    //sort S
    TPMMS_sort(S_BEGIN, S_END, 201, false);
    //build index
    printf("---------------------------------\n");
    printf("             建立R索引             \n");
    printf("---------------------------------\n");
    build_index(117, 132, 301, true);
    printf("---------------------------------\n");
    printf("             建立S索引             \n");
    printf("---------------------------------\n");
    build_index(233, 264, 401, true);
    //select S.C = 128
    printf("---------------------------------\n");
    printf("         关系选择S.C=128         \n");
    printf("---------------------------------\n");
    buf.numIO = 0;
    int index = find_index_128(401, 403);
    select_data(index, 264, 501, true);
}
//select data from R from block start to end
void select_data(int start, int end, int write2blk, bool is_print){
    unsigned char *data, *output;
    output = getNewBlockInBuffer(&buf);
    unsigned int output_count=0, output_block=write2blk;
    int match_count = 0;
    bool find_out = false;
    for (unsigned int i = start; i < end; i++) {
        read_block(&buf, &data, i, true);
        for (int j = 0; j < 7; j++) {
            int X = string_int_1(data, j);
            int Y = string_int_2(data, j);
            if (X == 128) {
                match_count++;
                if (is_print) {
                    printf("( X = %d, Y = %d ) \n", X, Y);
                }
                save_data(output, X, Y, output_count);
                output_count++;
                if(output_count == 7)
                {
                    fill_next_index(output,output_block,output_count);
                    write_block(&buf, output_block, output);
                    if (is_print) {
                        printf("注： 写出数据块\t%d\n", output_block);
                    }
                    freeBlockInBuffer(output,&buf);
                    output = getNewBlockInBuffer(&buf);
                    output_block++;
                    output_count = 0;
                }
            }else if(X > 128){
                find_out = true;
                break;
            }
        }
        freeBlockInBuffer(data,&buf);
        if (find_out) {
            break;
        }
    }
    if (output_count!=0) {
        fill_next_index(output,output_block,output_count);
        write_block(&buf, output_block, output);
        if (is_print) {
            printf("注： 写出数据块\t%d\n", output_block);
        }
    }
    if (is_print) {
        printf("满足条件的元组一共%d个\n", match_count);
        printf("IO次数\t%d\n", buf.numIO);
    }
    freeBlockInBuffer(output,&buf);
}

//find index block
int find_index_128(int start, int end){
    unsigned char *data;
    int last_num, this_num=1000000;
    int last_index, this_index=-1;
    for (unsigned int i = start; i <=end; i++) {
        read_block(&buf, &data, i, false);
        for (int j = 0; j < 7; j++) {
            last_num = this_num;
            last_index = this_index;
            this_num = string_int_1(data, j);
            this_index = string_int_2(data, j);
            if (last_num <= 128 && this_num >= 128) {
                freeBlockInBuffer(data,&buf);
                return last_index;
            }
        }
        freeBlockInBuffer(data,&buf);
    }
    return -1;
}
//build index
void build_index(int start, int end, int write2blk, bool is_print){
    unsigned char *data, *output;
    output = getNewBlockInBuffer(&buf);
    unsigned int output_count=0, output_block=write2blk;
    for (unsigned int i = start; i <= end; i+=2) {
        read_block(&buf, &data, i, is_print);
        int X = string_int_1(data, 0);
        save_data(output, X, i, output_count);
        output_count++;
        if(output_count == 7)
        {
            fill_next_index(output,output_block,output_count);
            write_block(&buf, output_block, output);
            if (is_print) {
                printf("\n写出数据块\t%d\n", output_block);
            }
            freeBlockInBuffer(output,&buf);
            output = getNewBlockInBuffer(&buf);
            output_block++;
            output_count = 0;
        }
        freeBlockInBuffer(data,&buf);
    }
    if (output_count!=0) {
        fill_next_index(output,output_block,output_count);
        write_block(&buf, output_block, output);
        if (is_print) {
            printf("\n写出数据块\t%d\n", output_block);
        }
    }
    freeBlockInBuffer(output,&buf);
}
