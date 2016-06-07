#include "tinydir.h"
#include <iostream>
#include "LeapData.h"

int main()
{
	LeapData test("C:\\Users\\IASA-FRI\\Desktop\\SRI\\database gesture 6\\P1\\G6\\2_leap_motion.csv");
	test.printAttributes();
    return EXIT_SUCCESS;
}