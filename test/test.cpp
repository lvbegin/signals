#include <signals.h>
#include <unistd.h>
#include <iostream>

int main()
{
    int handler1Called = 0;
    int handler2Called = 0;
    Signals::addHandler(SIGINT, "handler 1", [&handler1Called]() { handler1Called++; }); 
    std::cout << "Check handler is called." << std::endl;
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 0 == handler1Called; i++)
        sleep(1);
    if (0 == handler1Called)
        return EXIT_FAILURE;

    std::cout << "Check handler is called a second time." << std::endl;
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 1 == handler1Called; i++)
        sleep(1);
    if (1 == handler1Called)
        return EXIT_FAILURE;

    std::cout << "Check that two handlers are called." << std::endl;
    Signals::addHandler(SIGINT, "handler 2", [&handler1Called, &handler2Called]() { 
	if (3 != handler1Called)
            exit(EXIT_FAILURE);
        handler2Called++; 
    }); 
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 2 == handler1Called && 0 == handler2Called; i++)
        sleep(1);
    if (2 == handler1Called)
        return EXIT_FAILURE;
    if (0 == handler2Called)
        return EXIT_FAILURE;

    std::cout << "Check that handler can be removed." << std::endl;
    Signals::removeHandler(SIGINT, "handler 2");
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 3 == handler1Called; i++)
        sleep(1);
    if (3 == handler1Called)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
