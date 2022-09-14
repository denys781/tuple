#include <iostream>
#include "tuple.hpp"

int main()
{
    std::size_t num{};
    auto t = experimental::make_tuple(42, std::string("plain text"), std::ref(num));

    std::cout
        << "The value at index 0: " << experimental::get<0>(t) << '\n'
        << "The value at index 1: " << experimental::get<1>(t) << '\n'
        << "The value at index 2: " << experimental::get<2>(t) << '\n';
    return 0;
}