#include <signal_handler.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <memory.h>

static int handler1Called = 0;
static int handler2Called = 0;

static void handler1() { handler1Called++; }

static void handler2() {
    if (3 != handler1Called)
        exit(EXIT_FAILURE);
    handler2Called++; 
} 

int main()
{
    const int sigint_handler_id = handler_info_create(SIGINT, handler1); 

    printf("Check handler is called.\n");
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 0 == handler1Called; i++)
        sleep(1);
    if (0 == handler1Called)
        return EXIT_FAILURE;

    printf("Check handler is called a second time.\n");
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 1 == handler1Called; i++)
        sleep(1);
    if (1 == handler1Called)
        return EXIT_FAILURE;
    printf("Check that two handlers are called.\n");
    const int handler2_id = handler_info_create(SIGINT, handler2); 
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 2 == handler1Called && 0 == handler2Called; i++)
        sleep(1);
    if (2 == handler1Called)
        return EXIT_FAILURE;
    if (0 == handler2Called)
        return EXIT_FAILURE;

    printf("Check that handler can be removed.\n");
    handler_info_delete(SIGINT, handler2_id); 
    kill(getpid(), SIGINT);
    for (int i = 0; i < 5 && 3 == handler1Called; i++)
        sleep(1);
    if (3 == handler1Called)
        return EXIT_FAILURE;

    printf("Check that signal with no registered handler can be received\n");
    kill(getpid(), SIGTERM);
    handler_info_delete(SIGINT, sigint_handler_id); 
    return EXIT_SUCCESS;
}
