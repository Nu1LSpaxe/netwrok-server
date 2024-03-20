#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

// Unlike TCP accept then set up client socket, 
// UDP use only one socket for sending data to all clients
//
// int recvfrom(
//   _In_        SOCKET          s,
//   _Out_       char            *buf,
//   _In_        int             len,
//   _In_        int             flags,
//   _Out_       struct sockaddr *from,
//   _Inout_     int             *fromlen
// );

int main()
{
	int iResult{ 0 };
	WSADATA wsaData;
	SOCKET RecvSocket;
	sockaddr_in RecvAddr;
	unsigned short Port = 27015;
	char RecvBuf[1024];
	int BufLen = 1024;
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	// Create a new socket to receive datagrams
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (RecvSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Bind the socket to any address and the specified port
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	iResult  = bind(RecvSocket, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"bind failed with error: %u\n", WSAGetLastError());
		closesocket(RecvSocket);
		WSACleanup();
		return 1;
	}

	// Call the recvfrom function to receive datagrams on the bound socket
	wprintf(L"Receiving datagrams...\n");
	iResult = recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		closesocket(RecvSocket);
		WSACleanup();
		return 1;
	}

	// Close the socket when finished receiving datagrams
	wprintf(L"Finished receiving. Closing socket.\n");
	iResult = closesocket(RecvSocket);
	if (iResult == SOCKET_ERROR)
	{
		wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Clean up and exit
	wprintf(L"Exiting.\n");
	WSACleanup();

	return 0;
}