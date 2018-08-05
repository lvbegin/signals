#ifndef SIGNAL_HANDLER_H__
#define SIGNAL_HANDLER_H__

#include <functional>

class SignalHandler {
public:
	SignalHandler(int sig, std::function<void(void)> handler) ;
        ~SignalHandler();
private:
        const int sig;
        const int id;
};

#endif
