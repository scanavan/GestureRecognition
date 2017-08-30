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
void LeapCapture::GetGestureVector(std::vector<float>& data)
{
	for (int i = 0; i<fingerDirections.size(); i++)
	{
		data.push_back(fingerDirections[i].x);
		data.push_back(fingerDirections[i].y);
		data.push_back(fingerDirections[i].z);
	}
	for (int i = 0; i<extendedFingers.size(); i++)
	{
		data.push_back(static_cast<float>(extendedFingers[i]));
	}
	data.push_back(pinchStrength);
	data.push_back(grabStrength);
	data.push_back(scaleFactor);
	for (int i = 0; i<fingertips.size(); i++)
	{
		data.push_back(fingertips[i].x - palmPosition.x);
		data.push_back(fingertips[i].y - palmPosition.y);
		data.push_back(fingertips[i].z - palmPosition.z);
	}
}
void LeapCapture::AppendArffFile(std::string outName)
{
	outArffFile.open(outName, std::fstream::app);
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
		/*<< "@ATTRIBUTE fingertip1x NUMERIC\n"
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
		<< "@ATTRIBUTE stableFingertip5z NUMERIC\n"*/
		<< "@ATTRIBUTE extendedFinger1 NUMERIC\n"
		<< "@ATTRIBUTE extendedFinger2 NUMERIC\n"
		<< "@ATTRIBUTE extendedFinger3 NUMERIC\n"
		<< "@ATTRIBUTE extendedFinger4 NUMERIC\n"
		<< "@ATTRIBUTE extendedFinger5 NUMERIC\n"
		<< "@ATTRIBUTE pinchStrength NUMERIC\n"
		<< "@ATTRIBUTE grabStrength NUMERIC\n"
		/*<< "@ATTRIBUTE palmPositionX NUMERIC\n"
		<< "@ATTRIBUTE palmPositionY NUMERIC\n"
		<< "@ATTRIBUTE palmPositionZ NUMERIC\n"*/
		/*<< "@ATTRIBUTE stablePalmPositionX NUMERIC\n"
		<< "@ATTRIBUTE stablePalmPositionY NUMERIC\n"
		<< "@ATTRIBUTE stablePalmPositionZ NUMERIC\n"*/
		<< "@ATTRIBUTE scaleFactor NUMERIC\n"
		//<< "@ATTRIBUTE frontMostFinger NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceX1 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceY1 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceZ1 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceX2 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceY2 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceZ2 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceX3 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceY3 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceZ3 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceX4 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceY4 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceZ4 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceX5 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceY5 NUMERIC\n"
		<< "@ATTRIBUTE fingertipDistanceZ5 NUMERIC\n"
		<< "@ATTRIBUTE class {G01,G02,G03,G04,G05,G06,G07,G08,G09,G11,G12,G13,G14,G15,G16,G17,G18,G19,G20,G21,G22,G23,G24,G25}\n"
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

			//fingertips
			const Leap::Vector tip = distal.nextJoint();
			fingertips.push_back(tip);
			//stable fingertips
			stableTipPositions.push_back(finger.stabilizedTipPosition());
			//frontMostFinger
			frontMostFinger = fingers.frontmost();
			//pinchStrength
			pinchStrength = hand.pinchStrength();
			//extended fingers
			if (finger.isExtended())
			{
				extendedFingers.push_back(1);
			}
			else
			{
				extendedFingers.push_back(0);
			}
		}

		//grabAngle
		grabStrength = hand.grabStrength();
		//stable palm position
		stablePalmPosition = hand.stabilizedPalmPosition();
		//Frame scaleFactor
		scaleFactor = frame.scaleFactor(referenceFrame);
		
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
	/*for (int i = 0; i<fingertips.size(); i++)
	{
		outArffFile << fingertips[i].x << ", " << fingertips[i].y << ", " << fingertips[i].z << ", ";
	}
	for (int i = 0; i<stableTipPositions.size(); i++)
	{
		outArffFile << stableTipPositions[i].x << ", " << stableTipPositions[i].y << ", " << stableTipPositions[i].z << ", ";
	}*/
	for (int i = 0; i<extendedFingers.size(); i++)
	{
		outArffFile << extendedFingers[i] << ", ";
	}

	outArffFile << pinchStrength << ", ";
	outArffFile << grabStrength << ", ";
	//outArffFile << palmPosition.x << ", " << palmPosition.y << ", " << palmPosition.z << ", ";
	//outArffFile << stablePalmPosition.x << ", " << stablePalmPosition.y << ", " << stablePalmPosition.z << ", ";
	outArffFile << scaleFactor << ", ";
	//outArffFile << frontMostFinger.type() << ", ";
	for (int i = 0; i<fingertips.size(); i++)
	{
		outArffFile << fingertips[i].x - palmPosition.x << ", " << fingertips[i].y - palmPosition.y << ", " << fingertips[i].z - palmPosition.z << ", ";
	}
	int label = button - 65;
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
	extendedFingers.clear();
}