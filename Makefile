
ALL_FILES=Client Server Guard Singleton serverTest Pollclient Pollserver
all: $(ALL_FILES)


# shared_object.so: $(ALL_FILES)
# 	g++ -shared -fPIC -pthread $(ALL_FILES) -o shared_object.so

client: Client
	./Client 127.0.0.1

server: Server
	./Server

guard: Guard
	./Guard

singleton: Singleton
	./Singleton

test1: serverTest
	./serverTest

Client: client.cpp
	g++ -o Client client.cpp

pollclient: Pollclient
	./Pollclient 127.0.0.1

pollserver: Pollserver
	./Pollserver

Server: server.cpp queue.hpp queue.cpp active_object.cpp active_object.hpp 
	g++ -pthread -o Server server.cpp queue.cpp active_object.cpp

Guard: Guard.cpp Guard.hpp TestGuard.cpp
	g++ -o Guard -pthread Guard.cpp TestGuard.cpp

Singleton: singleton.cpp singleton.hpp
	g++ -o Singleton -pthread singleton.cpp

serverTest: server_test.cpp
	g++ -o serverTest server_test.cpp

Pollclient: pollclient.cpp
	g++ -o Pollclient -pthread pollclient.cpp

Pollserver: pollserver.cpp reactor.cpp reactor.hpp
	g++ -pthread -o Pollserver pollserver.cpp reactor.cpp

clean:
	rm -f *.o *.out Server Client Guard Singleton serverTest Pollserver Pollclient