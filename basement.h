#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "extmem.h"
#include "stdbool.h"

#define R_BEGIN 1
#define R_END 16
#define S_BEGIN 17
#define S_END 48

#ifndef basement_h
#define basement_h
typedef struct entry
{
    int AC;
    int BD;
} ENTRY;
#endif /* basement_h */
//initialize buffer
int init_buffer(Buffer *buf);

/*  Write the block to the hard disk
    * @param buf: buffer
    * @param block_num: block number
    * @param data: data
*/
int write_block(Buffer *buf, unsigned int block_num, unsigned char *data);
/**
 * compare the ENTRY
 * @param a
 * @param b
 * @return 1: a > b;\n 0: a = b;\n -1: a \< b;
 */
int compare(ENTRY *a, ENTRY *b);
//use quickly sort to sort from data[start] to data[end]
void quickly_sort(ENTRY *data, int start, int end);
/* Read the block from the hard disk
    * @param buf: buffer
    * @param block_num: block number
    * @param data: data
*/
void read_block(Buffer *buf,unsigned char** data, unsigned int block_num, bool is_print);
//convert string to int, first situation
int string_int_1(unsigned char *s,int i);
//convert string to int, second situation
int string_int_2(unsigned char *s,int i);
//convert int to string
unsigned char* int_string(int X);
//save data to block
void save_data(unsigned char *blk, int X, int Y, unsigned int index);
//fill index of block at end
void fill_next_index(unsigned char *output_blk, unsigned int this_index, unsigned int output_count);
//find the value by the index
int find_index(int start, int end, int value);