#include <stdlib.h>
#include <string>

using namespace std;

int big_cur_row = 1;
int big_cur_col = 0;


int* smol_pos;
int* smol_end_pos;
int* small_table_dims;
char* small_table;

char* big_table = "XTTTXXXXXTTXTXXXXXXTTTXXXXXXXXTXTTTTXXXTSTXXTXXXTXXXTTXXXTXXXTTXXXTTXXTXXXXXTTTTXXXXXXXXTXXXXXXXTTX";

void set_move(char dir, int move[2]){
    move[0] = 0;
    move[1] = 0;
    switch (dir){
    case 'D':
        move[0] = 1;    
        break;
    case 'U':
        move[0] = -1;
        break;
    case 'L':
        move[1] = -1;
        break;
    case 'R':
        move[1] = 1;
        break;
    }

    /*
        low col addr is left
        low row addr is top

        move[0] is vertical direction
        move[1] is horizontal direction
    */
}

void do_move(int move[2]){
    while(1){
        int new_big_row = big_cur_row+move[0];
        int new_big_col = big_cur_col+move[1];
        
        int new_smol_row = smol_pos[0]+move[0];
        int new_smol_col = smol_pos[1]+move[1];


        if(new_big_row >= 0
        && new_big_row < 11
        && new_big_col >= 0 
        && new_big_col < 9 
        && big_table[new_big_row*9+new_big_col] == 'T'){
            big_cur_row = new_big_row;
            big_cur_col = new_big_col;
        }if(new_smol_row >= 0
        && new_smol_row < small_table_dims[0]
        && new_smol_col >= 0
        && new_smol_col < small_table_dims[1]
        && small_table[new_smol_row*small_table_dims[1]+new_smol_col] == 'T'){
            smol_pos[0] = new_smol_row;
            smol_pos[1] = new_smol_col;
        } else {
            break;
        }
    }
}

void init_table(){
    smol_pos = (int*)malloc(2*sizeof(int));
    smol_pos[0] = 0;
    smol_pos[1] = 0;
    
    smol_end_pos = (int*)malloc(2*sizeof(int));
    smol_end_pos[0] = 4;
    smol_end_pos[1] = 4;

    small_table_dims = (int*)malloc(2*sizeof(int));
    small_table_dims[0] = 7;//num rows
    small_table_dims[1] = 5;//num cols

    small_table = (char*)malloc(35*sizeof(char));
    for(int idx = 0; idx < 35; ++idx){
        small_table[idx] = 'X';
    }
    
    for(int row = 0; row < small_table_dims[0]; row += 2){
        for(int col = 0; col < small_table_dims[1]; ++col){
            small_table[row*small_table_dims[0]+col] = 'T';
        }
        if(row%4 == 0){
            small_table[(row+2)*small_table_dims[1]-1] = 'T';
        } else {
            small_table[(row+1)*small_table_dims[1]] = 'T';
        }
    }
    /*
            0 1 2 3 4
        0   s T T T T 
        1   X X X X T
        2   T T T T T 
        3   T X X X X
        4   T T T T e`
        5   X X X X T
        6   T T T T T

            0 1 2 3 4 5 6 7 8
        0   X T T T X X X X X
        1   s T X T X X X X X
        2   X T T T X X X X X
        3   X X X T X T T T T
        4   X X X T T T X X T
        5   X X X T X X X T e`
        6   X X X T X X X T T
        7   X X X T T X X T X
        8   X X X X T T T T X
        9   X X X X X X X T X
        10  X X X X X X T T X
    */
}


bool check_win(){
    if(big_cur_row == 5
    && big_cur_col == 8
    && smol_pos[0] == smol_end_pos[0]
    && smol_pos[1] == smol_end_pos[1]){
        return true;
    }
    return false;
}

bool run_sim(const string& cmds){
    if(cmds.size() < 20){
        return -1;
    }
    init_table();
    int move[2];
    for(char dir : cmds){
        set_move(dir, move);
        do_move(move);
    }
    check_win();
}