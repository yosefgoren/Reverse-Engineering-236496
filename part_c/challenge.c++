#include <string>
#include <iostream>
using namespace std;
int main() {
    string  line;
    int wood = 0;
    int brick = 0;
    int wool = 0;
    int grain = 0;
    int ore = 0;

    while (getline(cin, line)){
        if (line == "road"){
            wood++;
            brick++;
        }
        if (line == "settlement"){
            wood++;
            brick++;
            wool++;
            grain++;
        }
        if (line == "city"){
            grain += 2;
            ore += 3;
        }
        if (line == "development"){
            grain++;
            ore++;
            wool++;
        }

    }
    cout << "You total is - wood:" << wood <<", brick:" << brick<<", wool:"<<wool<<", grain:"<<grain<<", ore:"<<ore<< endl;
    return 0;
}