#include <iostream>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>

using namespace std;

mutex mut;
condition_variable cond_var;

condition_variable var_cond;

atomic<bool> done;

queue<std::function<void()>> work_queue;

void consume() {
    while (true) {
        unique_lock<mutex> lock(mut);
        cond_var.wait(lock, []{ return done || !work_queue.empty(); });
        if (done)
            return;
        work_queue.front()();
        work_queue.pop();
        var_cond.notify_one();
    }
}


int main(int argc, char** argv) {
    done = false;
    vector<thread> consumers;
    for (int i = 0; i < 16; ++i) 
        consumers.emplace_back(consume);

    int N = atoi(argv[1]);

    for (int i = 0; i < N; ++i) {
        lock_guard<mutex> lock(mut);
        work_queue.push([i]() { cout << i; });
        cond_var.notify_one();
    }

    std::mutex aa;
    unique_lock<mutex> bb(aa);
    var_cond.wait(bb, [] { return work_queue.empty();} );
    done = true;

    for (auto& t : consumers) {
        cond_var.notify_all();
        t.join();
    }
}
