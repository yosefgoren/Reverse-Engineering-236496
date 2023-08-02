#include <stdio.h>

char* table;
const char cmd_str_arr[][] = {
    "DLD",
    "RDD",
    "DR",
    "DDL",
    "URD",
    "D",
    "RUR",
    "RRU"
};

int translate_direction(char dir){
    switch (dir){
    case 'U':
        return 0x14; //20       0b10100
    case 'D':
        return 0x3; //3         0b00011
    case 'R':
        return 0x11; //17       0b10001
    case 'L':
        return 0xb; //11        0b01011
    }
    exit(1);
}

void do_move(int* p_col_idx, int* p_row_idx, char direction){
    int code = translate_direction(direction);
    switch (code){
    case 0x14:
        (*p_row_idx)--;
        return;
    case 0x3:
        (*p_row_idx)++;
        return;
    case 0x11:
        (*p_col_idx)++;
        return;
    case 0xb:
        (*p_col_idx)--;
        return;
    default:
        exit(1);
    }
    if((unsigned)*p_col_idx > 9 || (unsigned)*p_row_idx > 9){
        exit(1);
    }
}

void check_victory(){
    int row = 0;//eax
    int col = 0;//edx
    char* pos = table+row*10+col;//ebx

    while(1){
        if(*pos == 'P'){
            *pos = 'B';
            if(col != 0){
                if(pos[-1] == 'P'){
                    *pos = '.';
                    continue;
                }
            }
            if(row != 0){
                if(pos[-10] == 'P'){
                    *pos = '.';
                    continue;;
                }
            }
        } else {
            if(col != 9 || row != 9 || ckck() != 0){
                exit(1);
            }
            puts("SUCCESS!");
            return;
        }
    }
}

int main(){
    //...
    char fmt_buf[14];
    char digit_buf[2];

    for(int cmd_idx = 0; cmd_idx < 8; ++cmd_idx){
        sprintf(fmt_buf, "%%%ds\n", 2);
        //now fmt_buf is "%2s"
        scanf(fmt_buf, digit_buf);
        int col_idx = digit_buf[0]-'0', row_idx = digit_buf[1]-'0';
        
        //equiv to:
        //  scanf("%2s", &col_idx, &row_idx);
        // col_idx -= '0'; row_idx -= '0';

        if(table[row_idx*10+col_idx] == 'P')
            exit(1);

        table[row_idx*10+col_idx] = 'P';

        int i = 0;// esi
        while(strlen(cmd_str_arr[cmd_idx]) <= i){
            do_move(&col_idx, &row_idx, cmd_str_arr[cmd_idx][i]);
            table[row_idx*10+col_idx] = 'P';
        }
    }
    check_victory();
    return 0;
}