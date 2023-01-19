#include "question1.h"
#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "basement.h"
#define OUTPUT_BLOCK 50

extern Buffer buf;                   // main.c

void question1()
{
    unsigned char *data, *output;
    output = getNewBlockInBuffer(&buf);
    unsigned input_block = S_BEGIN;
    unsigned output_block = OUTPUT_BLOCK;
    unsigned output_count = 0, match_count = 0;
    for (size_t i = 0; i <= S_END - S_BEGIN; i++)
    {
        read_block(&buf, &data ,input_block, true);
        for (int j = 0; j < 7; j++) // 一个blk存7个元组加一个地址
        {
            int X = string_int_1(data, j);
            int Y = string_int_2(data, j);
            if (X == 128)
            {
                printf("( X = %d, Y = %d ) \n", X, Y);
                match_count++;
                save_data(output, X, Y, output_count);
                output_count++;
                if(output_count == 7)
                {
                    fill_next_index(output,output_block,output_count);
                    write_block(&buf, output_block, output);
                    printf("注：写出数据块\t%d\n", output_block);
                    freeBlockInBuffer(output,&buf);
                    output = getNewBlockInBuffer(&buf);
                    output_block++;
                    output_count = 0;
                }
            }
        }
        input_block = string_int_1(data, 7);
        freeBlockInBuffer(data,&buf);
    }
    if (output_count!=0&&match_count>0) {
        fill_next_index(output,output_block,output_count);
        write_block(&buf, output_block, output);
        printf("注：写出数据块\t%d\n", output_block);
    }
    freeBlockInBuffer(output,&buf);
    printf("\n");
    printf("满足条件的元组一共%d个。IO读写%d次\n", match_count, buf.numIO);
}