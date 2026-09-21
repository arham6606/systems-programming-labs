#include <iostream>
#include <thread>
#include <vector>

long long counter = 0;

void increment()
{
    for (long long i = 0; i < 1'000'000; ++i)
    {
        counter++;
    }
}

int main()
{
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Expected: 2000000\n";
    std::cout << "Actual:   " << counter << '\n';

    return 0;
}
