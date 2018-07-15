#include <signals.h>
#include <unistd.h>
#include <iostream>

int main()
{
    Signals::addHandler(SIGINT, "handler 1", []() { std::cout << "Handler called" << std::endl;});
    return EXIT_SUCCESS;
}
