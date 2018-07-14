#include <signals.h>
#include <csignal>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>


struct Signals::handlerInfo {
    std::string name;
    std::function<void()> handler;
    handlerInfo(std::string name, std::function<void()> handler) : name(name), handler(handler) { }
    ~handlerInfo() = default;
};


void Signals::addHandler(int sig, std::string name, std::function<void()> handler) {
    std::lock_guard<std::mutex> l(m);

    if (!isCatchable(sig))
        throw std::runtime_error("Error: uncatchable signal");
    all_handlers[sig].push_back(new handlerInfo(std::move(name), handler)); 
}

void Signals::removeHandler(int sig, std::string name) {
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

bool Signals::isCatchable(int sig) {
    auto b = uncatchableSignals.begin();
    auto e = uncatchableSignals.end();
    return e == std::find(b, e, sig);
}

void Signals::start() {
    sigset_t set;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, nullptr);
 
    std::thread([set]() {
        while (true)  {
            int sig;
            auto s = sigwait(&set, &sig);
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

std::mutex Signals::m;
const std::vector<int> Signals::uncatchableSignals = { SIGSTOP, SIGKILL, }; 
std::map<int, std::vector<Signals::handlerInfo *>> Signals::all_handlers;
bool Signals::dummy = (Signals::start(), 0);
