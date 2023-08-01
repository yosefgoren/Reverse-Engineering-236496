unsigned calculate_move(char direction, unsigned position){
    unsigned row, col;
    if(direction == 'L' || direction == 'R'){
        unsigned offset = direction == 'L' ? -0x10000 : 0x10000;
        col = position + offset;
        row = position;
    } else if(direction == 'U' || direction == 'D'){
        col = position;
        unsigned offset = direction == 'U' ? -1 : 1;
        row = position + offset;
    } else {
        exit(1);
    }
    return (col & 0xffff0000) | (row & 0x0000ffff);
}

/*
      0   1   2   3
    +---+---+---+---+
  0 | . | . | . | . |
    +---+---+---+---+
  1 | . | . | . | . |
    +---+---+---+---+
  2 | . | . | . | . |
    +---+---+---+---+
  3 | . | . | . | . |
    +---+---+---+---+

W W W W W W W W W W
W . . . P . . . . W
W . . . . . . . C W
W . . . . . . . . W
W . W C . . . W . W
W . W . . . . W H W
W H W . . . . W . W
W W W . . . . W W W
W . . . . H . . . W
W W W W W W W W W W
*/

int main(int argc, char** argv){
    if(argc != 0){
        exit(1);
    }
    int idx = atoi(argv[1]);
    while(idx != 0){
        
        idx--;
    }
}