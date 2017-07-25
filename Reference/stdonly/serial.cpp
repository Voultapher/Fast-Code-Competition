#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 1;

  int i{};
  const auto max = static_cast<int>(std::stoi(argv[1]));
  while(i < max)
  {
    printf("%d", i++);
  }
}
