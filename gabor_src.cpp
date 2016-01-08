#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
 
using namespace std;
using namespace cv;
#include "cv_itk.hpp"

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: %s <Image_Path>\n", argv[0]);
        return -1;
    }
    cv::Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    vector<Mat> rgb;//RGB
	// The actual splitting RGB.
    split(image, rgb);
	//cv::Mat image_f;
	//rgb[1].convertTo(image_f, CV_32F, 1.0/255, 0);//from 8 bit [0-255] to float [0.0-1.0]
	
    cv::Mat kernelImage;
	cv::Mat resultImage;
	//cvitk_GaborImageSource2D(rgb[1] /*image_f*/, kernelImage, resultImage);
	cvitk_GaborImageFilter2D(rgb[1] , kernelImage, resultImage);

	cv::namedWindow("inputimage", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO |CV_GUI_EXPANDED );
	cv::imshow("inputimage", image);

	cv::Mat rscaled;
	resultImage.convertTo( rscaled, CV_8UC1, 255.0 );	
	cv::namedWindow("resultimage", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO |CV_GUI_EXPANDED );
	cv::imshow("resultimage", rscaled);
	
	cv::Mat kscaled;
	kernelImage.convertTo( kscaled, CV_8UC1, 255.0 );	
	cv::namedWindow("kscaled", CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO |CV_GUI_EXPANDED );
	cv::imshow("kscaled", kscaled);

    waitKey(0);
    return 0;
}
