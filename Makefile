client: Client
	./Client 127.0.0.1

Client: client.cpp
	g++ -o Client client.cpp

server: Server
	./Server

Server: server.cpp queue.hpp queue.cpp active_object.cpp active_object.hpp 
	g++ -pthread -o Server server.cpp queue.cpp active_object.cpp

guard: Guard
	./Guard

Guard: Guard.cpp
	g++ -o Guard -pthread Guard.cpp

singleton: Singleton
	./Singleton

Singleton: singleton.cpp singleton.hpp
	g++ -o Singleton singleton.cpp

test1: serverTest
	./serverTest

serverTest: server_test.cpp
	g++ -o serverTest server_test.cpp

clean:
	rm -f *.o *.out Server Client Guard Singleton serverTest