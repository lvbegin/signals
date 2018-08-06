A simple c++ class to handler signals.
The implementation is based on the sigwait() function.
This function is called by a separate thread whose the only purpose is managing signals.
The signals are masked in all other threads; thus no signal handler is automatically executed when a signal is received by the process. Signals should not be unmasked for any thread; otherwise it is undefined if the signal is manager by the thread that unmasked that signal or if the function sigwait() returns in the thread supposed to manage signals.

Registering an handler for a signal is done by creating a object of the class SignalHandler
Remove an hander is done by deleting the object.

Several handlers can be registered for a same signal. In that case, handlers are executed in the order of registration.
