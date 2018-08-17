A simple c++ class to handler signals. c wrappers are also available.
The implementation is based on the sigwait() function.
This function is called by a separate thread whose the only purpose is managing signals.
The signals are masked in all other threads; thus no signal handler is automatically executed when a signal is received by the process. Signals should not be unmasked for any thread; otherwise it is undefined if the signal is manager by the thread that unmasked that signal or if the function sigwait() returns in the thread supposed to manage signals.

C++ API:
--------
Registering an handler for a signal is done by creating a object of the class SignalHandler.

Removing an hander is done by deleting the object.

Several handlers can be registered for a same signal. In that case, handlers are executed in the order of registration.

C WRAPPERS:
----------
c wrappers are available to link with c programs.
In order to execute "make check", it is probably necessary to set the LIBS environment variable to contain the directive to link with the standard c++ library. For instance, on Ubuntu 18.04, LIBS must contains -lstdc++. Otherwise the c wrapper test program may not link because some symbols defined in the c++ standard library are missing. Adding explicit linking to the standard c++ library may be also needed when linking a c program that uses the wrappers.  
