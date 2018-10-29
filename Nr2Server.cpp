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
class Server
{
public:
    int sock;
    string name;
    vector <string>route;     
};
Server clientsSockets[6];

void sendCommand(int socket, string message)
{
    char buffer[MAXMSG];


     for(int i = 0; i < message.size(); i++)
        {
            //laga slash
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
        //FORWARD SLASH
        message.insert(0, 1, '\01');
        message += "\04";
        strcpy(buffer, message.c_str());

        write(socket, buffer, strlen(buffer));

}
//int currentMinute;
int keepAlive(int currentMinute){

    const time_t now = std::time(nullptr) ;
    const tm calendar_time = *std::localtime( std::addressof(now) ) ;

   int minuteRightNow = calendar_time.tm_min;
   if(currentMinute != minuteRightNow)
    {
        for(int i = 0; i < 6; i++)
        {
            if(clientsSockets[i].sock != 0)
            { 
                sendCommand(clientsSockets[i].sock, "keepAlive!");
            }
        }
       currentMinute = minuteRightNow;
    }
    return currentMinute;
}

//Get the first empty socket
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
//Get the Ip address or name of the server we want to conenct ot
string getIpAddress()
{ 
    cout << "\033[1;31mPlease enter the hostname / ip address: \033[0m";
    char buffer[MAXMSG];
        
        bzero(
         buffer, 
        MAXMSG);
        bzero(buffer,256);

        fgets(buffer,255,stdin);
        string ret(buffer);
        string ip = delUnnecessary(ret);
        return ip;

}


//https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


//https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
string getTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  string str(buffer);

  cout << str;
  return str;

}


//Disconnect from the chat server
int disconnect(int sender, struct sockaddr_in serv_addr , int addrlen, int index, Server emptyServer)
{
    getpeername(sender , (struct sockaddr*)&serv_addr , (socklen_t*)&addrlen);
    cout << "Host disconnected, his ip: " << inet_ntoa(serv_addr.sin_addr) << ", and the port: " << UNO;
    close(sender);

    clientsSockets[index] = emptyServer;
    return 0;
}




void connectToServer(int sockfd2, struct hostent *server2, fd_set activeSocks2, int n2, int portNumber)
{
    int sockfd, n;
    //int portno = TRES;
    struct sockaddr_in serv_addr;           // Socket address structure
    struct hostent *server;
    fd_set activeSocks, readySocks;

    string  groupId = "Server2";
    char bufferGroupId[MAXMSG] = "";
    strcpy(bufferGroupId, groupId.c_str());

        //clientConnect();
        //const char *ip = getIpAddress().c_str();
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
        cout << "\033[1;31mPlease enter username: \033[0m";

        char buffer[MAXMSG];
        
        bzero(
         buffer, 
        MAXMSG);
        bzero(buffer,256);

       // fgets(buffer,255,stdin);
        
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
        sendCommand(sockfd, "CMD,,server2,LISTSERVERS");

}
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



//This is our bulcky message function, it handles the API from the client
void echoMessage(char buffer[], int sender, int val, string username, int sockfd, struct hostent *server, struct sockaddr_in serv_addr, fd_set activeSocks, int addrlen, int index)
{
    
    string leave(buffer);
    if(leave.length() > 2)
    { 
        leave = leave.substr(1, leave.find('\04')-1);
    }
    string fullMEssage = leave;
    //char buffer[MAXMSG];
    cout <<endl << "BUFFER: " << leave << endl;
    //API values for if statements
    string checkWHO = "WHO";
    string checkId = "ID" ;
    string checkChangeId = "CHANGE ID" ;
    string MSG = "MSG";
    string checkMsgAll = "MSG ALL";
    string ALL = "ALL";
    string emptyChecker = "0";
    string connectServer = "SERVER";
    string listServers = "LISTSERVERS";
    string RSP = "RSP";
    string cmd ="CMD";
    string fetch ="FETCH";
    buffer[val] = '\0';
    string str(buffer);
    
    
    if (!str.empty() && str[str.length()-1] == '\n') 
    {
        str.erase(str.length()-1);
    }
    //Here we start some working with the input string from the client. 
    int a = str.length();
    char buff[a+1];
    strcpy(buff, str.c_str());
    username = username + ": ";
    int n = username.length();
    char userArr[n+1];
    strcpy(userArr, username.c_str());
   
    string portNumberString = "";
    string usernameCheck =  delUnnecessary(leave).substr(0, delUnnecessary(leave).find(","));
    string messageALL =  "";
    if(delUnnecessary(leave).length() > 6)
    { 
        messageALL =  delUnnecessary(leave).substr(0, 7);
        portNumberString = leave.substr(7,leave.length());
        cout<<endl<<"portNumberString"<< portNumberString<<endl;
    }
// cout<<endl<<"portNumberString2"<< portNumberString<<endl;
    string workingWithLeave = leave;

    if(usernameCheck == "RSP")
    {
        string RSPString = workingWithLeave.substr(workingWithLeave.find(";"),workingWithLeave.length());

        string RSPdelimiter = ";";
        int RSPcounter = -1;
        size_t RSPpos = 0;

        
        string RSPtoken;
        while ((RSPpos = RSPString.find(RSPdelimiter)) != string::npos) 
        {
            RSPtoken = RSPString.substr(0, RSPpos);

            clientsSockets[index].route.push_back(RSPtoken + "2");
        
            RSPString.erase(0, RSPpos + RSPdelimiter.length());
        }
    }
    

    
    bool usernameBool = false;
    string firstParam = "";
    string listServersCheck = leave;
    string commandForServer = "";


    string delimiter = ",";
    int counter = -1;
    size_t pos = 0;

    leave = leave + ",";
    string token, user,from,message;
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


   if(clientsSockets[index].name == "empty")
    {
        cout << "clientsSockets.NAME...from:" <<from<<endl ;
        clientsSockets[index].name = from;
        clientsSockets[index].route.push_back(from + "1");
    }

    if(clientsSockets[index].name == "verySecretClientName")
    { 
        cout<<"SECRET STUFF"<< endl;
        if(cmd ==  usernameCheck)
        {
            for(int a = 0; a < 6; a++)
            {
                cout << clientsSockets[a].name << "=?" << user<< endl;
                if(clientsSockets[a].name == user)
                { 
                    cout<<"leave:" << fullMEssage << ":" << endl;
                    sendCommand(clientsSockets[a].sock, fullMEssage);
                }
            }
        }

      //  cout << "port1" << portNumberString<< endl;
        if(is_number(portNumberString))
        { 
           // cout << "port" << portNumberString<< endl;

            int portNumberInt = stoi(portNumberString);
            if(connectServer == usernameCheck)
            {
                connectToServer(sockfd, server, activeSocks, n, portNumberInt);
            }
           
        }
        if(delUnnecessary(listServersCheck) == "LISTROUTES")
        {
            cout<< "LISTROUTES!"<< endl;
            for(int a = 0; a < 6; a++)
            {
                if(clientsSockets[a].name != "verySecretClientName" && clientsSockets[a].name != "empty" )
                { 
                    cout << "name: " << clientsSockets[a].name << endl;
                    for(string n : clientsSockets[a].route)
                    {
                        if(n != "empty2")
                        { 
                            cout << "n="<<n << endl;
                        }
                    }
                }
            }
        }

        if(listServers == delUnnecessary(listServersCheck))
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

    //cout << "CMD="<< usernameCheck << endl;
    else if(cmd ==  usernameCheck)
    {

        string rspMessage ="RSP,"+from + ",server2,";

        for(int i = 0; i < 6; i++)
        { 
            if(clientsSockets[i].name == from)
            {
                rspMessage += ","; 
                if(message == "LISTSERVERS")
                {
                    rspMessage += listServersString();
                }
                if(message == "LISTROUTES")
                {
                    //TODO LISTROUTES
                }
                if(message == "FETCH")
                {
                    rspMessage += fetchHash(commandForServer);
                }
                for(int a = 0; a < 6; a++)
                {
                    // cout <<from << ":" <<  clientsSockets[a].name <<endl;
                    if(clientsSockets[a].name == from)
                    {
                       // cout <<from << ":" <<  clientsSockets[a].name <<endl;
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
    int portno = UNO;
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

    cout << "creating socket" << endl;
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
    serv_addr.sin_port = htons(UNO);

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
    string groupId = "server2";
    char bufferGroupId[MAXMSG] = "";
    strcpy(bufferGroupId, groupId.c_str());


    while(1)
    { 
        char buffer[MAXMSG];      
        FD_ZERO(&readfds);   
        FD_SET(sockfd, &readfds);
        setTheSet( readfds, sockfd);
        select(FD_SETSIZE ,&readfds ,NULL ,NULL ,threshold);

        if (FD_ISSET(sockfd, &readfds)) 
        {
            cout << "NEW CLIENT HAPPENING" << endl;
            int newSocket = getNewSocket(sockfd, serv_addr, addrlen);
            int emptySocket = getEmptySocket();
            clientsSockets[emptySocket].sock = newSocket;
            sendCommand(newSocket, "CMD,,server2,LISTSERVERS");
        }
            
        for (int i = 0; i < MAXUSER; i++) 
        {
            int sender = clientsSockets[i].sock;
            
            if (FD_ISSET(sender , &readfds)) 
            {
                if ((val = read( sender , buffer, 1024)) == 0)
                {
                    cout << "DISCONNECT HAPPENING" << endl;
                    clientsSockets[i].sock = disconnect(sender, serv_addr, addrlen, i, emptyServer);
                }
                else
                {
                    cout << "SEND MESSAGE HAPPENING" << endl;
                    echoMessage(buffer, sender, val, clientsSockets[i].name, sockfd, server, serv_addr, activeSocks, addrlen, i);
                }
            }
        }


        currentMinute = keepAlive(currentMinute);

    }
    

    return 0; 
}