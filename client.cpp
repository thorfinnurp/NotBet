#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

using namespace std;

// DEFINES
#define MAXMSG  256     // Max length of message
#define UNO     4566    // First port
#define DOS     4568    // Second
#define TRES    4569    // Third


void error(const char *msg) {
    perror(msg);
    exit(0);
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

    string message(buffer);
    string checkLeave = "LEAVE";
    if(delUnnecessary(message) == checkLeave)
    { 
        return true;
    }
    else
    {


   // char buffer[MAXMSG];


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

       // write(socket, buffer, strlen(buffer));

        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0)
        {
            error("ERROR writing to socket");
        }
        return false;
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

int main(int argc, char *argv[]) {

    int sockfd, n;
    int portno = UNO;
    struct sockaddr_in serv_addr;           // Socket address structure
    struct hostent *server;
    fd_set activeSocks, readySocks;

    while(1)
    { 

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

        serv_addr.sin_port = htons(portno);
        cout << "\033[1;31mPlease enter username: \033[0m";

        char buffer[MAXMSG];
        
        bzero(
         buffer, 
        MAXMSG);
        bzero(buffer,256);

        fgets(buffer,255,stdin);
        
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

        n = write(sockfd,buffer,strlen(buffer));
        
        FD_ZERO(&activeSocks);
        FD_SET(STDIN_FILENO, &activeSocks);
        FD_SET(sockfd, &activeSocks);
        cout << "\033[1;31mWelcome to our chat server! You can use these commands: \033[0m" << endl;
        cout << "\033[1;33mto get the id of the server: <ID> \033[0m" << endl;
        cout << "\033[1;33mto leave the server: <LEAVE> \033[0m" << endl;
        cout << "\033[1;33mto get the list of users: <WHO> \033[0m"  << endl;
        cout << "\033[1;33mto send a message to a specific user: <MSG> <USERNAME> <Your message> \033[0m" << endl;
        cout << "\033[1;33mto send a message to all the users: <MSG> <ALL> <Your message> \033[0m" << endl;
        cout << "\033[1;33mto change the ID of the server: <MSG> <ALL> <Your message> \033[0m" << endl;

        bool leaveChecker = false;
        while(leaveChecker == false)
        {

            readySocks = activeSocks;

            if(select(FD_SETSIZE, &readySocks, NULL, NULL, NULL) < 0)
            {
                error("ERROR happened while selecting");
            }

            if(FD_ISSET(sockfd, &readySocks))
            {
                printMessage(sockfd);
            }
            if(FD_ISSET(STDIN_FILENO, &readySocks))
            {
                leaveChecker = sendMessage(sockfd);
            }
        }  
        
        close(sockfd);
    }

    return 0;

}
































