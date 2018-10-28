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
   // Client(int sock, string name){} 

    //~Client(){}       
};
Server clientsSockets[5];
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


//Connect to the server and check if the client wants to exit
void clientConnect()
{
    bool connect = false;
    while(connect == false)
    { 
        cout << "\033[1;31mEnter CONNECT or EXIT \033[0m" << endl;
        int n;
        char buffer[MAXMSG];
        
        bzero(
         buffer, 
        MAXMSG);
        bzero(buffer,256);

        fgets(buffer,255,stdin);

        string leave(buffer);
        string checkConnect = "CONNECT";
        string exitProgram = "EXIT";
        if(delUnnecessary(leave) == exitProgram)
        {
            cout << "Exiting program..." << endl;
            exit(0);
        }
        if(delUnnecessary(leave) == checkConnect)
        {
            connect = true;
        }
    }
}

//Print message from server
void printMessage(int sockfd)
{
    int n;
    char buffer[MAXMSG];
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if(n == 0)
    {
        return;
    }
    if (n < 0)
    {
        error("ERROR reading from socket");
    }
    printf("%s\n",buffer);
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
//Send message to other clients or use one of the API commands
bool sendMessage(int sockfd)
{
    int n;
    char buffer[MAXMSG];
    
    bzero(
        buffer, 
        MAXMSG);
    bzero(buffer,256);

    fgets(buffer,255,stdin);

    string leave(buffer);
    string checkLeave = "LEAVE";
    if(delUnnecessary(leave) == checkLeave)
    { 
        return true;
    }
    else
    {

        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
        {
            error("ERROR writing to socket");
        }
        return false;
    }

}

//Create the unique ID for the group with the fortune of the day
string createId()
{
    string result;
    array<char, 128> buffer2;
     
    FILE* pipe = popen("fortune -s", "r");
    if (!pipe)
    {
        std::cerr << "Couldn't start command." << std::endl;
        return 0;
    }
    while (fgets(buffer2.data(), 128, pipe) != NULL) 
    {
 
        result += buffer2.data();
    }
    auto returnCode = pclose(pipe);
    result = result + "THPHO " + getTime(); 
    return result;

}


//Disconnect from the chat server
int disconnect(int sender, struct sockaddr_in serv_addr , int addrlen)
{
    getpeername(sender , (struct sockaddr*)&serv_addr , (socklen_t*)&addrlen);
    cout << "Host disconnected, his ip: " << inet_ntoa(serv_addr.sin_addr) << ", and the port: " << UNO;
    close(sender);
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

        //int newSocket = getNewSocket(sockfd, serv_addr, addrlen);
        int emptySocket = getEmptySocket();
        clientsSockets[emptySocket].sock = sockfd;
        write(clientsSockets[emptySocket].sock, bufferGroupId,strlen(bufferGroupId));
        read(clientsSockets[emptySocket].sock, buffer, 1024);
        string username(buffer);
        clientsSockets[emptySocket].name = delUnnecessary(username);


        for (int i = 0; i < 5; i++)
        {
             cout << endl << clientsSockets[i].sock << " Name: " << clientsSockets[i].name << endl;
        }




}

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


//This is our bulcky message function, it handles the API from the client
string echoMessage(char buffer[], int sender, int val, string username, string serverId, int sockfd, struct hostent *server, struct sockaddr_in serv_addr, fd_set activeSocks, int addrlen)
{
    string leave(buffer);
    //clearing bitstuffing from string
    if(leave.length() > 2)
    { 
        leave = leave.substr(1, leave.find('\04')-1);
    }
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
    //string usernameCheck =  delUnnecessary(leave).substr(0, delUnnecessary(leave).find(","));
    string messageALL =  "";
    if(delUnnecessary(leave).length() > 6)
    { 
        messageALL =  delUnnecessary(leave).substr(0, 7);
        portNumberString = leave.substr(7,leave.length());
    }
    
    bool usernameBool = false;

   // string user =  delUnnecessary(portNumberString).substr(0, delUnnecessary(portNumberString).find(","));
    cout << "UsernameCheck2: " << usernameCheck  << "Portnumber int "<< portNumberString << endl;
    if(is_number(portNumberString))
    { 
        int portNumberInt = stoi(portNumberString);
        cout << "UsernameCheck3: " << usernameCheck  << "Portnumber int "<< portNumberInt << endl;
        if(connectServer == usernameCheck)
        {
           // connectToServer(sockfd, server, serv_addr2, activeSocks, n, DOS);
            connectToServer(sockfd, server, activeSocks, n, portNumberInt);
           // send(sockfd, buff, strlen(buff), 0);
        }
    }
    if(listServers == delUnnecessary(leave))
    {

       string serverList;
        char bufferServerList[MAXMSG] = "";
        
        for(int i = 0; i < 5; i++)
        { 
            serverList = serverList + " , " + clientsSockets[i].name;
            cout << endl << clientsSockets[i].name << endl;

        }

        sendCommand(sender, serverList);
        
    }
    cout << endl << "USernameCheck" << usernameCheck <<endl;
    cout << endl << "DelUNNess LEAVe: "<<delUnnecessary(leave) << endl;


    cout << "CMD " << endl;
        cout<< "leave1" << leave<< endl;


        leave = leave.substr(4,leave.length());

        cout << "leave2" << leave <<endl; 
        //string s = "scott>=tiger>=mushroom";
        string delimiter = ",";
        int counter = 0;
        size_t pos = 0;
        leave = leave + ",";
        string token, user,from,message;
        while ((pos = leave.find(delimiter)) != string::npos) {
            token = leave.substr(0, pos);
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
                cout<< "MEssageIf";
                message = token;
            }
            counter = counter +1;
             cout << token << endl;
        
        
        
        leave.erase(0, pos + delimiter.length());
        }
        cout << leave << endl;

        //string serverList = "CMD STUFF";
        
        //string user = leave.substr(0, leave.find(","));
        
       // string from =  leave.substr(user.length() + 1, leave.find(","));
        
      //  string message =  leave.substr(user.length()+1 + from.length() +1, leave.length());



    if(cmd ==  usernameCheck)
    {
        cout << "CMD USERTO " << user << endl; 
        cout << "CMD FROM " << from << endl; 
        cout << "MEssage: " << message << endl;
        for(int i = 0; i < 5; i++)
        { 
            cout << endl << clientsSockets[i].name << "=" << user;

           
            if(clientsSockets[i].name == user)
            {
                cout << "USER SIGUR";

                sendCommand(clientsSockets[i].sock, message);
                //strcpy(bufferCMD, message.c_str());
      
                //write(clientsSockets[i].sock, bufferCMD, strlen(bufferCMD));
            }

        }
        
    }

    if(RSP == usernameCheck)
    {
        cout << "CMD USERTO " << user << endl; 
        cout << "CMD FROM " << from << endl; 
        cout << "MEssage: " << message << endl;
        string serverList = "";

        for(int i = 0; i < 5; i++)
        { 
            cout << endl << clientsSockets[i].name << "=" << from;

           
            if(clientsSockets[i].name == from)
            {
                cout << endl << "Message:" << message <<"ListServers:" << listServers << endl;
                if(message == listServers)
                {
                    cout << "Listservers RSP";
                    char bufferServerList[MAXMSG] = "";
        
                    for(int i = 0; i < 5; i++)
                        { 
                         serverList = serverList + " , " + clientsSockets[i].name;
                        cout << endl << clientsSockets[i].name << endl;

                        }

                }
                cout << "USER SIGUR";
               // strcpy(bufferRSP, serverList.c_str());
                sendCommand(clientsSockets[i].sock, serverList);
      
              //  write(clientsSockets[i].sock, bufferRSP, strlen(bufferRSP));
            }

        }

    }

    if(usernameCheck == MSG)
    {
        leave = leave.substr(3,leave.length());
        string user =  delUnnecessary(leave).substr(0, delUnnecessary(leave).find(" "));
        string message =  delUnnecessary(leave).substr(delUnnecessary(leave).find(" "), delUnnecessary(leave).length());
        int n = message.length();
        char retMessage[n+1];
        strcpy(retMessage, message.c_str());
        if(user == ALL)
        {
            //Send message to all users
            goto messageAllUsers;
        }
        for(int  i = 0; i < MAXUSER; i++)
        {
            //send message to one user MSG <USERNAME> "message we want to send"
            if(user == clientsSockets[i].name)
            {
                user = user + ":";
                int n = user.length();
                char retUser[n+1];
                strcpy(retUser, user.c_str());
                //Send the username
                send(clientsSockets[i].sock, userArr, strlen(userArr), 0);
                //Send the message
                send(clientsSockets[i].sock, retMessage, strlen(retMessage), 0);
            }
        }
        usernameBool = true;
    }
    if(usernameBool == false)
    {
        //Printing the list of users connected
        if(delUnnecessary(leave) == checkWHO)
        {
            for(int  i = 0; i < MAXUSER; i++)
            {
                if(clientsSockets[i].name != emptyChecker)
                { 
                    string currUser = clientsSockets[i].name;
                    currUser = currUser + ": ";
                    int n = currUser.length();
                    char userArr[n+1];
                    strcpy(userArr, currUser.c_str());
                    send(sender, userArr, strlen(userArr), 0);
                }
            }
        }
        //Sending the ID of the server to the client
        else if(delUnnecessary(leave) == checkId)
        {
            int n = serverId.length();
            char userArr[n+1];
            strcpy(userArr, serverId.c_str());
            send(sender, userArr, strlen(userArr), 0);
        }
        //Creating a new ID for the server
        else if(leave == checkChangeId)
        {
            serverId = createId();
        }
        //This if statement tends not to be true and is only used for our goto to work
        if (1 == 2)
        { 
            //Sending message to all users
            messageAllUsers:
            for (int i = 0; i < MAXUSER; i++) 
            {
                if(clientsSockets[i].sock != sender)
                {
                    //Send the username of the sender;
                    send(clientsSockets[i].sock, userArr, strlen(userArr), 0);
                    //Sending the message
                    send(clientsSockets[i].sock, buff, strlen(buff), 0);            
                }
            }
        }
    }

    cout <<endl << "END of ECHO MESSAGE FUNC " << endl;

    return serverId;
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

   // Server servers[5];
    int sockfd, n;
    int portno = UNO;
    //struct sockaddr_in serv_addr2;           // Socket address structure
    struct hostent *server;
    fd_set activeSocks, readySocks;

    int client1, max_sd;
    int client2;
    //int sockfd;
    //int clientsSockets[MAXUSER];
    
    int opt = 1;
    int val = 0;
    fd_set readfds;
    //string userNames[MAXUSER];
    struct sockaddr_in serv_addr;


    Server emptyServer;
    emptyServer.sock = 0;
    emptyServer.name = "empty";

    string serverId = createId();

   
    for (int i = 0; i < 5; i++)
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
    string  groupId = "server2";
    char bufferGroupId[MAXMSG] = "";
    strcpy(bufferGroupId, groupId.c_str());


    while(1)
    {
        char buffer[MAXMSG];
        
        FD_ZERO(&readfds);   
        FD_SET(sockfd, &readfds);
        setTheSet( readfds, sockfd);
        select(FD_SETSIZE ,&readfds ,NULL ,NULL ,NULL);

        if (FD_ISSET(sockfd, &readfds)) 
        {
            cout << "NEW CLIENT HAPPENING" << endl;
            int newSocket = getNewSocket(sockfd, serv_addr, addrlen);
            int emptySocket = getEmptySocket();
            clientsSockets[emptySocket].sock = newSocket;

            write(clientsSockets[emptySocket].sock, bufferGroupId,strlen(bufferGroupId));


            read(clientsSockets[emptySocket].sock, buffer, 1024);
            string username(buffer);
            clientsSockets[emptySocket].name = delUnnecessary(username);


        }
            
        for (int i = 0; i < MAXUSER; i++) 
        {
            int sender = clientsSockets[i].sock;
            
            if (FD_ISSET(sender , &readfds)) 
            {
                if ((val = read( sender , buffer, 1024)) == 0)
                {
                    cout << "DISCONNECT HAPPENING" << endl;
                    clientsSockets[i].sock = disconnect(sender, serv_addr, addrlen);
                }
                else
                {
                    cout << "SEND MESSAGE HAPPENING" << endl;
                    serverId = echoMessage(buffer, sender, val, clientsSockets[i].name, serverId, sockfd, server, serv_addr, activeSocks, addrlen);
                }
            }
        }

    }
    

    return 0; 
}