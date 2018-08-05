#ifndef SIGNAL_HANDLER_EXECUTOR_H__
#define SIGNAL_HANDLER_EXECUTOR_H__

#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <functional>

class SignalHandlerExecutor {
public:
    SignalHandlerExecutor() = delete;
    static int addHandler(int sig, std::function<void()> handler); 
    static void removeHandler(int sig, int handlerId);
private:
    class HandlerInfo; 

    static bool isCatchable(int sig); 
    static void start(); 

    static std::mutex  m;
    static const std::vector<int> uncatchableSignals;
    static std::map<int, std::vector<HandlerInfo *>> all_handlers;
    static bool dummy;
};

#endif
