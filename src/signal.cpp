#include <cstdlib>
#include <iostream>
#include <csignal>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

class Signals {
public:
    Signals() = delete;
    static void addHandler(int sig, std::string name, std::function<void()> handler) {
        std::lock_guard<std::mutex> l(m);

        if (!isCatchable(sig))
            throw std::runtime_error("Error: uncatchable signal");
        all_handlers[sig].push_back(new handlerInfo(std::move(name), handler)); 
    }

    static void removeHandler(int sig, std::string name) {
        std::lock_guard<std::mutex> l(m);

        if (!isCatchable(sig))
            throw std::runtime_error("Error: uncatchable signal");
        auto handlers = all_handlers[sig];
        auto it = std::find_if(handlers.begin(), handlers.end(), [&name](auto it) { return name == it->name;});
        if (it == handlers.end())
            throw std::out_of_range("handler does not exist");
        delete *it;
        handlers.erase(it);
        all_handlers[sig] = std::move(handlers);
    }

private:
    struct handlerInfo {
        std::string name;
        std::function<void()> handler;
        handlerInfo(std::string name, std::function<void()> handler) : name(name), handler(handler) { }
        ~handlerInfo() = default;
    } ;

    static bool isCatchable(int sig) {
        auto b = uncatchableSignals.begin();
        auto e = uncatchableSignals.end();
        return e == std::find(b, e, sig);
    }

    static void start() {
        sigset_t set;
        sigfillset(&set);
        pthread_sigmask(SIG_BLOCK, &set, nullptr);
 
        std::thread([set]() {
            while (true)  {
                int sig;
                auto s = sigwait(&set, &sig);
                std::cout << "Signal received: " << sig << std::endl;
                {
                    std::lock_guard<std::mutex> l(m);
                    auto handlers = all_handlers[sig];
                    std::for_each(handlers.begin(), handlers.end(), [] (auto *h) {
                        h->handler();
                    });
                }    
            }
        }).detach();
    }
    static std::mutex  m;
    static const std::vector<int> uncatchableSignals;
    static std::map<int, std::vector<handlerInfo *>> all_handlers;
    static bool dummy;
};

std::mutex Signals::m;
const std::vector<int> Signals::uncatchableSignals = { SIGSTOP, SIGKILL, }; 
std::map<int, std::vector<Signals::handlerInfo *>> Signals::all_handlers;
bool Signals::dummy = (Signals::start(), 0);

#include <unistd.h>
int main()
{
    Signals::addHandler(SIGINT, "handler1", []() { std::cout << "Handler called" << std::endl;});
    Signals::addHandler(SIGINT, "handler2", []() { std::cout << "Handler called bis" << std::endl;});

    for (int i = 0 ; i < 10; i++) 
    {
        std::cout << i << std::endl;
        sleep(1);
    }

    Signals::removeHandler(SIGINT, "handler1");
    for (int i = 0 ; i < 10; i++) 
    {
        std::cout << i << std::endl;
        sleep(1);
    }
    Signals::removeHandler(SIGTERM, "handler1");


    while (true) sleep(1);
    return EXIT_SUCCESS;
}