#include <Windows.h>
#include "LeapCapture.h"
#include <fstream>
#include "../inc/RandomizedForest.h"
#include "../inc/GestureVector.h"
void Capture(std::string mode)
{
	LeapCapture lc;
	if (mode == "write")
	{
		lc.WriteArffFileHeader("test.arff");//"C:/Users/IASA-FRI/Documents/leapArffFiles/leapData.arff"
	}
	else if (mode == "append")
	{
		//outArffFile.open("test.arff", std::fstream::in | std::fstream::out | std::fstream::app);
		lc.AppendArffFile("test.arff");
	}
	while (1)
	{
		bool captured = lc.Capture();
		//ADD EXTENDED FINGERS TO ARFFWRITER FUNCTION AND TO @ATTRIBUTES
		for (char button = 65; button < 91; button++)
		{
			if (GetAsyncKeyState(button) && captured)
			{
				if (button == 'J' || button == 'Z')
				{
					std::cout << button << " is not a valid gesture right now!" << std::endl;
				}
				//to do real-time, we need to call the random forest here.
				//need to write a function that takes LeapCapture data and tests on forest
				else
				{
					lc.writeArffFile(button);
					std::cout << button << " captured!" << std::endl;
				}
			}
		}
		//can this be a shorter sleep and still get the right data?
		Sleep(200);
		lc.clearVectors();
	}
}
std::vector<GestureVector> parffArse(std::string path)
{
	std::vector<GestureVector> data;
	std::ifstream ifs;
	std::string line;
	ifs.open(path, std::ifstream::in);

	if (ifs.is_open()) {
		std::getline(ifs, line);
		while (line != "@data") {
			if (ifs.eof()) { break; }
			std::getline(ifs, line);
		}
		std::getline(ifs, line);
		while (true) {
			std::getline(ifs, line);
			if (ifs.eof()) { break; }
			std::istringstream ss(line);
			std::string token;
			std::string::size_type sz;
			std::vector<float> val;
			unsigned int lab;
			while (std::getline(ss, token, ',')) {
				if (token[0] == 'G') {
					lab = std::stoi(token.substr(1, 2));
				}
				else {
					val.push_back(std::stof(token, &sz));
				}
			}
			data.push_back(GestureVector(val, lab));
			val.clear();
			lab = 0;
		}
		ifs.close();
	}
	return data;
}
void Test()
{
	unsigned int nb_labels = 24;
	size_t vector_size;
	double minV = -2. * PI;
	//double maxV = 2. * PI;
	double maxV = 100.;
	unsigned int depth = 150;
	//unsigned int nb_trees = 15;
}
void Train()
{
	unsigned int nb_labels = 24;
	size_t vector_size;
	double minV = -2. * PI;
	//double maxV = 2. * PI;
	double maxV = 100.;
	unsigned int depth = 150;
	//unsigned int nb_trees = 15;

	std::vector<GestureVector> gesture = parffArse("C:/LeapAngles/arff/CGHI.arff");
	vector_size = gesture[0].getFeatures().size();


	for (int trees_i = 4; trees_i <= 30; ++trees_i) {
		RandomizedForest forest(nb_labels, true, depth, trees_i, vector_size, minV, maxV);

//		trainForest(gesture, forest, "C:/RFlib_Test/CGHI");
		std::cout << trees_i << std::endl;
//		testForest(gesture, "C:/RFlib_Test/CGHI");
		/*std::vector<GestureVector>::const_iterator first = gesture.begin() + gesture.size() - 240;
		std::vector<GestureVector>::const_iterator last = gesture.end();
		std::vector<GestureVector> test(first, last);*/

		//forest.~RandomizedForest();
	}
}
int main(int argc, char* argv[])
{
	//Need function to train random forest
	//function needs to take LeapCapture data and create a tree
	std::string mode(argv[1]);
	if (mode.compare("train") == 0)
	{
		Train();
	}
	else if (mode.compare("test") == 0)
	{
		Test();
	}
	else if (mode.compare("capture new") == 0)
	{
		Capture("write");
	}
	else if (mode.compare("capture append") == 0)
	{
		Capture("append");
	}
	return 0;
}






