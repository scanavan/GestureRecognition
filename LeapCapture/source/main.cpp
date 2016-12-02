#include <Windows.h>
#include "LeapCapture.h"
#include <fstream>
#include "../inc/RandomizedForest.h"
#include "../inc/GestureVector.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void Capture(std::string mode);
void Test(std::string treeFile);
void Train();
std::vector<GestureVector> parffArse(std::string path);
void trainForest(std::vector<GestureVector> gesture, RandomizedForest forest, std::string filename);
void DisplayLetter(int letter);

std::string title = "Sign Language Letters";
void DisplayLetter(int letter)
{
	std::string image;
	if (letter < 10)
	{
		image = "0" + std::to_string(letter) + ".jpeg";
	}
	else
	{
		image = std::to_string(letter) + ".jpeg";
	}
	cv::Mat letterImage = cv::imread(image);
	cv::imshow(title, letterImage);
	cv::waitKey(33);
}
void trainForest(std::vector<GestureVector> gesture, RandomizedForest forest, std::string filename)
{
	//std::cout << "Training...\n" << std::flush;

	for (int i = 0; i < gesture.size(); ++i) {
		forest.train(gesture[i], gesture[i].getLabel());
	}

	forest.save(filename);
}
void Capture(std::string mode)
{
	int ctr = 0;
	LeapCapture lc;
	if (mode == "write")
	{
		lc.WriteArffFileHeader("FRIDemoSWTAR.arff");//"C:/Users/IASA-FRI/Documents/leapArffFiles/leapData.arff"
	}
	else if (mode == "append")
	{
		//outArffFile.open("test.arff", std::fstream::in | std::fstream::out | std::fstream::app);
		lc.AppendArffFile("FRIDemoSWTAR.arff");
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
					std::cout << button << " captured!  " << (ctr%10+1) << std::endl;
					ctr++;

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
		while (line != "@DATA") {
			if (ifs.eof()) { break; }
			std::getline(ifs, line);
		}
		std::getline(ifs, line);
		while (true) {
			std::getline(ifs, line);
			if (ifs.eof()) { 
				break; 
			}
			std::istringstream ss(line);
			std::string token;
			std::string::size_type sz;
			std::vector<float> val;
			unsigned int lab;
			while (std::getline(ss, token, ',')) {
				if (token[1] == 'G') {
					lab = std::stoi(token.substr(2, 2));
					if (lab >= 10) { lab--; }
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
void Test(std::string treeFile)
{
	LeapCapture lc;
	std::cout << "Loading Random Forest...";
	RandomizedForest forest;
	forest.load(treeFile + ".rf");
	std::cout << "DONE." << std::endl;
	std::vector<float> data;
	cv::namedWindow(title);
	while (1)
	{
		bool found = lc.Capture();
		int classify(0);
		if (found)
		{
			lc.GetGestureVector(data);
			GestureVector gesture(data, 1);
			classify = forest.classify(gesture);
			//std::cout << classify << std::endl;
			data.clear();
		}
		DisplayLetter(classify);
		lc.clearVectors();
	}
}
void Train()
{
	//NOTE: This function should probably take a string that is the arrf file to read.
	unsigned int nb_labels = 5;
	unsigned vector_size;
	double minV = -2. * PI;
	//double maxV = 2. * PI;
	double maxV = 100.;
	unsigned int depth = 37;
	//unsigned int nb_trees = 15;

	std::vector<GestureVector> gesture = parffArse("FRIDemoSWTAR.arff");
	if (!gesture.empty())
	{
		vector_size = static_cast<unsigned>(gesture[0].getFeatures().size());
		//for (int trees_i = 4; trees_i <= 30; ++trees_i) 
		{
			RandomizedForest forest(nb_labels, false, depth, 15, vector_size, minV, maxV);
			trainForest(gesture, forest, "testTree");
		}
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
		Test("testTree");
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






