#include <iostream>
int main() {
    uint64_t N,G,days;
    std::cin >> N;
    while (std::cin >> N >> G) {
        days = !!(N -= G);
        while (N &= (N - 1))
            days += 1;
        std::cout << days << std::endl;
    }
}

