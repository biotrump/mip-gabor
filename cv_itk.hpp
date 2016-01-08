/** @brief : ntu mip 2015 final project
 * @author : <thomas tsai, d04922009@ntu.edu.tw, thomas@life100.cc>
 *
 * http://itk.org/ITKSoftwareGuide/html/Book2/ITKSoftwareGuide-Book2ch2.html
 * 
 * 
 */
#ifndef _H_CV_ITK_H
#define	_H_CV_ITK_H

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkConvolutionImageFilter.h"

#include "itkGaborImageSource.h"
#include "itkGaussianInterpolateImageFunction.h"
#include "itkImageRegionIterator.h"
#include "itkTimeProbe.h"

#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"

int cvitk_mediaFilter(cv::Mat &inputImage, cv::Mat &resultImage);
int cvitk_GaborImageSource2D(cv::Mat &inputImage, cv::Mat &kernelImage, cv::Mat &resultImage);
int cvitk_GaborImageFilter2D(cv::Mat &inputImage, cv::Mat &kernelImage, cv::Mat &resultImage,
	int ks=21, float sig=5.0, float the=0.0, float lm=50.0, float ps=90.0);
int cvitk_AnisotropicDiffusionFilter(cv::Mat &inputImage, cv::Mat &resultImage, 
			int numberOfIterations=5, const double & conductance=1.0, float timeStep=0.25);
int cvitk_CurvatureAnisotropicDiffusionFilter(cv::Mat &inputImage, cv::Mat &resultImage, 
				int numberOfIterations=5, const double & conductance=1.0, float timeStep=0.25,
				int useImageSpacing=0);

void Process_itk(int pos, void *userdata);
void Process(int pos, void *userdata);
#endif
