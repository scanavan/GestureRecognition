#include "LeapCapture.h"
#include <fstream>

int main()
{
	std::ofstream out;
	out.open("C:/Users/IASA-FRI/Documents/leapArffFiles/leapData.arff");
	out << "% 1. Title: Leap Database\n"
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

	LeapCapture lc(false);
	while (1)
	{
		lc.Capture();

		//ADD EXTENDED FINGERS TO ARFFWRITER FUNCTION AND TO @ATTRIBUTES

		for (char button = 65; button < 91; button++)
		{
			if (GetAsyncKeyState(button))
			{
				lc.writeArffFile(out, button);
				std::cout << button << " captured!" << std::endl;
			}
		}

		Sleep(500);
		lc.clearVectors();

		//lc.SendData();
	}
	out.close();
	return 0;


	//getASyncKeystroke Windows?
	//Bind keypress to call arffWriter


	//Calling functions first time, write header one time, everytime you press key, you append to file
}








