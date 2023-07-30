int count_bits(int dword){
    int cnt = 0;
    for(int i = 0; i < 32; i++){
        cnt += (dword & (1<<i));
    }
    return cnt;
}


struct row{
    char x;//+0
    char charnum;//+4
    int n;//+8
    char cond;//+12
    int tgt;//+13
};
struct row* table = 0x00407080;

void init_table(){
    // table[0] = 'A';
    // table[4] = '8';
    // table[8] = 3;
    // table[12] = 0;
    // table[13] = 0;

    // table[16] = 'F';
    // table[20] = '6';
    // table[24] = 2;
    // table[28] = 0;
    // table[29] = 0;
    
    // table[32] = 'G';
    // table[36] = '2';
    // table[40] = 2;
    // table[44] = 1;
    // table[45] = 0;

    // table[48] = 'D';
    // table[52] = '3';
    // table[56] = 4;
    // table[60] = 1;
    // table[61] = 0;

    table[0].x = 'A';       //0100 0001
    table[0].charnum = '8'; //0011 1000
    table[0].n = 3;
    table[0].cond = 0;
    table[0].tgt = 0;

    table[1].x = 'F';       //0100 0110
    table[1].charnum = '6'; //0011 0110
    table[1].n = 2;
    table[1].cond = 0;
    table[1].tgt = 0;
    
    table[2].x = 'G';       //0100 0111
    table[2].charnum = '2'; //0011 0010
    table[2].n = 2;
    table[2].cond = 1;
    table[2].tgt = 0;

    table[3].x = 'D';       //0100 0100
    table[3].charnum = '3'; //0011 0011
    table[3].n = 4;
    table[3].cond = 1;
    table[3].tgt = 0;

    /*
        A 8
        B 8
        C 8

        F 6
        G 6

        G 2
        G 3 
        
        D 3
        D 4
        D 5
        D 6
    */
}

int check_finished(){
    for(int idx = 0; idx < 4; ++idx){
        struct row* r = table+idx;
        if(count_bits(r->tgt) != r->n){
            return 0;
        }
    }
    return 1;
}

void get_input(char* p_x, char* p_charnum){
    scanf("%c", p_x);
    if(*p_x > 'H' || *p_x < 'A')
        exit(-1);
    scanf("%c", p_charnum);
    if(*p_charnum > '8' || *p_charnum < '1')
        exit(-1);
}

void do_move(char x, char charnum){
    for(int row_idx = 0; row_idx < 4; ++row_idx){
        struct row* r = table+row_idx;
        if(r->cond){
            if(x == r->x && charnum >= r->charnum && charnum < r->charnum + r->n){
                r->tgt |= 1<<(r->n + r->charnum - charnum);
            }
        } else {
            if(r->charnum == charnum && x >= r->x && x < r->x + r->n){
                r->tgt |= 1<<(r->n + r->x - x);
            }
        }
    }
}