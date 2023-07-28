#include <stdio.h>

int finished_flag;
int table[64];

char verifyPath(int row, int col){
	if(row <= 7 && col <= 7){
		char cell = table[row*8+col];
		if(cell != 'X'){
			return cell;
		} else {
			puts("path blocked!");
			exit(1);
		}
	}
	puts("out of bounds!");
		exit(1);
}

void Continue(int* p_row, int* p_col, int* p_direction, int steps){
	int cur_col = *p_col, cur_row = *p_row, direction = *p_direction;
	if(steps != 0){
		steps -= 1;
		if(direction == 'R'){
			if(cur_col == 7){
				cur_row +=1;
				direction = 'L';
			} else {
				cur_col += 1;
			}
		} else {
			if(direction == 'L'){
				if(cur_col == 0){
					cur_row += 1;
					direction = 'R';
				} else {
					cur_col -= 1;
				}
			}
		}
		verifyPath(cur_row, cur_col);
		*p_row = cur_row;
		*p_col = cur_col;
		*p_direction = direction;
		if(table[cur_row*8+cur_col] == 'F'){
			finished_flag += 1;
		}
		Continue(p_row, p_col, p_direction, steps);
	}
}

void MoveVertical(int *p_row, int col, char cmd_arg){
	int cmd, loc_1, tile_value, cur_row;
	cmd = cmd_arg;
	cur_row = *p_row;
	tile_value = table[cur_row*8+col];
	if(tile_value == '.' || tile_value == 'X'){
		exit(-1);
	}
	if(cmd == 'U'){
		cur_row += tile_value;
	} else {
		if(cmd == 'D'){
			cur_row -= tile_value;
		}
	}
	verifyPath(cur_row, col);
	*p_row = cur_row;
}

void GetCmd(int* p_cmd, int* p_steps){
	scanf("%c", p_cmd);
	char cmd = *p_cmd;
	if(cmd < 'C' || (cmd > 'D' && cmd != 'U')){
		exit(-1);
	}
	if(cmd == 'C'){
		scanf("%u", p_steps);
		if(*p_steps > 8)
			exit(1);
	}
	return;
}
void InitVars(int* p_row, int* p_col, int* p_direction){
	*p_row = 0;
	*p_col = 0;
	*p_direction = 'R';
	finished_flag = 0;
	memset(table, '.', 64);
	memset(table+8, 'X', 8);
	memset(table+24, 'X', 8);
	memset(table+48, 'X', 8);
	
	table[8*0+1] = 4;
	table[8*0+4] = 7;
	table[8*5+7] = 2;
	table[8*7+6] = 3;
	table[8*0+3] = 'F';
	table[8*4+7] = 'F';

	/*  
		  0   1   2   3   4   5   6   7
		+---+---+---+---+---+---+---+---+
	0	| S > 4 | . | F | 7 | . | . | . |
		+---+---+---+---+-v-+---+---+---+
	1	| X | X | X | X | X | X | X | X |
		+---+---+---+---+---+---+---+---+
	2	| . | . | . | . | . | . | . | . |
		+---+---+---+---+---+---+---+---+
	3	| X | X | X | X | X | X | X | X |
		+---+---+---+---+---+---+---+---+
	4	| . | . | . | . | . | . | > | F |
		+---+---+---+---+---+---+---+-v-+
	5	| . | . | . | . | . | . | . | 2 |
		+---+---+---+---+---+---+---+-v-+
	6	| X | X | X | X | X | X | X | X |
		+---+---+---+---+---+---+-^-+---+
	7	| E | . | . | . | > | . | 3 | < |
		+---+---+---+---+---+---+---+---+
		C 4
		U
		C 2
		D
		C 2
		U
		C 7
	*/
}

int main(){
	unclear();
	int steps, direction, col, row;
	char cmd;
	int idx = 0;
	InitVars(&row, &col, &direction);
	while(idx < 8){
		idx++;
		GetCmd(&cmd, &steps);
		if(cmd == 'C')
			Continue(&row, &col, &direction, steps);
		else
			MoveVertical(&row, col, cmd);
		if(col != 0)
			continue;
		if(row != 7)
			continue;
		if(finished_flag == 2)
			puts("Good Job! dont forget the dry part!");
		else
			exit(0);
	}
	return 0;
}