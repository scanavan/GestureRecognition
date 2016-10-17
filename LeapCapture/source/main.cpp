#include "LeapCapture.h"

int main()
{
	LeapCapture lc;
	while (1)
	{
		lc.Capture();
		lc.SendData();
	}
	return 0;
}