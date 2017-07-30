#include <iostream>
#include <cassert>

int main(int argc, char const* argv[])
{
  assert(argc==2);

  int N = std::stoi(argv[1]);

  //auto get_delim = [c=char(0)]() mutable { return std::exchange(c, '\n'); };

  for(int i=0; i<N; ++i)
    std::cout << i;
    //printf("%d", i);

  return 0;
}
