#include <stdio.h>
int compare(const char* a, const char* b){
    int i=0;
    while(a[i] && b[i]){
        if(a[i] != b[i]){
            return 0;
        }
        i++;
    }
    return !a[i] && !b[i];
    return 0;
}

int main(){
    int wood = 0;
    int brick = 0;
    int wool = 0;
    int grain = 0;
    int ore = 0;
    char line[200];

    while (fgets(line, sizeof(line)-1, stdin)){
        if (compare(line, "road\n")){
            wood++;
            brick++;
        }
        else if (compare(line,"settlement\n")){
            wood++;
            brick++;
            wool++;
            grain++;
        }
        else if (compare(line,"city\n")){
            grain += 2;
            ore += 3;
        }
        else if (compare(line,"development\n")){
            grain++;
            ore++;
            wool++;
        }
    }
    printf("%02d %02d %02d %02d %02d\n", wood, brick, wool, grain, ore);
    return 0;
}