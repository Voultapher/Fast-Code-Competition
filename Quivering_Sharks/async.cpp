
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

int main(int argc, char* argv[])
{
    std::deque<int> queue;
    std::mutex mtx;
    std::condition_variable cv;
    bool done=false;

    std::thread consumer([&]() {
        while(true)
        {
            int param;
            int count = 0;
            {
                std::unique_lock<std::mutex> lk(mtx);
                cv.wait(lk, [&]() { return !queue.empty() || done; });
                if(queue.empty() && done) { break; }
                param = queue.front();
                queue.pop_front();
            }
            printf(std::to_string(param).c_str());    
        }
    });

    char buffer[255];
    int const N = std::atoi(argv[1]);
    //queue.reserve(N;

    for(int i = 0; i< N; ++i) {
        std::lock_guard<std::mutex> lk(mtx);
        queue.push_back(i);
        cv.notify_one();
     }
     done = true;
     cv.notify_all();
     consumer.join();
}

