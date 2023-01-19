#include <stdbool.h>
#include "basement.h"
#include "extmem.h"

extern Buffer buf;                   // main.c
//initialize buffer
int init_buffer(Buffer *buf){
    if (!initBuffer(520, 64, buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }
    return 0;
}
/*
    * Write the block to the hard disk
    * @param buf: buffer
    * @param block_num: block number
    * @param data: data
*/
int write_block(Buffer *buf, unsigned int block_num, unsigned char *data){
    if (writeBlockToDisk(data, block_num, buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }
    return 0;
}

/* Read the block from the hard disk
    * @param buf: buffer
    * @param block_num: block number
    * @param data: data
*/
void read_block(Buffer *buf,unsigned char** data, unsigned int block_num, bool is_print){
    if ((*data = readBlockFromDisk(block_num, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return;
    }else{
        if (is_print)
            printf("读入数据块\t%d\n", block_num);
    }
}
/**
 * compare the ENTRY
 * @param a
 * @param b
 * @return 1: a > b, 0: a = b, -1: a < b
 */
int compare(ENTRY *a, ENTRY *b){
    int ans;
    if (a->AC < b->AC)
        ans = -1;
    else if (a->AC > b->AC)
        ans = 1;
    else{
        if (a->BD < b->BD)
            ans = -1;
        else if (a->BD > b->BD)
            ans = 1;
        else
            ans = 0;
    }
    return ans;
}

//use quickly sort to sort from data[start] to data[end]
//as the AC in ENTRY
void quickly_sort(ENTRY *data, int start, int end){
    if (start >= end)
    {
        return;
    }
    int i = start, j = end;
    ENTRY key = data[start];
    while (i < j)
    {
        while (i<j && compare(&data[j], &key) >= 0)
        {
            j--;
        }
        data[i] = data[j];
        while (i<j && compare(&data[i], &key) <= 0)
        {
            i++;
        }
        data[j] = data[i];
    }
    data[i] = key;
    quickly_sort(data, start, i-1);
    quickly_sort(data, i+1, end);
}
//convert string to int, first situation
int string_int_1(unsigned char *s,int i){
    int res = 0;
    char str[5];
    for (int k = 0; k < 4; k++)
    {
        str[k] = *(s + i*8 + k);
    }
    res = atoi(str);
    return res;
}
//convert string to int, second situation
int string_int_2(unsigned char *s,int i){
    int res = 0;
    char str[5];
    for (int k = 0; k < 4; k++)
    {
        str[k] = *(s + i*8 + 4 + k);
    }
    res = atoi(str);
    return res;
}
//convert int to string
unsigned char* int_string(int X){
    unsigned char *str = (unsigned char *)malloc(5);
    int i=4;
    while (X>0&&i>=0)
    {
        str[i--]=X%10+'0';
        X/=10;
    }
    while (i>=0)
    {
        for (size_t j = 0; j < 4; j++)
        {
            str[j] = str[j+1];
        }
        str[4] = (unsigned char) NULL;
        i--;
    }
    return str;
}
//save data to block
void save_data(unsigned char *blk, int X, int Y, unsigned int index){
    char *str = int_string(X);
    for (int k = 0; k < 4; k++)
    {
        *(blk + index*8 + k) = str[k];
    }
    str = int_string(Y);
    for (int k = 0; k < 4; k++)
    {
        *(blk + index*8 + 4 + k) = str[k];
    }
}
//fill index of block at end
void fill_next_index(unsigned char *output_blk, unsigned int this_index, unsigned int output_count){
    char* str = int_string(this_index+1);
    for (;output_count<7;output_count++)
    {
        for (int k = 0; k < 4; k++)
        {
            *(output_blk + output_count*8 + k) = (unsigned char) NULL;
        }
        for (int k = 0; k < 4; k++)
        {
            *(output_blk + output_count*8 + 4 + k) = (unsigned char) NULL;
        }
    }
    for (int k = 0; k < 4; k++)
    {
        *(output_blk + 7*8 + k) = str[k];
    }
}
//find the value by the index
int find_index(int start, int end, int value){
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
            if (last_num <= value && this_num >= value) {
                freeBlockInBuffer(data,&buf);
                return last_index;
            }
        }
        freeBlockInBuffer(data,&buf);
    }
    return -1;
}
