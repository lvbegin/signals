#ifndef SIGNALS_H__
#define SIGNALS_H__

#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <functional>
#include <csignal>

class Signals {
public:
    Signals() = delete;
    static void addHandler(int sig, std::string name, std::function<void()> handler); 
    static void removeHandler(int sig, std::string name);
private:
    struct handlerInfo; 

    static bool isCatchable(int sig); 
    static void start(); 

    static std::mutex  m;
    static const std::vector<int> uncatchableSignals;
    static std::map<int, std::vector<handlerInfo *>> all_handlers;
    static bool dummy;
};

#endif
