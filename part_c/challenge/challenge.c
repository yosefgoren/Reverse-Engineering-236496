#include <stdio.h>
#include <string.h>
// int compare(const char* a, const char* b){
//     int i=0;
//     while(a[i] && b[i]){
//         if(a[i] != b[i]){
//             return 0;
//         }
//         i++;
//     }
//     return !a[i] && !b[i];
//     return 0;
// }

int main(){
    int wood = 0;
    int brick = 0;
    int wool = 0;
    int grain = 0;
    int ore = 0;
    char line[200];

    while (fgets(line, sizeof(line)-1, stdin)){
        if (strcmp(line, "road\n")==0 || strcmp(line, "road")==0){
            wood++;
            brick++;
        }
        if (strcmp(line,"settlement\n")==0 || strcmp(line,"settlement")==0){
            wood++;
            brick++;
            wool++;
            grain++;
        }
        if (strcmp(line,"city\n")==0 || strcmp(line,"city")==0){
            grain += 2;
            ore += 3;
        }
        if (strcmp(line,"development\n")==0 || strcmp(line,"development")==0){
            grain++;
            ore++;
            wool++;
        }
    }
    printf("%02d %02d %02d %02d %02d\n", wood, brick, wool, grain, ore);
    return 0;
}