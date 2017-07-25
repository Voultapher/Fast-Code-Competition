#include <algorithm>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <vector>

void printN(int n) {
    std::async(std::launch::async, [=] { printf("%d", n); });
}

std::deque<int> queue;
std::mutex mtx;

void task() {
    while (1) {
        mtx.lock();
        int i = queue.front();
        queue.pop_front();
        mtx.unlock();

        if (i == -1) return;

        printf("%d", i);
    }
}

// - Main thread creates tasks
// - Consumer thread, accepts tasks without blocking
// - Run loop creating and feeding tasks N times
// - First in First out

int main(int argc, char *argv[]) {

    if (argc != 2) { std::exit(1); }

    std::thread t(task);

    const int n = std::atoi(argv[1]);
    for (int i = 0; i < n; ++i) {
        mtx.lock();
        queue.push_back(i);
        mtx.unlock();
    }
    queue.push_back(-1);

    t.join();
}