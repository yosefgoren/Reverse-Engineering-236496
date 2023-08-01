void get_input(char* direction, int* dist){
    //...
}

// char calc_move(){

// }

char* table;

void check_move(int res){
    if(res == 'X'){
        exit(-1);
    } else {
        if(res == 'E')
            printf("SUCCESS!");
    }
}

void mystery(){
    int cur_row = 0, cur_col, cnt = 0;
    while(cur_row <= 9){
        cur_col = 0;
        while(cur_col <= 9){
            cnt += (table[cur_row*10+cur_col] == '.');
        }
    }
    if(cnt == 0){
        table[5*10+0] = 'E';
    }

}

int main(){
    int row, col;
    init_table(&row, &col);//sets x=5, y=5.
    int idx = 0;
    char direction;
    int n_steps;
    int MAX_ITER = 11;
    while(idx < MAX_ITER){
        idx++;
        get_input(&direction, &n_steps);
        int count = 0;
        while(count < n_steps){
            char orig_tile_value = calc_move(&row, &col, direction);
            check_move(orig_tile_value);
            count++;
        }

        
    }
}