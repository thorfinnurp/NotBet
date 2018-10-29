/*
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;
// DEFINES _____________________________________________
// http://www.gnu.org/software/libc/manual/html_node/Server-Example.html
//  Sja myndina i tessari grein
// https://simpledevcode.wordpress.com/2016/06/16/client-server-chat-in-c-using-sockets/
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <algorithm>
#include <string>
#include <array>
#include <ctime>
#include <vector>



#include <netdb.h> 


using namespace std;
#define MAXUSER 10
#define MAXMSG  256     // Max length of message
#define UNO     4566    // First port
#define DOS     4568    // Second
#define TRES    4569    // Third

//We read this article before starting on our server: https://www.binarytides.com/multiple-socket-connections-fdset-select-linux/


void error(const char *msg) 
{
    perror(msg);
    exit(1);
}
//Keeps information about the servers that are connected
class Server
{
public:
    int sock;
    string name;
    vector <string>route;     
};

//Array of Servers
Server clientsSockets[6];


//This function is used for communication with other functions
//It alsi bitstuffs every message/command we send
void sendCommand(int socket, string message)
{
    char buffer[MAXMSG];
    for(int i = 0; i < message.size(); i++)
    {
        //Somthing That should be done
        if(message[i] == '\01')
        {
            //faera allt til um einn og inserta 01 a i +1
            //insert i +1
        }
        //laga slash
        if(message[i] == '\04')
        {
            //faera allt til um einn og inserta 04 a i +1
        }
    }

    message.insert(0, 1, '\01');
    message += "\04";

    strcpy(buffer, message.c_str());
    write(socket, buffer, strlen(buffer));

}

//This function checks if a minute has passed since it was las called and if the minute has
//passed then it sends keep alive message to other servers connected
int keepAlive(int currentMinute){

    const time_t now = std::time(nullptr) ;
    const tm calendar_time = *std::localtime( std::addressof(now) ) ;

   int minuteRightNow = calendar_time.tm_min;
   if(currentMinute != minuteRightNow)
    {
        for(int i = 0; i < 6; i++)
        {
            if(clientsSockets[i].sock != 0 && clientsSockets[i].name != "verySecretClientName")
            { 
                sendCommand(clientsSockets[i].sock, "keepAlive!");
            }
        }
       currentMinute = minuteRightNow;
    }
    return currentMinute;
}

//Get the first empty socket in our array of servers
int getEmptySocket()
{
    for (int i = 0; i < 5; i++) 
    {
        if(clientsSockets[i].sock == 0)
        {
            return i;
        }
    }
}


//Create a new socket
int getNewSocket(int sockfd, struct sockaddr_in serv_addr,int addrlen)
{
    int newSocket;
    if ((newSocket = accept(sockfd, (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen)) < 0)
    {
        error("Error on accept");
    }
    return newSocket;
}

//https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c
//clear empty spaces from string
string delUnnecessary(string &str)
{
    int size = str.length();
    for(int j = 0; j<=size; j++)
    {
        for(int i = 0; i <=j; i++)
        {
            if(str[i] == ' ' && str[i+1] == ' ')
            {
                str.erase(str.begin() + i);
            }
            else if(str[0]== ' ')
            {
                str.erase(str.begin());
            }
            else if(str[i] == '\0' && str[i-1]== ' ')
            {
                str.erase(str.end() - 1);
            }
        }
    }
    //Clear newlines from string
    if (!str.empty() && str[str.length()-1] == '\n') 
    {
        str.erase(str.length()-1);
    }
    return str;
}


//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//Disconnect from the chat server
int disconnect(int sender, struct sockaddr_in serv_addr , int addrlen, int index, Server emptyServer)
{
    getpeername(sender , (struct sockaddr*)&serv_addr , (socklen_t*)&addrlen);
    cout << "Host disconnected, his ip: " << inet_ntoa(serv_addr.sin_addr) << ", and the port: " << DOS;
    close(sender);
    //Make space for new servers
    clientsSockets[index] = emptyServer;
    return 0;
}



//This function is used for connecting to new servers
void connectToServer(int sockfd2, struct hostent *server2, fd_set activeSocks2, int portNumber)
{
    int sockfd, n;
    struct sockaddr_in serv_addr;           // Socket address structure
    struct hostent *server;
    fd_set activeSocks, readySocks;

    string  groupId = "V_GROUP_20";
    char bufferGroupId[MAXMSG] = "";
    strcpy(bufferGroupId, groupId.c_str());

    string ipAddress = "127.0.0.1";
    const char *ip = ipAddress.c_str();
    sockfd = socket(
        AF_INET, 
        SOCK_STREAM, 
        0);

    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    server = gethostbyname(ip);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero(
        (char *) &serv_addr, 
        sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portNumber);
    
    char buffer[MAXMSG];
    
    bzero(
     buffer, 
    MAXMSG);
    bzero(buffer,256);

    if (n < 0)
    {
        error("ERROR writing to socket");
    }

    if  (connect(
            sockfd,
            (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) {

        error("ERROR connecting");
    }

    int addrlen = sizeof(serv_addr);
    int emptySocket = getEmptySocket();
    clientsSockets[emptySocket].sock = sockfd;
    //This is done to get the name of the server we are connecting to
    sendCommand(sockfd, "CMD,,V_GROUP_20,LISTSERVERS");

}

//Returns a list of servers connected to our server
string listServersString()
{
    string serverList;
        char bufferServerList[MAXMSG] = "";
        
        for(int i = 0; i < 6; i++)
        { 
            if(i != 0 || clientsSockets[i].name != "verySecretClientName")
            {
                serverList += ";";
            }
            if(clientsSockets[i].name != "verySecretClientName")
            { 
                serverList +=  clientsSockets[i].name;
            }
        }
    return serverList;
}

//returns the index hashed asked for
string fetchHash(string index)
{
    if(index == "1")
    {
        return "be5d5d37542d75f93a87094459f76678";
    }
    if(index == "2")
    {
        return "6a7f6c24b6ea8591257217ef47bf0480";
    }
    if(index == "3")
    {
        return "8bf8854bebe108183caeb845c7676ae4";
    }
    if(index == "4")
    {
        return "0d149b90e7394297301c90191ae775f0";
    }
    if(index == "5")
    {
        return "d80ea254f1b207e19040e7932e958d1c";
    }
    return "Sorry,No Hashes with that index";
}



//This function handles every command we get from our client or other servers
void echoMessage(char buffer[], int sender, int val, int sockfd, struct hostent *server, struct sockaddr_in serv_addr, fd_set activeSocks, int addrlen, int index)
{
    //This is the pure string we received from the server/client
    string leave(buffer);
    //This deletes the bitstuffing from the command we received from the server/client
    if(leave.length() > 2)
    { 
        leave = leave.substr(1, leave.find('\04')-1);
    }
    //this is used for when we want to send a CMD from our client to other servers
    string fullMEssage = leave;

    cout <<endl << "BUFFER: " << leave << endl;
    
    //Here we start some working with the input string from the client. 

    string portNumberString = "";
    string usernameCheck =  delUnnecessary(leave).substr(0, delUnnecessary(leave).find(","));
    string messageALL =  "";
    //Working with the string from the server/clinet
    //I now know it's not such a good idea using substr when working with strings since it causes 
    //the program to crash if sth is out of index
    if(delUnnecessary(leave).length() > 6)
    { 
        messageALL =  delUnnecessary(leave).substr(0, 7);
        portNumberString = leave.substr(7,leave.length());
    }
    string workingWithLeave = leave;
    //This if statement is used to add to the LISTTOUTES table when we get a LISTSERVICE
    //from other servers
    if(usernameCheck == "RSP")
    {
        if(workingWithLeave.find(';') != string::npos)
        { 
            string RSPString = workingWithLeave.substr(workingWithLeave.find(";"),workingWithLeave.length());

            string RSPdelimiter = ";";
            int RSPcounter = -1;
            size_t RSPpos = 0;

            
            string RSPtoken = "";
            while ((RSPpos = RSPString.find(RSPdelimiter)) != string::npos) 
            {
                RSPtoken = RSPString.substr(0, RSPpos);
                
                if(RSPtoken != "V_GROUP_20" && RSPtoken != "")
                { 
                    clientsSockets[index].route.push_back(RSPtoken + "2");
                }
                RSPString.erase(0, RSPpos + RSPdelimiter.length());
            }
        }
    }

    string firstParam = "";
    string listServersCheck = leave;
    string commandForServer = "";

    string delimiter = ",";
    int counter = -1;
    size_t pos = 0;

    leave = leave + ",";
    string token, user,from,message;
    //This while function splits up the string from the client/servers to get the commands
    while ((pos = leave.find(delimiter)) != string::npos) {
        token = leave.substr(0, pos);

        if(counter == -1)
        {
            firstParam = token;
        }
        if(counter == 0)
        {
            user = token;
        }
        if(counter == 1)
        {
            from = token;
        }
        if(counter == 2)
        {
            message = token;
        }
        if(counter == 3)
        {
            commandForServer = token;
        }
        counter = counter +1;
    
        leave.erase(0, pos + delimiter.length());
    }

   //Checks if the server doesn't have a name and if not that means we are receving out first
   //message/command from the server/client and add the name to our array. 
   if(clientsSockets[index].name == "empty")
    {
        clientsSockets[index].name = from;
        clientsSockets[index].route.push_back(from + "1");
    }

    //We didn't make a port for our client and added a "password" instead.
    //Only the client can execute the commands in this if statement or other servers
    //if they discover our password
    if(clientsSockets[index].name == "verySecretClientName")
    { 
        //This sends the CMD we received from our client to the appropriate server
        if("CMD" ==  usernameCheck)
        {
            for(int a = 0; a < 6; a++)
            {
                if(clientsSockets[a].name == user)
                { 
                    sendCommand(clientsSockets[a].sock, fullMEssage);
                }
            }
        }
        //Checks if the portNumberString is a number and if it is then we can connect to another server
        if(is_number(portNumberString))
        { 
            int portNumberInt = stoi(portNumberString);
            //Checks if the client wants it's server to connect to another server
            if("SERVER" == usernameCheck)
            {
                connectToServer(sockfd, server, activeSocks, portNumberInt);
            }
           
        }
        //Checks if the client want's the server to print it's Listroutes
        if(delUnnecessary(listServersCheck) == "LISTROUTES")
        {
            cout<< "LISTROUTES!"<< endl;
            for(int a = 0; a < 6; a++)
            {
                if(clientsSockets[a].name != "verySecretClientName" && clientsSockets[a].name != "empty" )
                { 
                    //print the name of the server and then it's routes
                    cout << "name: " << clientsSockets[a].name << endl;
                    for(string n : clientsSockets[a].route)
                    {
                        //Last minute mix to elimnate empty routes
                        if(n != "empty2")
                        { 
                            cout << "n=" << n << endl;
                        }
                    }
                }
            }
        }
        //This is for the client to get the LISTSERVERS from it's server
        if("LISTSERVERS" == delUnnecessary(listServersCheck))
        {

            string serverList;
            char bufferServerList[MAXMSG] = "";
            
            for(int i = 0; i < 6; i++)
            { 
                if(i != 0 || clientsSockets[i].name != "verySecretClientName")
                {
                    serverList += ";";
                }
                if(clientsSockets[i].name != "verySecretClientName")
                { 
                    serverList +=  clientsSockets[i].name;
                }
            }

            sendCommand(sender, serverList);
        }
    }

    //other servers can execute these commands and get a RSP back
    else if("CMD" ==  usernameCheck)
    {
        //This is the string we send to other servers
        string rspMessage ="RSP,"+from + ",V_GROUP_20,";

        for(int i = 0; i < 6; i++)
        { 
            if(clientsSockets[i].name == from)
            {
                rspMessage += ","; 
                if(message == "LISTSERVERS")
                {
                    rspMessage += listServersString();
                }
                if(message == "FETCH")
                {
                    rspMessage += fetchHash(commandForServer);
                }
                for(int a = 0; a < 6; a++)
                {
                    //Sending the RSP to the right server
                    if(clientsSockets[a].name == from)
                    {
                        sendCommand(clientsSockets[i].sock, rspMessage);
                    }
                }
            }
        }
    }
    
}


//Creating the FDSET
void setTheSet( fd_set &readfds, int maxSd)
{
    int sd = 0;
    
    for (int i = 0 ; i < MAXUSER ; i++) 
        {
            sd = clientsSockets[i].sock;
            if(sd > 0)
            {
                FD_SET( sd , &readfds);
            }
            if(sd > maxSd)
            {
                maxSd = sd;
            }
        }
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    int portno = DOS;
    int currentMinute = 0;
    struct hostent *server;
    fd_set activeSocks, readySocks;

    int client1, max_sd;
    int client2;
    timeval *threshold = new timeval;
    threshold->tv_usec = 250000;
   
    int opt = 1;
    int val = 0;
    fd_set readfds;
    struct sockaddr_in serv_addr;

    vector <string> emptyRoute = {};
    Server emptyServer;
    emptyServer.sock = 0;
    emptyServer.name = "empty";
    emptyServer.route = emptyRoute;

    for (int i = 0; i < 6; i++)
    {
        clientsSockets[i] =  emptyServer;
    }

    cout << "The sever is online!" << endl;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(DOS);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, MAXUSER) < 0)
    {
        error("happened while listening");
    }
   
    int addrlen = sizeof(serv_addr);
    string groupId = "V_GROUP_20";
    char bufferGroupId[MAXMSG] = "";
    strcpy(bufferGroupId, groupId.c_str());

    //This while loop keeps going while the sever is up
    //It uses select to check for incomming commands or connections
    while(1)
    { 
        char buffer[MAXMSG];      
        FD_ZERO(&readfds);   
        FD_SET(sockfd, &readfds);
        setTheSet( readfds, sockfd);
        select(FD_SETSIZE ,&readfds ,NULL ,NULL ,threshold);

        //Handles new connections from other servers
        if (FD_ISSET(sockfd, &readfds)) 
        {
            int newSocket = getNewSocket(sockfd, serv_addr, addrlen);
            int emptySocket = getEmptySocket();
            clientsSockets[emptySocket].sock = newSocket;
            sendCommand(newSocket, "CMD,,V_GROUP_20,LISTSERVERS");
        }
            
        for (int i = 0; i < MAXUSER; i++) 
        {
            int sender = clientsSockets[i].sock;
            
            if (FD_ISSET(sender , &readfds)) 
            {
                //read the command and check if someone is disconnecting
                if ((val = read( sender , buffer, 1024)) == 0)
                {
                    cout << "DISCONNECT HAPPENING" << endl;
                    clientsSockets[i].sock = disconnect(sender, serv_addr, addrlen, i, emptyServer);
                }
                //When we receive a command from another server
                else
                {
                    echoMessage(buffer, sender, val, sockfd, server, serv_addr, activeSocks, addrlen, i);
                }
            }
        }


        currentMinute = keepAlive(currentMinute);

    }
    

    return 0; 
}