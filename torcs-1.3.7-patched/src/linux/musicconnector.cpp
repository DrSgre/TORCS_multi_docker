typedef int SOCKET;
typedef struct sockaddr_in tSockAddrIn;
#define CLOSE(x) close(x)
#define INVALID(x) x < 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <sstream>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#define UDP_LISTEN_PORT 4001
#define UDP_ID "musicplayer-connect"
#define UDP_DEFAULT_TIMEOUT 10000
#define UDP_MSGLEN 1000

static int UDP_TIMEOUT = UDP_DEFAULT_TIMEOUT;
static int RESTARTING[10];

static int listenSocket[10];
socklen_t clientAddressLength[10];
tSockAddrIn clientAddress[10], serverAddress[10];

using namespace std;

void
newconnection(int index)
{
    bool identified=false;
    char line[UDP_MSGLEN];

    listenSocket[index] = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenSocket[index] < 0)
    {
        std::cerr << "Error: cannot create listenSocket!";
        exit(1);
    }
    srand(time(NULL));

    // Bind listen socket to listen port.
    serverAddress[index].sin_family = AF_INET;
    serverAddress[index].sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress[index].sin_port = htons(UDP_LISTEN_PORT+index);

    if (bind(listenSocket[index],
             (struct sockaddr *) &serverAddress[index],
             sizeof(serverAddress[index])) < 0)
    {
        std::cerr << "cannot bind socket";
        exit(1);
    }

    // Wait for connections from clients.
    listen(listenSocket[index], 5);

    std::cout << "Waiting for request on port " << UDP_LISTEN_PORT+index << "\n";

    // Loop until a client identifies correctly
    while (!identified)
    {
        //std::cout << "!identified" << "\n";

        clientAddressLength[index] = sizeof(clientAddress[index]);

        // Set line to all zeroes
        memset(line, 0x0, UDP_MSGLEN);
        if (recvfrom(listenSocket[index], line, UDP_MSGLEN, 0,
                     (struct sockaddr *) &clientAddress[index],
                     &clientAddressLength[index]) < 0)
        {
            std::cerr << "Error: problem in receiving from the listen socket";
            exit(1);
        }
        // compare received string with the ID
        if (strncmp(line,UDP_ID,3)==0)
        {
            std::string initStr(line);
            char line[UDP_MSGLEN];
            sprintf(line,"***identified***");
            // Sending the car state to the client
            if (sendto(listenSocket[index], line, strlen(line) + 1, 0,
                       (struct sockaddr *) &clientAddress[index],
                       sizeof(clientAddress[index])) < 0)
                std::cerr << "Error: cannot send identification message";
            identified=true;
        }
    }
}

/* Called before the module is unloaded */
static void
shutdownconnector(int index)
{
    if (RESTARTING[index]!=1)
    {
        char line[UDP_MSGLEN];
        sprintf(line,"***shutdown***");
        // Sending the car state to the client
        if (sendto(listenSocket[index], line, strlen(line) + 1, 0,
                   (struct sockaddr *) &clientAddress[index],
                   sizeof(clientAddress[index])) < 0)
            std::cerr << "Error: cannot send shutdown message";
    }
    else
    {
        char line[UDP_MSGLEN];
        sprintf(line,"***restart***");
        // Sending the car state to the client
        if (sendto(listenSocket[index], line, strlen(line) + 1, 0,
                   (struct sockaddr *) &clientAddress[index],
                   sizeof(clientAddress[index])) < 0)
            std::cerr << "Error: cannot send shutdown message";
    }
    RESTARTING[index]=0;
    CLOSE(listenSocket[index]);
}

void
musicmenu(int index, bool enable)
{
    // local variables for UDP
    struct timeval timeVal;
    fd_set readSet;

    /**********************************************************************
     ****************** Building state string *****************************
     **********************************************************************/

    char line[UDP_MSGLEN];
    if (enable) {
        sprintf(line,"***1***");
        std::cout << "Requested music start" << std::endl;
    }
    else {
        sprintf(line,"***0***");
        std::cout << "Requested music stop" << std::endl;
    }

    if (RESTARTING[index]==0)
    {
        // Sending the car state to the client
        if (sendto(listenSocket[index], line, strlen(line) + 1, 0,
                (struct sockaddr *) &clientAddress[index],
                sizeof(clientAddress[index])) < 0)
            std::cerr << "Error: cannot send car state";
    }
}