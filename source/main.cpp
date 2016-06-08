#include "tinydir.h"
#include <iostream>
#include "LeapData.h"

int main()
{
	LeapData test("C:\\Users\\IASA-FRI\\Desktop\\SRI\\database gesture 6\\P1\\G6\\2_leap_motion.csv");
	test.printAttributes();

	/*std::vector<Point> p = test.getProjection();

	for (int i = 0; i < p.size(); i++) {
		p[i].printPoint();
	}*/
    return EXIT_SUCCESS;
}