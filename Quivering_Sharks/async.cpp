
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

#include <boost/lockfree/spsc_queue.hpp>

int main(int argc, char* argv[])
{
    std::mutex mtx;
    std::condition_variable cv;
    bool done=false;

    int const N = std::atoi(argv[1]);
    boost::lockfree::spsc_queue<int> queue(N);

    std::thread consumer([&]() {
        int count = 0;
        while(true)
        {
            int param;
            if(queue.pop(&param, 1) == 1) {
                printf(std::to_string(param).c_str());
            } else {
                std::lock_guard<std::mutex> lk(mtx);
                if(done) {
                    break;
                }
            }
        }
    });

    for(int i = 0; i< N; ++i) {
        queue.push(i);
    }

    {
        std::lock_guard<std::mutex> lk(mtx);
        done = true;
    }

    consumer.join();
}

