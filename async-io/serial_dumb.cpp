#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    int total = atoi(argv[1]);
    for (int i = 0; i <= total; ++i)
        cout << i << '\n';
}
