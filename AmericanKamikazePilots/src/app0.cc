#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc != 2) { std::exit(1); }

    int n = std::atoi(argv[1]);

    for (int i = 0; i < n; ++i) { printf("%d", i); }

    return 0;
}