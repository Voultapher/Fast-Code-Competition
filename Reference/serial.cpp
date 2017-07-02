#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
    return 1;

  int i{};
  while(i < static_cast<int>(std::stoi(argv[1])))
    std::cout << i++;
}
