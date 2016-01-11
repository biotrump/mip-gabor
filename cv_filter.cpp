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
#include "cv_itk.hpp"

#if 0
/* Greylevel input image
 * http://stackoverflow.com/questions/30379359/how-can-i-make-my-anisotropic-diffusion-function-faster
 * 
 */
void anisotropicDiffusion(const cv::Mat& res,  int num_iter, double delta_t, int kappa)
{
	static const cv::Mat hN = (Mat_<double>(3, 3) << 0, 1, 0, 0, -1, 0, 0, 0, 0);
	// Variables
	double delta;
	double ikappa = 1 / kappa;

	// argument for filter
	static const int ddepth = -1;//CV_32F

	// convert to grayscale             ~0.5ms
	//cvtColor(img, res, CV_BGR2GRAY);

	// initialize filter outputs        ~2-3us
	cv::Mat nablaN, nablaS, nablaW, nablaE, nablaNE, nablaSE, nablaSW, nablaNW;

	// further initializations          ~2-3us
	cv::Mat cN, cS, cW, cE, cNE, cSE, cSW, cNW;

	// Anisotropic diffusion.           
	for (int i = 1; i <= num_iter; i++) // one iteration ~44ms
	{   
		// filtering                ~5ms
		cv::filter2D(res, nablaN, ddepth, hN);
		cv::filter2D(res, nablaS, ddepth, hS);
		cv::filter2D(res, nablaW, ddepth, hW);
		cv::filter2D(res, nablaE, ddepth, hE);
		cv::filter2D(res, nablaNE, ddepth, hNE);
		cv::filter2D(res, nablaSE, ddepth, hSE);
		cv::filter2D(res, nablaSW, ddepth, hSW);
		cv::filter2D(res, nablaNW, ddepth, hNW);

		// Diffusion function       ~28ms
		cN = nablaN*ikappa;
		cN.mul(cN);
		cN = 1.0 / (1.0 + cN);

		cS = nablaS*ikappa;
		cS.mul(cS);
		cS = 1.0 / (1.0 + cN);

		cW = nablaW*ikappa;
		cW.mul(cW);
		cW = 1.0 / (1.0 + cW);

		cE = nablaE*ikappa;
		cE.mul(cE);
		cE = 1.0 / (1.0 + cE);

		cNE = nablaNE*ikappa;
		cNE.mul(cNE);
		cNE = 1.0 / (1.0 + cNE);

		cSE = nablaSE*ikappa;
		cSE.mul(cSE);       
		cSE = 1.0 / (1.0 + cSE);

		cSW = nablaSW*ikappa;
		cSW.mul(cSW);
		cSW = 1.0 / (1.0 + cSW);

		cNW = nablaNW*ikappa;
		cNW.mul(cNW);
		cNW = 1.0 / (1.0 + cNW);

		// Discrete PDE solution.       ~11ms
		res = res +
			delta_t * (                                     // dy^2 = 1 und dd^2 = 1/2, deshalb entspricht es:
			cN.mul(nablaN)                                  //  (1 / (cv::pow(dy, 2)))*cN.mul(nablaN)
			+ cS.mul(nablaS)                                //+ (1 / (cv::pow(dy, 2)))*cS.mul(nablaS)
			+ cW.mul(nablaW)                                //+ (1 / (cv::pow(dx, 2)))*cW.mul(nablaW)
			+ cE.mul(nablaE)                                //+ (1 / (cv::pow(dx, 2)))*cE.mul(nablaE)
			+ cNE.mul(nablaNE) * 0.5                        //+ (1 / (cv::pow(dd, 2)))*cNE.mul(nablaNE)
			+ cSE.mul(nablaSE) * 0.5                        //+ (1 / (cv::pow(dd, 2)))*cSE.mul(nablaSE)
			+ cSW.mul(nablaSW) * 0.5                        //+ (1 / (cv::pow(dd, 2)))*cSW.mul(nablaSW)
			+ cNW.mul(nablaNW) * 0.5);                      //+ (1 / (cv::pow(dd, 2)))*cNW.mul(nablaNW));
	}
}
#endif

cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps)
{
    int hks = (ks-1)/2;
    double theta = th*CV_PI/180;
    double psi = ps*CV_PI/180;
    double del = 2.0/(ks-1);
    double lmbd = lm;
    double sigma = sig/ks;
    double x_theta;
    double y_theta;
    cv::Mat kernel(ks,ks, CV_32F);
    for (int y=-hks; y<=hks; y++)
    {
        for (int x=-hks; x<=hks; x++)
        {
            x_theta = x*del*cos(theta)+y*del*sin(theta);
            y_theta = -x*del*sin(theta)+y*del*cos(theta);
            kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(POW2(x_theta)+POW2(y_theta))/POW2(sigma))* 
				cos(2*CV_PI*x_theta/lmbd + psi);
        }
    }
    return kernel;
}

int g_fansiotropic=0;
int kernel_size=21;
int pos_sigma= 5;
int pos_lm = 50;
int pos_th = 0;
int pos_psi = 90;
cv::Mat src_f;
cv::Mat dest;

void Process(int pos, void *userdata)
{
	tracker_data *tracker = (tracker_data *)userdata;
//	assert(tracker);
    double sig = pos_sigma;
    double lm = 0.5+pos_lm/100.0;
    double th = pos_th;
    double ps = pos_psi;
    if (!kernel_size%2)
    {
        kernel_size+=1;
    }
    cv::Mat src_f;
    if(tracker){
		src_f=tracker->src_f;
		printf("%s:offset %d\n",tracker->trackername.c_str(), pos);
	}else{

	}
    cv::Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
    cv::filter2D(src_f, dest, CV_32F, kernel);
	if(g_fansiotropic){
		//convert from [0.0-1.0]- > [0-255], because itk reads 8 bit grey image
		cv::Mat wmat;
		dest.convertTo(wmat, CV_8UC1, 255.0);//
		//anisotropic smooth filter, input image is 8bit grey
		cvitk_AnisotropicDiffusionFilter(wmat, dest, 2);
	}
	if(tracker)
		cv::imshow(tracker->winname, dest);
	else
		cv::imshow("Process window", dest);
    cv::Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
    cv::resize(kernel, Lkernel, Lkernel.size());
    Lkernel /= 2.;
    Lkernel += 0.5;
    cv::imshow(std::string("Kernel ")+tracker->winname, Lkernel);
    cv::Mat mag;
    cv::pow(dest, 2.0, mag);
    cv::imshow(std::string("Mag ")+tracker->winname, mag);
}
