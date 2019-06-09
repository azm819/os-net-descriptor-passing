GCC := g++ -std=c++14

all: server client

server: server.cpp
	$(GCC) -o server server.cpp

client: client.cpp
	$(GCC) -o client client.cpp

runServer: server
	./server

runClient: client
	./client

clean:
	rm server client