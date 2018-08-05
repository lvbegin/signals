#include <signalHandler.h>
#include <signalHandlerExecutor.h>

SignalHandler::SignalHandler(int sig, std::function<void()> handler) : 
     sig(sig), id(SignalHandlerExecutor::addHandler(sig, handler)) { } 

SignalHandler::~SignalHandler() {
    SignalHandlerExecutor::removeHandler(sig, id);
}

