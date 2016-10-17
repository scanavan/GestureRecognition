#ifndef LEAP_CAPTURE_H
#define LEAP_CAPTURE_H
#include "Leap.h"
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class LeapCapture
{
public:
	LeapCapture(bool connectToSocket=true);
	~LeapCapture();
	void Capture();
	void SendData();
private:
	Leap::Vector palmPosition;
	Leap::Vector normal;
	Leap::Vector direction;

	Leap::Controller controller;
	void SocketConnect();
	SOCKET ConnectSocket;

	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
};
#endif