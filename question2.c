#include "question2.h"

extern Buffer buf;                   // main.c
int question2()
{
    printf("---------------------------------\n");
    printf("           对 R 进行排序           \n");
    printf("---------------------------------\n");
    sort_R();
    printf("---------------------------------\n");
    printf("           对 S 进行排序           \n");
    printf("---------------------------------\n");
    sort_S();
    return 0;
}
/**
 * use TPMMS to sort
 * @param start block number of start
 * @param end   block number of end
 * @param write2blk write to which block
 */
void TPMMS_sort(unsigned int start, unsigned int end, unsigned int write2blk, bool is_print){
    int group_num;
    group_num = sort_in_group(start, end, write2blk,is_print);
    if (is_print)
        printf("\n");
    sort_between_group(write2blk, write2blk+end-start, write2blk+end-start+1, group_num,is_print);
}
void sort_R(){
    //sort R
    TPMMS_sort(R_BEGIN, R_END,101,true);
}
void sort_S(){
    //sort S
    TPMMS_sort(S_BEGIN, S_END,201,true);
}
// sort in group
int sort_in_group(unsigned int start, unsigned int end, unsigned int write2blk, bool is_print){
    ENTRY number[7];
    unsigned char data[9][64], *output, *term;
    output = getNewBlockInBuffer(&buf);
    unsigned int group_start, group_end, group_count=0;
    unsigned int output_count=0, output_block=write2blk;
    for (unsigned int i = start; i <= end; i=group_end+1) {
        group_start = i, group_end = (i+7>end)?end:i+7;
        group_count++;
        //sort in block
        for (unsigned int j = group_start; j <= group_end; ++j){
            read_block(&buf, &term, j, false);
            //print term
//            for (int i=0;i<64;i++){
//                printf("%c",term[i]);
//            }
            for (int k = 0; k < 7; ++k) {
                number[k].AC = string_int_1(term, k);
                number[k].BD = string_int_2(term, k);
            }
            quickly_sort(number, 0, 6);
            for (int k = 0; k < 7; ++k) {
                save_data(term, number[k].AC, number[k].BD, k);
            }
            memcpy(data[j-group_start+1], term, sizeof(data[0]));
            freeBlockInBuffer(term, &buf);
        }
        //sort in group
        int index_of_blk[group_end-group_start+2];
        memset(index_of_blk, 0, sizeof(index_of_blk));
        ENTRY min;
        while ((min = get_min_number(data, index_of_blk, group_start, group_end)).AC != -1)
        {
            save_data(output, min.AC, min.BD, output_count);
            output_count++;
            if (output_count == 7){
                fill_next_index(output, output_block, output_count);
                write_block(&buf, output_block, output);
                if (is_print){
                    printf("写出组内排序数据块 %d\n",output_block);
                }
                output_block++;
                output_count = 0;
                freeBlockInBuffer(output,&buf);
                output = getNewBlockInBuffer(&buf);
            }
        }
    }
    //free blocks
    freeBlockInBuffer(output,&buf);
    return group_count;
}
//get the min number from group
ENTRY get_min_number(unsigned char data[9][64], int index_of_blk[], int group_start, int group_end){
    ENTRY ans;
    ans.AC = 1000000;
    int min_index = -1;
    bool have_find=false;
    for (int i = group_start; i <= group_end; ++i) {
        if (index_of_blk[i-group_start+1] < 7){
            have_find = true;
            ENTRY temp;
            temp.AC = string_int_1(data[i-group_start+1], index_of_blk[i-group_start+1]);
            temp.BD = string_int_2(data[i-group_start+1], index_of_blk[i-group_start+1]);
            if (compare(&temp, &ans) == -1){
                ans = temp;
                min_index = i-group_start+1;
            }
//            if (temp < min){
//                min = temp;
//                min_index = i-group_start+1;
//            }
        }
    }
    if(!have_find){
        ans.AC = -1;
        return ans;
    }else{
        index_of_blk[min_index]++;
        return ans;
    }
}
// sort between blocks
void sort_between_group(unsigned int start, unsigned int end, unsigned int write2blk, int group_num,bool is_print){
    unsigned char* data[group_num], *output, *term;
    output = getNewBlockInBuffer(&buf);
    unsigned int output_count=0, output_block=write2blk;

    int index_of_blk[group_num], use_of_group[group_num];
    bool write_out=false;
    memset(index_of_blk, 0, sizeof(index_of_blk));
    for (int i=0;i<group_num;i++){
        read_block(&buf, &data[i], start+i*8, false);
        use_of_group[i] = 1;
    }

    while(!write_out){
        ENTRY min = get_min_number_2(data, group_num,index_of_blk,use_of_group);
        if (min.AC == -2){
            write_out = true;
            continue;
        }else if (min.AC == -1){
            if (use_of_group[min.BD] == 8){
                use_of_group[min.BD]++;
                continue;
            }
            freeBlockInBuffer(data[min.BD],&buf);
            read_block(&buf, &data[min.BD], start+8*min.BD+use_of_group[min.BD], false);
            use_of_group[min.BD]++;
            continue;
        }
        save_data(output, min.AC, min.BD, output_count);
        output_count++;
        if (output_count == 7){
            fill_next_index(output, output_block, output_count);
            write_block(&buf, output_block, output);
            if (is_print){
                printf("写出组间排序数据块 %d\n",output_block);
            }
            output_block++;
            output_count = 0;
            freeBlockInBuffer(output,&buf);
            output = getNewBlockInBuffer(&buf);
        }
    }
    //free blocks
    for (int i=0;i<group_num;i++){
        freeBlockInBuffer(data[i],&buf);
    }
}

ENTRY get_min_number_2(unsigned char** data, int group_num, int index_of_blk[], int use_of_group[]){
    int min_index = -1;
    ENTRY ans;
    ans.AC = 1000000;
    bool use_all = true;
    for (int i=0;i<group_num;i++){
        if (use_of_group[i] == 9)
            continue;
        if (index_of_blk[i] == 7){
            index_of_blk[i] = 0;
            ans.AC = -1;
            ans.BD = i;
            return ans;
        }
        use_all = false;
    }
    if (use_all){
        ans.AC = -2;
        return ans;
    }
    for (int i = 0; i < group_num; ++i) {
        if (use_of_group[i] < 9){
            ENTRY temp;
            temp.AC = string_int_1(data[i], index_of_blk[i]);
            temp.BD = string_int_2(data[i], index_of_blk[i]);
            if (compare(&temp, &ans) == -1){
                ans = temp;
                min_index = i;
            }
        }
    }
    index_of_blk[min_index]++;
    return ans;
}
