A simple c++ class to handler signals.
The implementation is based on the sigwait() function.
This function is called by a separate thread whose the only purpose is managing signals.
The signals are masked in all other threads; thus no signal handler is automatically executed when a signal is received by the process. Signals should not be unmasked for any thread; otherwise it is undefined if the signal is manager by the thread that unmasked that signal or if the function sigwait() returns in the thread supposed to manage signals.

Two methods are available to manage signals:
- addHandler: allows to register a handler for a signal. Several handlers can be registered for one signal. In that case, the handlers are called in the order of registration; 
- removeHandler: allows to remove a handler based on its name.
