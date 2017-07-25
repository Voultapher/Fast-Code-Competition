
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    char buffer[255];
    int const N = std::atoi(argv[1]);
    for(int i = 0; i< N; ++i) {
        printf(std::to_string(i).c_str());
     }
}

