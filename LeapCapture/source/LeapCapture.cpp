#include "LeapCapture.h"
#include <iostream>
#include <fstream>








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



	handFound = false;
	for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
	{
		// Get the first hand
		const Leap::Hand hand = *hl;
		
		handFound = true;




		//Get a reference frame if there's only one hand in view, ASSUMING ONE HAND FOR NOW
		//if (hands.count() == 1)
		//{




		//ScaleFactor turns to 1 and stays at 1 after you take your hand away, WHY DOES THIS HAPPEN?
		if (ctr == 1)
		{
			referenceFrame = frame;
			ctr++;
		}
		//}


		// Get the hand's normal vector and direction
		normal = hand.palmNormal();
		direction = hand.direction();
		palmPosition = hand.palmPosition();
		pinchStrength = hand.pinchStrength();
		frontMostFinger = frame.fingers().frontmost();
		//std::cout << "Palm Position: " << palmPosition << std::endl;


		fingers = hand.fingers();
		extended = fingers.extended();






		int i = 0;
		for (Leap::FingerList::const_iterator fl = extended.begin(); fl != extended.end(); ++fl)
		{
			const Leap::Finger finger = *fl;
			types.push_back(finger.type());
			const Leap::Bone distal = finger.bone(Leap::Bone::TYPE_DISTAL);
			const Leap::Vector tip = distal.nextJoint();
		}
		//This code can be omitted if you do fingers.begin() -> fingers.end() instead of extended.begin() -> extended.end()
		//-1*10^8 only occurs when the finger isnt recognized as detected




		//if (fingerWidths[0] > 0 && fingerWidths[1] > 0 && fingerWidths[2] > 0 && fingerWidths[3] > 0 && fingerWidths[4] > 0)
		//{
		//std::cout << "FingerWidths: " << fingerWidths[0] << " " << fingerWidths[1] << " " << fingerWidths[2] << " " << fingerWidths[3] << " "
		//<< fingerWidths[4] << std::endl;
		//}










		for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Leap::Finger finger = *fl;
			//FingerDirections
			const Leap::Vector fingerDirection = finger.direction();
			fingerDirections.push_back(fingerDirection);

			//finger joints from top to bottom
			const Leap::Bone distal = finger.bone(Leap::Bone::TYPE_DISTAL);
			const Leap::Bone intermediate = finger.bone(Leap::Bone::TYPE_INTERMEDIATE);
			const Leap::Bone proximal = finger.bone(Leap::Bone::TYPE_PROXIMAL);
			const Leap::Bone metacarpal = finger.bone(Leap::Bone::TYPE_METACARPAL);
			averageFingerWidth = (distal.width() + intermediate.width() + proximal.width() + metacarpal.width()) / 4.0;
			//Did with Array instead of Vector out of curiosity
			fingerWidths[i] = averageFingerWidth;
			i++;
			//Don't think this is necessary
			//if (i < 5) { i++; }




			//fingertips
			const Leap::Vector tip = distal.nextJoint();
			fingertips.push_back(tip);
			//stable fingertips
			stableTipPositions.push_back(finger.stabilizedTipPosition());
			//frontMostFinger
			frontMostFinger = fingers.frontmost();
			//pinchStrength
			pinchStrength = hand.pinchStrength();









			//std::cout << "FingerWidths: " << fingerWidths[0] << " " << fingerWidths[1] << " " << fingerWidths[2] << " " << fingerWidths[3] << " "
			//<< fingerWidths[4] << std::endl;
		}




		for (int i = 0; i < fingerDirections.size(); i++)
		{
			//std::cout << "Finger Direction" << i + 1 << ": " << fingerDirections[i] << std::endl;
		}
		//fingerDirections.clear();
		//Can't print vector directly
		for (int i = 0; i < fingertips.size(); i++)
		{
			//std::cout << "Fingertip Position" << i+1 << ": " << fingertips[i] << std::endl;
		}
		//fingertips.clear();
		for (int i = 0; i < stableTipPositions.size(); i++)
		{
			//std::cout << "Stable Fingertip Position" << i+1 << ": " << stableTipPositions[i] << std::endl;
		}
		//stableTipPositions.clear();


		//grabAngle
		grabStrength = hand.grabStrength();
		//stable palm position
		stablePalmPosition = hand.stabilizedPalmPosition();
		//Frame scaleFactor
		scaleFactor = frame.scaleFactor(referenceFrame);


		//std::cout << "Pinch Strength: " << pinchStrength << std::endl;
		//std::cout << "Hand Grab Strength: " << grabStrength << std::endl;
		//std::cout << "Palm Position: " << palmPosition << std::endl;
		//std::cout << "Stable Palm Position: " << stablePalmPosition << std::endl;
		//std::cout << "Hand ScaleFactor: " << hand.scaleFactor(referenceFrame) << std::endl;
		//std::cout << "Frame ScaleFactor: " << frame.scaleFactor(referenceFrame) << std::endl;




		//Starts off at a decimal value but increases to 1 and stays at 1 forever?
		//std::cout << "Confidence: " << hand.confidence() << std::endl;




		//returns frontMost FingerId, last digit of ID: 0=thumb, 1=index, 2=middle, 3=ring, 4=pinky
		//std::cout << "FrontMost Finger: " << frontMostFinger << std::endl;



	}
	if (!handFound)
	{
		fingerDirections.clear();
		fingertips.clear();
		stableTipPositions.clear();
	}
}


void LeapCapture::writeArffFile(std::ofstream& out, char button)
{
	if (!handFound) { return; }

	for (int i = 0; i<fingerDirections.size(); i++)
	{
		out << fingerDirections[i].x << ", " << fingerDirections[i].y << ", " << fingerDirections[i].z << ", ";
	}


	for (int i = 0; i<fingertips.size(); i++)
	{
		out << fingertips[i].x << ", " << fingertips[i].y << ", " << fingertips[i].z << ", ";
	}


	for (int i = 0; i<stableTipPositions.size(); i++)
	{
		out << stableTipPositions[i].x << ", " << stableTipPositions[i].y << ", " << stableTipPositions[i].z << ", ";
	}


	out << pinchStrength << ", ";
	out << grabStrength << ", ";
	out << palmPosition.x << ", " << palmPosition.y << ", " << palmPosition.z << ", ";
	out << stablePalmPosition.x << ", " << stablePalmPosition.y << ", " << stablePalmPosition.z << ", ";
	out << scaleFactor << ", ";
	out << frontMostFinger.type() << ", ";
	out << button << "\n";
}

void LeapCapture::clearVectors()
{
	fingerDirections.clear();
	fingertips.clear();
	stableTipPositions.clear();
}