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


//Disconnect from the chat server
int disconnect(int sender, struct sockaddr_in serv_addr , int addrlen)
{
    getpeername(sender , (struct sockaddr*)&serv_addr , (socklen_t*)&addrlen);
    cout << "Host disconnected, his ip: " << inet_ntoa(serv_addr.sin_addr) << ", and the port: " << UNO;
    close(sender);
    return 0;
}


//This is our bulcky message function, it handles the API from the client
string echoMessage(char buffer[], int clientsSockets[], int sender, int val, string username, string userNames[], string serverId)
{
    //API values for if statements
    string checkWHO = "WHO";
    string checkId = "ID" ;
    string checkChangeId = "CHANGE ID" ;
    string MSG = "MSG";
    string checkMsgAll = "MSG ALL";
    string ALL = "ALL";
    string emptyChecker = "0";
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
    string leave(buffer);
  
    string usernameCheck =  delUnnecessary(leave).substr(0, delUnnecessary(leave).find(" "));
    string messageALL =  delUnnecessary(leave).substr(0, 6);
    bool usernameBool = false;
    

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
            if(user == userNames[i])
            {
                user = user + ":";
                int n = user.length();
                char retUser[n+1];
                strcpy(retUser, user.c_str());
                //Send the username
                send(clientsSockets[i], userArr, strlen(userArr), 0);
                //Send the message
                send(clientsSockets[i], retMessage, strlen(retMessage), 0);
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
                if(userNames[i] != emptyChecker)
                { 
                    string currUser = userNames[i];
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
                if(clientsSockets[i] != sender)
                {
                    //Send the username of the sender;
                    send(clientsSockets[i], userArr, strlen(userArr), 0);
                    //Sending the message
                    send(clientsSockets[i], buff, strlen(buff), 0);            
                }
            }
        }
    }

    return serverId;
}
//Get the first empty socket
int getEmptySocket(int clientsSockets[])
{
    for (int i = 0; i < MAXUSER; i++) 
    {
        if(clientsSockets[i] == 0)
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
//Creating the FDSET
void setTheSet(int clientsSockets[], fd_set &readfds, int maxSd)
{
    int sd = 0;
    
    for (int i = 0 ; i < MAXUSER ; i++) 
        {
            sd = clientsSockets[i];
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

    int client1, max_sd;
    int client2;
    int sockfd;
    int clientsSockets[MAXUSER];
    char buffer[MAXMSG];
    int opt = 1;
    int val = 0;
    fd_set readfds;
    string userNames[MAXUSER];
    struct sockaddr_in serv_addr;

    string serverId = createId();
   
    for (int i = 0; i < MAXUSER; i++){

        clientsSockets[i] = 0;
        userNames[i] = "0";

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

    while(1)
    {

        FD_ZERO(&readfds);   
        FD_SET(sockfd, &readfds);
        setTheSet(clientsSockets, readfds, sockfd);
        select(FD_SETSIZE ,&readfds ,NULL ,NULL ,NULL);

        if (FD_ISSET(sockfd, &readfds)) 
        {
            int newSocket = getNewSocket(sockfd, serv_addr, addrlen);
            int emptySocket = getEmptySocket(clientsSockets);
            clientsSockets[emptySocket] = newSocket;
            read(clientsSockets[emptySocket], buffer, 1024);
            string username(buffer);
            userNames[emptySocket] = delUnnecessary(username);

        }
            
        for (int i = 0; i < MAXUSER; i++) 
        {
            int sender = clientsSockets[i];
            
            if (FD_ISSET( sender , &readfds)) 
            {
                if ((val = read( sender , buffer, 1024)) == 0)
                {
                    clientsSockets[i] = disconnect(sender, serv_addr, addrlen);
                }
                else
                {
                    serverId = echoMessage(buffer, clientsSockets, sender, val, userNames[i], userNames, serverId);
                }
            }
        }

    }
    

    return 0; 
}
