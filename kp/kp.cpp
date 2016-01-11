#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
    if ( argc < 2 )
    {
        printf("usage: %s <Image_Path> rgb\n", argv[0]);
        return -1;
    }
    int is_rgb=0;
    if(argc >= 3){
		is_rgb=1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);

	vector<Mat> rgb;//RGB
	if(is_rgb){
		// Create Windows : PC is MSB, so B,G,R,B,G,R.....
		cv::namedWindow("Blue",WINDOW_AUTOSIZE);
		cv::namedWindow("Green",WINDOW_AUTOSIZE);
		cv::namedWindow("Red",WINDOW_AUTOSIZE);

		// The actual splitting RGB.
		split(image, rgb);
		cv::imshow("Blue", rgb[0]);
		cv::imshow("Green", rgb[1]);
		cv::imshow("Red", rgb[2]);
	}
	//-- Step 1: Detect the keypoints using SURF Detector
    /* threshold      = 0.04;
       edge_threshold = 10.0;
       magnification  = 3.0;    */

    // SIFT feature detector and feature extractor
    cv::SiftFeatureDetector detector( 0.05, 5.0 );
    cv::SiftDescriptorExtractor extractor( 3.0 );

    /* In case of SURF, you apply the below two lines
	int minHessian = 400;
    cv::SurfFeatureDetector detector(minHessian);
    cv::SurfDescriptorExtractor extractor();
    */
	if(is_rgb){
		std::vector<KeyPoint> keypoints[3];

		for(int i = 0; i < 3; i++){
			detector.detect( rgb[i], keypoints[i] );
		}
		//-- Draw keypoints
		Mat img_keypoints[3];

		for(int i = 0; i < 3; i++){
			drawKeypoints( rgb[i], keypoints[i], img_keypoints[i], Scalar::all(-1),
						DrawMatchesFlags::DRAW_RICH_KEYPOINTS /*DEFAULT*/ );
		}
		//-- Show detected (drawn) keypoints
		imshow("Keypoints B", img_keypoints[0] );
		imshow("Keypoints G", img_keypoints[1] );
		imshow("Keypoints R", img_keypoints[2] );
	}else{
		std::vector<KeyPoint> keypoints;

		detector.detect( image, keypoints );
		//-- Draw keypoints
		Mat img_keypoints;
		drawKeypoints( image, keypoints, img_keypoints, Scalar::all(-1),
						DrawMatchesFlags::DRAW_RICH_KEYPOINTS /*DEFAULT*/ );
		//-- Show detected (drawn) keypoints
		string str="Keypoints";
		str+= argv[1];
		imshow(str, img_keypoints );
	}
#if 0
    // Feature descriptor computation
    Mat descriptor1,descriptor2;
    extractor.compute( tmp, keypoints1, descriptor1 );
    extractor.compute( in, keypoints2, descriptor2 );
    /*int desc1 = descriptor1.size;
    int desc2 = descriptor2.size;
    printf("Descriptor1=%d \nDescriptor2=%d", desc1, desc2);*/

    // corresponded points
    std::vector matches;

    // L2 distance based matching. Brute Force Matching
    BruteForceMatcher<L2 > matcher;

    // Flann-based matching
    //FlannBasedMatcher matcher;

    // display of corresponding points
    matcher.match( descriptor1, descriptor2, matches );

    // matching result
    Mat result;
    drawMatches( tmp, keypoints1, in, keypoints2, matches, result );

    // output file
    imwrite( "result.bmp", result );

    // display the result
    namedWindow("SIFT", CV_WINDOW_AUTOSIZE );
    imshow("SIFT", result);
#endif

	waitKey(0);
    return 0;
}
