#include <iostream>
#include <sistream.h>

int main()
{
    sistream<char, std::function<void(std::string const&)>> scin{std::cin, " ", [](std::string const& a){std::cout << a << std::endl;}};
    std::cout << "Hello world." << std::endl;
    return 0;
}

