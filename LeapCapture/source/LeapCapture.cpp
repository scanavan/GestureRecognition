#include "LeapCapture.h"
#include <iostream>

LeapCapture::LeapCapture() :
	palmPosition(0.f, 0.f, 0.f)
{
}
LeapCapture::~LeapCapture()
{
	outArffFile.close();
}
void LeapCapture::WriteArffFileHeader(std::string outName)
{
	outArffFile.open(outName);
	outArffFile << "% 1. Title: Leap Database\n"
		<< "%\n"
		<< "% 2. Sources :\n"
		<< "% (a)Creator : NIH and Binghamton FRI 2016\n"
		<< "% (b)Date : Fall 2016\n"
		<< "%\n"
		<< "@RELATION asd\n"
		<< "\n"
		<< "@ATTRIBUTE fingerDirection1x NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection1y NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection1z NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection2x NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection2y NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection2z NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection3x NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection3y NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection3z NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection4x NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection4y NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection4z NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection5x NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection5y NUMERIC\n"
		<< "@ATTRIBUTE fingerDirection5z NUMERIC\n"
		<< "@ATTRIBUTE fingertip1x NUMERIC\n"
		<< "@ATTRIBUTE fingertip1y NUMERIC\n"
		<< "@ATTRIBUTE fingertip1z NUMERIC\n"
		<< "@ATTRIBUTE fingertip2x NUMERIC\n"
		<< "@ATTRIBUTE fingertip2y NUMERIC\n"
		<< "@ATTRIBUTE fingertip2z NUMERIC\n"
		<< "@ATTRIBUTE fingertip3x NUMERIC\n"
		<< "@ATTRIBUTE fingertip3y NUMERIC\n"
		<< "@ATTRIBUTE fingertip3z NUMERIC\n"
		<< "@ATTRIBUTE fingertip4x NUMERIC\n"
		<< "@ATTRIBUTE fingertip4y NUMERIC\n"
		<< "@ATTRIBUTE fingertip4z NUMERIC\n"
		<< "@ATTRIBUTE fingertip5x NUMERIC\n"
		<< "@ATTRIBUTE fingertip5y NUMERIC\n"
		<< "@ATTRIBUTE fingertip5z NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip1x NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip1y NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip1z NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip2x NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip2y NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip2z NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip3x NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip3y NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip3z NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip4x NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip4y NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip4z NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip5x NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip5y NUMERIC\n"
		<< "@ATTRIBUTE stableFingertip5z NUMERIC\n"
		<< "@ATTRIBUTE pinchStrength NUMERIC\n"
		<< "@ATTRIBUTE grabStrength NUMERIC\n"
		<< "@ATTRIBUTE palmPositionX NUMERIC\n"
		<< "@ATTRIBUTE palmPositionY NUMERIC\n"
		<< "@ATTRIBUTE palmPositionZ NUMERIC\n"
		<< "@ATTRIBUTE stablePalmPositionX NUMERIC\n"
		<< "@ATTRIBUTE stablePalmPositionY NUMERIC\n"
		<< "@ATTRIBUTE stablePalmPositionZ NUMERIC\n"
		<< "@ATTRIBUTE scaleFactor NUMERIC\n"
		<< "@ATTRIBUTE frontMostFinger NUMERIC\n"
		<< "@ATTRIBUTE class {A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,Q,R,S,T,U,V,X,Y}\n"
		<< "\n@DATA\n";
}
bool LeapCapture::Capture()
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

		//ScaleFactor turns to 1 and stays at 1 after you take your hand away, WHY DOES THIS HAPPEN?
		if (ctr == 1)
		{
			referenceFrame = frame;
			ctr++;
		}

		// Get the hand's normal vector and direction
		normal = hand.palmNormal();
		direction = hand.direction();
		palmPosition = hand.palmPosition();
		pinchStrength = hand.pinchStrength();
		frontMostFinger = frame.fingers().frontmost();

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
			averageFingerWidth = (distal.width() + intermediate.width() + proximal.width() + metacarpal.width()) / 4.f;
			//Did with Array instead of Vector out of curiosity
			fingerWidths[i] = averageFingerWidth;
			i++;

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

		//for (int i = 0; i < fingerDirections.size(); i++)
		//{
			//std::cout << "Finger Direction" << i + 1 << ": " << fingerDirections[i] << std::endl;
		//}
		//fingerDirections.clear();
		//Can't print vector directly
		//for (int i = 0; i < fingertips.size(); i++)
		//{
		//	std::cout << "Fingertip Position" << i+1 << ": " << fingertips[i] << std::endl;
		//}
		//fingertips.clear();
		//for (int i = 0; i < stableTipPositions.size(); i++)
		//{
			//std::cout << "Stable Fingertip Position" << i+1 << ": " << stableTipPositions[i] << std::endl;
		//}
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
	return handFound;
}
void LeapCapture::writeArffFile(char button)
{
	for (int i = 0; i<fingerDirections.size(); i++)
	{
		outArffFile << fingerDirections[i].x << ", " << fingerDirections[i].y << ", " << fingerDirections[i].z << ", ";
	}


	for (int i = 0; i<fingertips.size(); i++)
	{
		outArffFile << fingertips[i].x << ", " << fingertips[i].y << ", " << fingertips[i].z << ", ";
	}


	for (int i = 0; i<stableTipPositions.size(); i++)
	{
		outArffFile << stableTipPositions[i].x << ", " << stableTipPositions[i].y << ", " << stableTipPositions[i].z << ", ";
	}
	outArffFile << pinchStrength << ", ";
	outArffFile << grabStrength << ", ";
	outArffFile << palmPosition.x << ", " << palmPosition.y << ", " << palmPosition.z << ", ";
	outArffFile << stablePalmPosition.x << ", " << stablePalmPosition.y << ", " << stablePalmPosition.z << ", ";
	outArffFile << scaleFactor << ", ";
	outArffFile << frontMostFinger.type() << ", ";
	int label = button - 64;
	if (label < 10)
	{
		outArffFile << "G0";
	}
	else
	{
		outArffFile << "G";
	}
	outArffFile << label << "\n";
}

void LeapCapture::clearVectors()
{
	fingerDirections.clear();
	fingertips.clear();
	stableTipPositions.clear();
}