#include <Windows.h>
#include "LeapCapture.h"
#include <fstream>
#include "../inc/RandomizedForest.h"
#include "../inc/GestureVector.h"

int main()
{
	//Need function to train random forest
	//function needs to take LeapCapture data and create a tree

	LeapCapture lc;
	lc.WriteArffFileHeader("test.arff");//"C:/Users/IASA-FRI/Documents/leapArffFiles/leapData.arff"
	while (1)
	{
		bool captured = lc.Capture();
		//ADD EXTENDED FINGERS TO ARFFWRITER FUNCTION AND TO @ATTRIBUTES
		for (char button = 65; button < 91; button++)
		{
			if (GetAsyncKeyState(button) && captured)
			{
				//to do real-time, we need to call the random forest here.
				//need to write a function that takes LeapCapture data and tests on forest
				lc.writeArffFile(button);
				std::cout << button << " captured!" << std::endl;
			}
		}
		//can this be a shorter sleep and still get the right data?
		Sleep(200);
		lc.clearVectors();
	}
	return 0;


	//getASyncKeystroke Windows?
	//Bind keypress to call arffWriter


	//Calling functions first time, write header one time, everytime you press key, you append to file
}








