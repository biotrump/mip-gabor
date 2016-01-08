#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <math.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
 
using namespace std;
using namespace cv;
#include "gabor.hpp"

//void Process(Mat src_f, int kernel_size);
void Process(int , void *);
cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps);

extern int kernel_size;
extern int pos_sigma;
extern int pos_lm;
extern int pos_th;
extern int pos_psi;
extern cv::Mat src_f;
extern cv::Mat dest;

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: %s <Image_Path>\n", argv[0]);
        return -1;
    }
    Mat image;
    image = imread( argv[1], CV_LOAD_IMAGE_COLOR );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    cv::namedWindow("Display Image", WINDOW_AUTOSIZE );
    cv::imshow("Display Image", image);

	// Create Windows : PC is MSB, so B,G,R,B,G,R.....
    cv::namedWindow("Blue",WINDOW_AUTOSIZE);	
    cv::namedWindow("Green",WINDOW_AUTOSIZE);
    cv::namedWindow("Red",WINDOW_AUTOSIZE);
	vector<Mat> rgb;//RGB
	// The actual splitting RGB.
    split(image, rgb);
	cv::imshow("Blue", rgb[0]);
	cv::imshow("Green", rgb[1]);
	cv::imshow("Red", rgb[2]);
	tracker_data tracker[3]={{"gBlue"},
							{"gGreen"},
							{"gRed"}};	//rgb
	for(int i = 0; i < 3; i++){
		rgb[i].convertTo(tracker[i].src_f, CV_32F, 1.0/255, 0);//green channel
	}
	
	//process(0, (void *)(tracker+1) );//green channel
	
	for(int i = 0; i < 3; i++){
		//int kernel_size=21;
		cv::namedWindow(tracker[i].winname, WINDOW_AUTOSIZE);
		//the size of 2-D gaussian envelop by standard deviation
		tracker[i].trackername="Sigma";
		cv::createTrackbar("Sigma", tracker[i].winname, &pos_sigma, kernel_size, Process, 
						   tracker+i);
		//wavelength of sinusoid, freq = 1/lambda
		tracker[i].trackername="Lambda";
		cv::createTrackbar("Lambda", tracker[i].winname, &pos_lm, 100, Process, 
						   tracker+i);
		//Theta: orientation of the sinusoid by angualr 0-180 ie.(0-PI)
		tracker[i].trackername="Theta";
		cv::createTrackbar("Theta", tracker[i].winname, &pos_th, 180, Process, 
						   tracker+i);
		//Psi : the phase of the sinusoid
		tracker[i].trackername="Psi";
		cv::createTrackbar("Psi", tracker[i].winname, &pos_psi, 360, Process, 
						   tracker+i);
	}

    waitKey(0);
    return 0;
}
