#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc != 2) { std::exit(1); }

    int n = std::atoi(argv[1]);
    char* buffer;
    buffer  = (char*) malloc(n * 7 * sizeof(char));
    char* p = buffer;

    for (int i = 0; i < n; ++i) {
        sprintf(p, "%d", i);
        p += strlen(p);
    }
    printf("%s", buffer);

    free(buffer);

    return 0;
}