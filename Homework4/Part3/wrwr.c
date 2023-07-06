#include <stdio.h>

int main(){
	char buf[50000];
	while(1){
		if(scanf("%s", buf) == -1)
			break;
		printf("wrwr: %s\n", buf);
	}
}