#include <iostream>
#include <cstdlib>

// ./a.out -> argc = 1, argv = ["./a.out"]
// ./a.out 10 -> argc = 2, argv = ["./a.out", "10"]

void print_numbers(int min, int max);

int main(int argc, char **argv)
{
    int a = std::atoi(argv[1]);
    int b = std::atoi(argv[2]);
    print_numbers(a, b);
    return 0;
}

void print_numbers(int min, int max)
{
    for (int ii = min; ii <= max; ++ii) {
        std::cout << ii << std::endl;
    }
}
