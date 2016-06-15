#include <opencv2/opencv.hpp>
#include <iostream>

int GetAverageDepth(cv::Mat image)
{
    int avg(0);
    //get average depth value. It's a good place to start for thresholding per image as this will change
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            avg += static_cast<int>(image.at<uchar>(i, j));
        }
    }
    return(avg / (image.rows* image.cols));
}
void ThresholdImage(cv::Mat& image, int avg)
{
    //step is used to change threshold from avg
    int step(40);
    //threshold image
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            //remove background and some edge data that we know isn't the hand
            if (image.at<uchar>(i, j) >(avg - step) || (i >(image.rows*.75)))
            {
                image.at<uchar>(i, j) = 0;
            }
        }
    }
}
void GetBoundingBox(cv::Mat image, cv::Point& minHand, cv::Point& maxHand)
{
    //only care about 'y' coordinate
    int topOfHand(0);
    //check to see if we found the hand
    bool found(false);
    //current min/max values
    int min(image.cols);
    int max(0);
    //get top of hand
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            //non-background
            if (image.at<uchar>(i, j) != 0)
            {
                //we found the top of the hand
                if (!found)
                {
                    topOfHand = i;
                    found = true;
                }

                //set min and max x range
                if (j < min)
                {
                    min = j;
                }
                if (j > max)
                {
                    max = j;
                }
            }
        }
    }
    minHand.x = min - 10;
    minHand.y = topOfHand;
    //add a little bit to bounding box so we don't miss anything on the edge (may not even need this extra space)
    maxHand.x = max + 10;
    //115 is based on hand size. This could possibly use some more automation
    maxHand.y = topOfHand + 115;
}
void ColorHandRegion(cv::Mat image, cv::Mat& orig, cv::Point minHand, cv::Point maxHand)
{
    //loop through bounding box, if the depth isn't a background pixel, set it to white
    for (int i = minHand.y; i < maxHand.y; ++i)
    {
        for (int j = minHand.x; j < maxHand.x; ++j)
        {
            cv::Scalar intensity = image.at<uchar>(i, j);
            if (intensity.val[0] != 0)
            {
                orig.at<uchar>(i, j) = 255;
            }
        }
    }
}
int main()
{
    //load image
    cv::Mat image = cv::imread("C:\\Users\\Shaun\\Google Drive\\IASA-FRI\\Summer2016\\kinect_leap_dataset\\acquisitions\\P14\\G1\\1_depth.png", CV_LOAD_IMAGE_UNCHANGED);
    //clone image to display original depth values with hand
    cv::Mat orig = image.clone();
    
    //get average depth
    int avg = GetAverageDepth(image);

    //threshold image and remove background
    ThresholdImage(image, avg);

    //get bounding box around hand
    cv::Point minHand;
    cv::Point maxHand;
    GetBoundingBox(image, minHand, maxHand);

    //color hand region white
    ColorHandRegion(image, orig, minHand, maxHand);

    //create window
    cv::namedWindow("Test");
    for (;;)
    {
        //display original with new hand position
        cv::imshow("Test", orig);
        //exit with 'a' key
        if (cv::waitKey(33) == 'a')
        {
            break;
        }
    }
    return EXIT_SUCCESS;
}