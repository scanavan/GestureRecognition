#include <iostream>
#include "../inc/RandomizedForest.h"
#include "../inc/Color.h"
#include "../inc/Patch.h"

int main(int argc, char** argv)
{
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

}
