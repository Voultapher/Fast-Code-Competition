#include <algorithm>
#include <atomic>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
#include <numeric>
#include <vector>

std::deque<int> queue;
std::atomic_flag lock = ATOMIC_FLAG_INIT;

void task() {
    while (1) {
        while (lock.test_and_set(std::memory_order_acquire)) // acquire lock
            ;                                                // spin
        int i = queue.front();
        queue.pop_front();
        lock.clear(std::memory_order_release);

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
        while (lock.test_and_set(std::memory_order_acquire)) // acquire lock
            ;                                                // spin
        queue.push_back(i);
        lock.clear(std::memory_order_release);
    }
    queue.push_back(-1);

    t.join();
}