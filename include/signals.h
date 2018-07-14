#ifndef SIGNALS_H__

#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <functional>

class Signals {
public:
    Signals() = delete;
    static void addHandler(int sig, std::string name, std::function<void()> handler); 
    static void removeHandler(int sig, std::string name);
private:
    struct handlerInfo {
        std::string name;
        std::function<void()> handler;
        handlerInfo(std::string name, std::function<void()> handler) : name(name), handler(handler) { }
        ~handlerInfo() = default;
    } ;

    static bool isCatchable(int sig); 

    static void start(); 

    static std::mutex  m;
    static const std::vector<int> uncatchableSignals;
    static std::map<int, std::vector<handlerInfo *>> all_handlers;
    static bool dummy;
};

#endif
