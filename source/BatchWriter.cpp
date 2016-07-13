#include "BatchWriter.h"
#include <iostream>
#include <fstream>

void arffBatchWriter()
{
	std::ofstream file;
	file.open("C:/Users/IASA-FRI/Desktop/arff.bat");

	for (int i = 1; i < (1 << 12); ++i)
	{
		file << "java weka.filters.unsupervised.attribute.Remove -V -R ";

		if (i & (1 << 0)) file << "1,";
		if (i & (1 << 1)) file << "2,";
		if (i & (1 << 2)) file << "3,";
		if (i & (1 << 3)) file << "4,";
		if (i & (1 << 4)) file << "5,";
		if (i & (1 << 5)) file << "6,";
		if (i & (1 << 6)) file << "7-11,";
		if (i & (1 << 7)) file << "12-43,";
		if (i & (1 << 8)) file << "44-193,";
		if (i & (1 << 9)) file << "194-199,";
		if (i & (1 << 10)) file << "200-263,";
		if (i & (1 << 11)) file << "264-327,";
		
		file << "328 -i C:/Users/IASA-FRI/Desktop/A-B-C-D-E-F-G-Z-Y-X-W-V.arff -o C:/Users/IASA-FRI/Desktop/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "Z";
		if (i & (1 << 8)) file << "Y";
		if (i & (1 << 9)) file << "X";
		if (i & (1 << 10)) file << "W";
		if (i & (1 << 11)) file << "V";

		file << ".arff\n";

	}

}

void RFWriter()
{
	std::ofstream file;
	file.open("C:/Users/IASA-FRI/Desktop/RF.bat");

	for (int i = 1; i < (1 << 12); ++i)
	{
		file << "java weka.classifiers.trees.RandomForest -t ";

		file << "C:/Users/IASA-FRI/Desktop/ARFF/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "Z";
		if (i & (1 << 8)) file << "Y";
		if (i & (1 << 9)) file << "X";
		if (i & (1 << 10)) file << "W";
		if (i & (1 << 11)) file << "V";

		file << ".arff > C:/Users/IASA-FRI/Desktop/RF/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "Z";
		if (i & (1 << 8)) file << "Y";
		if (i & (1 << 9)) file << "X";
		if (i & (1 << 10)) file << "W";
		if (i & (1 << 11)) file << "V";

		file << ".txt\n";
	}

}

void arffLeapBatchWriter() {
	std::ofstream file;
	file.open("C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/BatchFiles/createARFF.bat");

	for (int i = 1; i < (1 << 9); ++i)
	{
		file << "java weka.filters.unsupervised.attribute.Remove -V -R ";

		if (i & (1 << 0)) file << "1,";
		if (i & (1 << 1)) file << "2,";
		if (i & (1 << 2)) file << "3,";
		if (i & (1 << 3)) file << "4,";
		if (i & (1 << 4)) file << "5,";
		if (i & (1 << 5)) file << "6,";
		if (i & (1 << 6)) file << "7-11,";
		if (i & (1 << 7)) file << "12-16,";
		if (i & (1 << 8)) file << "17-21,";

		file << "22 -i C:/Users/IASA-FRI/Desktop/SRI/test.arff -o C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/arffFiles/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".arff\n";

	}
}

void LeapRFWriter()
{
	std::ofstream file;
	file.open("C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/BatchFiles/RandomForest.bat");

	for (int i = 1; i < (1 << 12); ++i)
	{
		file << "java weka.classifiers.trees.RandomForest -t ";

		file << "C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/arffFiles/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".arff > C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/RandomForest/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".txt\n";
	}

}

void LeapRTWriter() {
	std::ofstream file;
	file.open("C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/BatchFiles/REPTree.bat");

	for (int i = 1; i < (1 << 12); ++i)
	{
		file << "java weka.classifiers.trees.REPTree -t ";

		file << "C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/arffFiles/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".arff > C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/REPTree/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".txt\n";
	}
}

void LeapLibSVMWriter() {
	std::ofstream file;
	file.open("C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/BatchFiles/LibSVM.bat");

	for (int i = 1; i < (1 << 12); ++i)
	{
		file << "java weka.classifiers.functions.LibSVM -t ";

		file << "C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/arffFiles/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".arff > C:/Users/IASA-FRI/Desktop/SRI/GestureRecognition/Data/AllLeapFeatures/LibSVM/Leap/";

		if (i & (1 << 0)) file << "A";
		if (i & (1 << 1)) file << "B";
		if (i & (1 << 2)) file << "C";
		if (i & (1 << 3)) file << "D";
		if (i & (1 << 4)) file << "E";
		if (i & (1 << 5)) file << "F";
		if (i & (1 << 6)) file << "G";
		if (i & (1 << 7)) file << "H";
		if (i & (1 << 8)) file << "I";

		file << ".txt\n";
	}
}