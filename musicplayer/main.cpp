#include <iostream>
#include <ostream>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <sys/select.h>
#include "musicplayer.h"

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

/*** defines for UDP *****/
#define UDP_MSGLEN 1000
#define UDP_CLIENT_TIMEUOT 1000000
//#define __UDP_CLIENT_VERBOSE__
/************************/

typedef int SOCKET;
typedef struct sockaddr_in tSockAddrIn;
#define CLOSE(x) close(x)
#define INVALID(x) x < 0

using namespace std;

int main() {
    SOCKET socketDescriptor;
    tSockAddrIn serverAddress;
    struct hostent *hostInfo;
    struct timeval timeVal;
    fd_set readSet;
    int numRead;
    char buf[UDP_MSGLEN];
    unsigned int serverPort = 4001;
    char id[1000] = "musicplayer";
    char hostName[1000] = "localhost";

    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID(socketDescriptor))
    {
        cerr << "cannot create socket\n";
        exit(1);
    }
    hostInfo = gethostbyname(hostName);
    if (hostInfo == NULL)
    {
        cout << "Error: problem interpreting host: " << hostName << "\n";
        exit(1);
    }
    // Set some fields in the serverAddress structure.
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
           hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons(serverPort);
    bool shutdownClient=false;
    do
    {
        /***********************************************************************************
        ************************* UDP client identification ********************************
        ***********************************************************************************/
        do
        {
        	string initString = "-connect";
            cout << "Sending id to server: " << id << endl;
            initString.insert(0,id);
            cout << "Sending init string to the server: " << initString << endl;
            if (sendto(socketDescriptor, initString.c_str(), initString.length(), 0,
                       (struct sockaddr *) &serverAddress,
                       sizeof(serverAddress)) < 0)
            {
                cerr << "cannot send data ";
                CLOSE(socketDescriptor);
                exit(1);
            }

            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server...";
                }
		else
		{
                	cout << "Received: " << buf << endl;

                	if (strcmp(buf,"***identified***")==0)
                    		break;
            	}
	      }
        }  while(1);
        while(1)
        {
            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;
            memset(buf, 0x0, UDP_MSGLEN);

            if (select(socketDescriptor+1, &readSet, NULL, NULL, NULL))
            {
                // Read data sent by the server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server?";
                    CLOSE(socketDescriptor);
                    exit(1);
                }

#ifdef __UDP_CLIENT_VERBOSE__
                cout << "Received: " << buf << endl;
#endif
                if (strcmp(buf,"***1***")==0)
                {
                    startMenuMusic();
                    cout << "Menu music enabled" << endl;
                }
                if (strcmp(buf,"***0***")==0)
                {
                    stopMenuMusic();
                    cout << "Menu music disabled" << endl;
                }
                if (strcmp(buf,"***shutdown***")==0)
                {
                    shutdownClient = true;
                    cout << "Client Shutdown" << endl;
                    break;
                }

                if (strcmp(buf,"***restart***")==0)
                {
                    cout << "Client Restart" << endl;
                    break;
                }
            }
            else
            {
                cout << "** Server did not respond in 1 second.\n";
            }
        }
    } while(shutdownClient==false);
    CLOSE(socketDescriptor);
    return 0;
}