#include <iostream>
#include "tuple.hpp"

int main()
{
    std::size_t pos_num{};
    auto t = experimental::make_tuple(42, std::string("plain text"), std::ref(pos_num));

    std::cout << "The value at index 0 is: " << experimental::get<0>(t) << '\n';
    std::cout << "The value at index 1 is: " << experimental::get<1>(t) << '\n';
    std::cout << "The value at index 2 is: " << experimental::get<2>(t) << '\n';
    return 0;
}
