ImClient
========
This will be the client side for my chatting system. First I'll be playing
with the gtkmm library trying to create somekind of gui and when that seems
good enough the communication will be implemented using the boost library.

g++ -o client *.cc `pkg-config --cflags gtkmm-2.4` `pkg-config --libs gtkmm-2.4` -lboost_system -lboost_thread-mt -std=c++0x

That should compile if the gtkmm and boost libraries are properly installed.
