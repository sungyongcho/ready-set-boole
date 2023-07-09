#include <iostream>
#include <cstdint>

#include "utils.hpp"

int main() {
    std::cout << "Mapped value: " << map(123, 456) << std::endl;
    reverse_map(map(123,456));
    std::cout << "Mapped value: " << map(1, 4) << std::endl;
    reverse_map(map(1,4));
    std::cout << "Mapped value: " << map(5, 3) << std::endl;
    reverse_map(map(5,3));
}
