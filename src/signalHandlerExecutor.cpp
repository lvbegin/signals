#include <signalHandlerExecutor.h>
#include <csignal>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

class SignalHandlerExecutor::HandlerInfo {
public:
    int id;
    std::function<void()> handler;
    ~HandlerInfo() = default;
    static HandlerInfo *create(std::function<void()> handler) {
        return new HandlerInfo(ids++, handler);
    }

private:
    static int ids;
    HandlerInfo(int id, std::function<void()> handler) : id(id), handler(handler) { }
};

int  SignalHandlerExecutor::HandlerInfo::ids = 0;

int SignalHandlerExecutor::addHandler(int sig, std::function<void()> handler) {
    std::lock_guard<std::mutex> l(m);

    if (!isCatchable(sig))
        throw std::runtime_error("Error: uncatchable signal");
    auto handlerInfo = HandlerInfo::create(handler);
    all_handlers[sig].push_back(handlerInfo); 
    return handlerInfo->id;
}

void SignalHandlerExecutor::removeHandler(int sig, int handlerId) {
    std::lock_guard<std::mutex> l(m);

    if (!isCatchable(sig))
        throw std::runtime_error("Error: uncatchable signal");
    auto handlers = all_handlers[sig];
    auto it = std::find_if(handlers.begin(), handlers.end(), [handlerId](auto it) { return handlerId == it->id;});
    if (it == handlers.end())
        throw std::out_of_range("handler does not exist");
    delete *it;
    handlers.erase(it);
    all_handlers[sig] = std::move(handlers);
}

bool SignalHandlerExecutor::isCatchable(int sig) {
    auto b = uncatchableSignals.begin();
    auto e = uncatchableSignals.end();
    return e == std::find(b, e, sig);
}

void SignalHandlerExecutor::start() {
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

std::mutex SignalHandlerExecutor::m;
const std::vector<int> SignalHandlerExecutor::uncatchableSignals = { SIGSTOP, SIGKILL, }; 
std::map<int, std::vector<SignalHandlerExecutor::HandlerInfo *>> SignalHandlerExecutor::all_handlers;
bool SignalHandlerExecutor::dummy = (SignalHandlerExecutor::start(), 0);
