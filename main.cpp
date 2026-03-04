#include <iostream>
#include <bipair.h>
#include <vector>

using namespace std;

int main() {
    std::vector<std::pair<string, string>> testPair = {{"A" , "B"}};
    Bipair<string> b = Bipair(testPair);
    cout << b.getCorrespondant("A");

    return 0;
}