all: server client client2
	
server: server.cpp
	g++ server.cpp -std=c++11 -o server 

client: client.cpp
	g++ client.cpp -std=c++11 -o client 

clean:
	$(RM) client
	$(RM) server
