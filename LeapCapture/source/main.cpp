#include "LeapCapture.h"


int main()
{
	LeapCapture lc(false);
	while (1)
	{
		lc.Capture();
		//lc.SendData();
	}
	return 0;

	//getASyncKeystroke Windows?
	//Bind keypress to call arffWriter

	//Calling functions first time, write header one time, everytime you press key, you append to file
}


