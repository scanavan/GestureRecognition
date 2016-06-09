#include "tinydir.h"
#include <iostream>
#include "LeapData.h"

int main()
{
	LeapData test("C:\\Users\\IASA-FRI\\Desktop\\SRI\\database gesture 6\\P1\\G6\\bad\\1_leap_motion.csv");

	test.writeToFile("C:\\Users\\IASA-FRI\\Desktop\\bad.txt");
	
    return EXIT_SUCCESS;
}