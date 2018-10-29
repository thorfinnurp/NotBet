# NotBet(BotNet)

To build the servers and clients:

g++ ./client1.cpp -o client1
g++ ./client2.cpp -o client2
g++ ./client3.cpp -o client3


g++ ./Nr1Server.cpp -o Server1
g++ ./Nr2Server.cpp -o Server2
g++ ./Nr3Server.cpp -o Server3

and to run the servers and clients, it's important to run the servers first and then the clients:

./Server1
./Server2
./Server3

./client1
./client2
./client3

The clients are used to send commands to the servers. Client1 connect to Server1 when the client file is runned. And client2 and server2...
As the botnet is setup now, it's only for local use. If you want to connect to a server on skel.ru.is the only thing you need to change is line 204 and change the ipAddress to "skel.ru.is".

If you want to connect to another server for example connecting server2 to server3, simply write <SERVER,4569> (sever3 is on port 4569) in the client2 terminal.
Commands to write in the clients:

<SERVER,PORTNUMBER> = connecting the server the client is connected to another server.

<LISTSERVERS> = lists the servers the on the server the client is connected to.

<LISTROUTES> = Lists the routes on the servers that are connected to this server. If the Server string ends on 1, that means that the sever is connected straight to that server, If the servers string ends on 2 then the Server is one hop away. It's important to first run <CMD,toServer,fromServer,LISTSERVERS> so that the routing table updates it self. 

<CMD,toServer,fromServer,command> = Sending a command to another server. Commands possible are LISTSERVERS, LISTROUTES, FETCH and others.

We only implemented LISTROUTES in that way it adds how many hopps away it is from the server. We have every single route listed and we don't calculate the shortest route. The next step would have been mapping the routes and use it to find the shortest route.
  
We managed to connect to Instructor server A and B. We also connected to a couple of student servers and got their hashes. We didn't put our server on Skel.

The poem is Jabberwocky by Lewis Carroll.

Here are the hashes from group 4:
ee85b62281ba8c77e8a83721683b5bcc
67d6f22c1496f25144e77361935ea263
ffbb278337131c55f78ce543de4e55fb
ffbb278337131c55f78ce543de4e55fb
287c683aedfe88bbfb62cea45b29bd08
group3:
22811dd94d65037ef86535740b98dec8
2990b1f8a30a2766e7d9ca603e22051f
dd22b70914cd2243e055d2e118741186
6f96cfdfe5ccc627cadf24b41725caa4
07cc694b9b3fc636710fa08b6922c42b

We used this website to crack their and our hashes:
https://isc.sans.edu/tools/reversehash.html


We took 3 snapshots of our LISTROUTES. 

snapshot1.png shows that we are connected to InstructorA, we can see that we are connected to I_A as the string ends on 1 - I_A1. We are also see that the instructor_test is 2 hops away as the string ends on a 2.

snapshot2.png we are connected to V_GROUP_4 and it shows what other servers and then the end of the string shows how long it is to connect to them.

snapshot3.png is from a local test.






