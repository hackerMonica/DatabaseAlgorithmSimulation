#include "basement.h"

int question2();
/**
 * use TPMMS to sort
 * @param start block number of start
 * @param end   block number of end
 * @param write2blk write to which block
 */
void TPMMS_sort(unsigned int start, unsigned int end, unsigned int write2blk, bool is_print);
void sort_R();
void sort_S();
// sort in block
int sort_in_group(unsigned int start, unsigned int end, unsigned int write2blk, bool is_print);
// sort between blocks
void sort_between_group(unsigned int start, unsigned int end, unsigned int write2blk, int group_num, bool is_print);
ENTRY get_min_number(unsigned char data[9][64], int index_of_blk[], int group_start, int group_end);
ENTRY get_min_number_2(unsigned char** data, int group_num, int index_of_blk[], int use_of_group[]);