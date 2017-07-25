#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>

void printN(int n) {
    std::async([=]{printf("%d", n);}).get();
}

// - Main thread creates tasks
// - Consumer thread, accepts tasks without blocking
// - Run loop creating and feeding tasks N times
// - First in First out

int main(int argc, char *argv[]) {

    if (argc != 2) { std::exit(1); }

    const int n = std::atoi(argv[1]);
    for (int i = 0; i < n; ++i) {
        printN(i);
    }
}