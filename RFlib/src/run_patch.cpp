#include <iostream>
#include "../inc/RandomizedForest.h"
//#include "../inc/Color.h"
//#include "../inc/Patch.h"
#include "../inc/GestureVector.h"

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
				} else {
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

int main(int argc, char** argv)
{
	unsigned int nb_labels = 24;
	unsigned int depth = 30;
	unsigned int nb_trees = 32;
	unsigned int vector_size;
	double minV = -2. * PI;
	double maxV = 11111.;

	
	std::cout << "Training...\n" << std::flush;

	std::vector<GestureVector> gesture = parffArse("C:/ASL_ARFF/ACEFGHX.arff");
	vector_size = gesture[0].getFeatures().size();

	RandomizedForest forest(nb_labels, true, depth, nb_trees, vector_size, minV, maxV);

	//std::cout << gesture[0].getLabel() << std::endl;

	for (int i = 0; i < gesture.size(); ++i) {
		forest.train(gesture[i], gesture[i].getLabel());
	}

	forest.save("ACEFGHX");

	std::cout << "Classifying..." << std::flush;


	return 0;


	/*
	const char* imagename = argv[1];
	const char* labelname = argv[2];
	
	IplImage* image = cvLoadImage(imagename);
	IplImage* label = cvLoadImage(labelname);

	unsigned int patch_size = 13;
	unsigned int vector_size = 13*13*3;
	unsigned int border_size = patch_size/2;
	unsigned int depth = 12;
	unsigned int nb_trees = 10;
	unsigned int nb_labels = 9;
	double minV = 0.;
	double maxV = 255.;

	// the forest
	RandomizedForest forest(nb_labels,true,depth,nb_trees,vector_size,minV,maxV);

	// Training
	std::cout<<"Training..."<<std::flush;
	IplImage* ground_truth = cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,3);
	for(unsigned int x=border_size; x<image->width-border_size ; x++)
	{
		for(unsigned int y=border_size; y<image->height-border_size; y++)
		{
			// train
			Patch p(image,x,y,patch_size);
			unsigned int lab = static_cast<unsigned int>(cvGet2D(label,y,x).val[0]+0.5);
			forest.train(p,lab);

			// save ground truth with colors
			if(lab == nb_labels-1) cvSet2D(ground_truth,y,x,cvScalar(0));
			else
			{
				double u = lab/static_cast<double>(nb_labels-2);
				Color c(u);
				cvSet2D(ground_truth,y,x,CV_RGB(c.getR(),c.getG(),c.getB()));
			}
		}	
	}
	cvSaveImage("ground_truth.png",ground_truth);
	forest.save("amazon");
	std::cout<<"OK!"<<std::endl;

	// Test the same data that was just learnt
	std::cout<<"Testing..."<<std::flush;
	RandomizedForest forest2;
	forest2.load("amazon.rf");
	forest2.setOccurenceNormalization(true);
	IplImage* test = cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U,3);
	for(unsigned int x=border_size; x<image->width-border_size ; x++)
	{
		for(unsigned int y=border_size; y<image->height-border_size; y++)
		{
			// hard classification
			Patch p(image,x,y,patch_size);	

			// save it with colors
			unsigned int lab = forest2.classify(p);
			if(lab == nb_labels-1) cvSet2D(test,y,x,cvScalar(0));
			else
			{
				double u = lab/static_cast<double>(nb_labels-2);
				Color c(u);
				cvSet2D(test,y,x,CV_RGB(c.getR(),c.getG(),c.getB()));
			}
		}	
	}
	cvSaveImage("classification.png",test);
	std::cout<<"OK!"<<std::endl;

	// free memory
	cvReleaseImage(&ground_truth);
	cvReleaseImage(&test);
	cvReleaseImage(&image);
	cvReleaseImage(&label);
	*/

}
