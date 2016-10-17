#include "LeapCapture.h"
#include <iostream>

LeapCapture::LeapCapture(bool connectToSocket) :
	palmPosition(0.f, 0.f, 0.f)
{
	if (connectToSocket)
	{
		SocketConnect();
	}
}
LeapCapture::~LeapCapture()
{
	closesocket(ConnectSocket);
	WSACleanup();
}
void LeapCapture::SocketConnect()
{
	int iResult;

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	WSADATA wsaData;

	ConnectSocket = INVALID_SOCKET;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("149.125.127.206", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		std::cout << "Creating socket." << std::endl;
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
		}
		std::cout << "connecting to socket" << std::endl;
		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		std::cout << WSAGetLastError() << std::endl;
		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET)
	{
		std::cerr << "Unable to connect to server!" << std::endl;
		WSACleanup();
	}

}
void LeapCapture::SendData()
{
	if (palmPosition.x != 0 && palmPosition.y != 0 && palmPosition.z != 0)
	{
		sprintf(sendbuf, "%f %f %f", palmPosition.x, palmPosition.y, palmPosition.z);
		int size((int)strlen(sendbuf));
		int iResult(0);
		// Send an initial buffer
		iResult = send(ConnectSocket, sendbuf, size, 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "Send failed with error: " << WSAGetLastError() << std::endl;
			closesocket(ConnectSocket);
			WSACleanup();
		}
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
	}
}
void LeapCapture::Capture()
{
	palmPosition = Leap::Vector(0.f, 0.f, 0.f);
	const Leap::Frame frame = controller.frame();
	Leap::HandList hands = frame.hands();
	for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
	{
		// Get the first hand
		const Leap::Hand hand = *hl;
		// Get the hand's normal vector and direction
		normal = hand.palmNormal();
		direction = hand.direction();
		palmPosition = hand.palmPosition();
		std::cout << palmPosition << std::endl;
		//const Leap::FingerList fingers = hand.fingers();
		//const Leap::FingerList extended = fingers.extended();

		//std::vector<Leap::Finger::Type> types;
		//for (Leap::FingerList::const_iterator fl = extended.begin(); fl != extended.end(); ++fl)
		//{
		//	const Leap::Finger finger = *fl;
		//	types.push_back(finger.type());
		//	const Leap::Bone distal = finger.bone(Leap::Bone::TYPE_DISTAL);
		//	const Leap::Vector tip = distal.nextJoint();
		//}
		//for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
		//	const Leap::Finger finger = *fl;
		//	const Leap::Vector direction = finger.direction();
		//	const Leap::Bone distal = finger.bone(Leap::Bone::TYPE_DISTAL);
		//	const Leap::Vector tip = distal.nextJoint();
		//}
	}
}