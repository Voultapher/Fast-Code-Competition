#include <iostream>
#include <cassert>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <array>

using namespace std;

mutex m;
condition_variable cond;
atomic_int produced {};


void write_cout(int N)
{
  int consumed = 0;

  unique_lock<mutex> lock(m);
  while(consumed<N-1)
  {
    for(; consumed < produced; ++consumed)
    {
      cout << consumed;
    }

    if(consumed<N-1)
      cond.wait(lock);
  }
}

int main(int argc, char const* argv[])
{
  assert(argc==2);
  int N = std::stoi(argv[1]);

  cout.sync_with_stdio(false);

  array<char, 1024> buffer;
  std::cout.rdbuf()->pubsetbuf(buffer.data(), buffer.size());


  thread writer([N](){ write_cout(N); });

  for(; produced<N; ++produced)
    cond.notify_one();

  writer.join();
  return 0;
}
