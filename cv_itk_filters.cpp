/** @brief : ntu mip 2015 final project
 * @author : <thomas tsai, d04922009@ntu.edu.tw, thomas@life100.cc>
 *
 * http://itk.org/ITKSoftwareGuide/html/Book2/ITKSoftwareGuide-Book2ch2.html
 * 
 * 
 */

#include <itkImage.h>
#include <itkCurvatureFlowImageFilter.h>
#include <itkOpenCVImageBridge.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <math.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <itkImage.h>
#include <itkMedianImageFilter.h>
#include <itkOpenCVImageBridge.h>
#include "cv_itk.hpp"

using namespace std;
using namespace cv;

/** @brief opencv and itk
 * 
 * https://github.com/biotrump/ITKv4-TheNextGeneration-Tutorial.git
 * http://www.na-mic.org/Wiki/images/4/4a/ITKOpenCVBridgeTutorial.pdf
 */
int cvitk_mediaFilter(cv::Mat &inputImage, cv::Mat &resultImage)
{
	typedef unsigned char                            InputPixelType;
	typedef unsigned char                            OutputPixelType;
	const unsigned int Dimension =                   2;
	typedef itk::Image< InputPixelType, Dimension >  InputImageType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	typedef itk::OpenCVImageBridge                   BridgeType;

	typedef itk::MedianImageFilter< InputImageType, OutputImageType > 
													FilterType;
	//opencv image to itk image
	InputImageType::Pointer itkImage =
	BridgeType::CVMatToITKImage< InputImageType >( inputImage );

	FilterType::Pointer filter = FilterType::New();
	InputImageType::SizeType neighborhoodRadius;
	neighborhoodRadius[0] = 9;
	neighborhoodRadius[1] = 9;
	filter->SetRadius( neighborhoodRadius );

	//set the input source for itk filter
	filter->SetInput( itkImage );
	try
	{
		filter->Update();	//filter is working here.
	}
	catch( itk::ExceptionObject & excp )
	{
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}

	//get the output of itk filter
	//convert to opencv image
	resultImage =
	BridgeType::ITKImageToCVMat< OutputImageType >( filter->GetOutput() );
	return EXIT_SUCCESS;
}

/** itk/Modules/Filtering/ImageSources/test/itkGaborImageSourceTest.cxx
 * generate a gabor image as a source
 * Orientation can be manipulated via the Transform classes of the toolkit.
 * the pixel value in the image is [0.0-1.0],
 * so openCV should resultImage.convertTo( scaled, CV_8UC1, 255.0 );
 * 
 * inputImage : opencv image, 8 bit 2D grey image
 * kernelImage, resultImage are all float type, [0.0-1.0]
 */
int cvitk_GaborImageSource2D(cv::Mat &inputImage, cv::Mat &kernelImage, cv::Mat &resultImage)
{
	typedef float PixelType;//4 bytes float
	typedef float OutputPixelType;
	typedef float InputPixelType;
	const unsigned int ImageDimension = 2;
	typedef itk::Image< InputPixelType,  ImageDimension >   InputImageType;
	typedef itk::Image< OutputPixelType,  ImageDimension >   OutputImageType;
	typedef itk::Image<PixelType, ImageDimension> ImageType;
	typedef itk::OpenCVImageBridge                   BridgeType;
	// Instantiate the filter
	typedef itk::GaborImageSource<ImageType> GaborSourceType;
	GaborSourceType::Pointer gaborImage = GaborSourceType::New();
	gaborImage->Print(std::cout);

	//convert opencv image to itk image
	InputImageType::Pointer itkImage =
	BridgeType::CVMatToITKImage< InputImageType >( inputImage );
	
	GaborSourceType::ArrayType sigma;
	sigma[0] = 2.0;
	sigma[1] = 5.0;
	//sigma[0] = 20.0;
	//sigma[1] = 50.0;
	
	ImageType::SizeType size;
	size.Fill( 32 /*64*4*/ );//N*N=256*256

	gaborImage->SetSize( size );

	gaborImage->SetSigma( sigma );
	gaborImage->SetFrequency( 0.1 );
	gaborImage->SetCalculateImaginaryPart( false);

	try
	{
		gaborImage->Update();
		gaborImage->Print(std::cout);
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}
/*
	typedef itk::ImageFileWriter<ImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( argv[1] );
	writer->SetInput( gaborImage->GetOutput() );
	writer->Update();
*/
	//get the output of itk filter
	//convert to opencv image, pixel value is float,32bits,[0.0-1.0]
	kernelImage =
	BridgeType::ITKImageToCVMat< ImageType >( gaborImage->GetOutput() );

    /**
     * Convolve the input image with the resampled gabor image kernel.
     */
    typedef itk::ConvolutionImageFilter<ImageType> ConvolutionFilterType;
    typename ConvolutionFilterType::Pointer convoluter
      = ConvolutionFilterType::New();
    convoluter->SetInput( itkImage );
#if ITK_VERSION_MAJOR >= 4
	convoluter->SetKernelImage(gaborImage->GetOutput());
#else
	convoluter->SetImageKernelInput(gaborImage->GetOutput());
#endif
    convoluter->NormalizeOn();
	try
	{
		convoluter->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}

	//convert to opencv image
	resultImage =
	BridgeType::ITKImageToCVMat< OutputImageType >( convoluter->GetOutput() );
	
	return EXIT_SUCCESS;
}

//int ks=21, float sig=5.0, float the=0.0, float lm=50.0, float ps=90.0)
int cvitk_GaborImageFilter2D(cv::Mat &inputImage, cv::Mat &kernelImage, cv::Mat &resultImage,
	int ks, float sig, float the, float lm, float ps)
{
    if (! (ks & 0x1) )	//even to odd
    {
        ks+=1;
    }
	int hks = (ks-1)/2;
	printf("ks=%d, hks=%d\n", ks, hks);
    float theta = the*CV_PI/180;
    float psi = ps*CV_PI/180;
    float del = 2.0/(ks-1);
    float lmbd = lm;
    float iso_sigma = sig/(float)ks;
	printf("iso_sigma=%f, sig=%f, ks=%d\n", iso_sigma, sig, ks);
    float x_theta;
    float y_theta;

	typedef float PixelType;//4 bytes float
	typedef float OutputPixelType;
	typedef float InputPixelType;
	const unsigned int ImageDimension = 2;
	typedef itk::Image< InputPixelType,  ImageDimension >   InputImageType;
	typedef itk::Image< OutputPixelType,  ImageDimension >   OutputImageType;
	typedef itk::Image<PixelType, ImageDimension> ImageType;
	typedef itk::OpenCVImageBridge                   BridgeType;
	// Instantiate the filter
	typedef itk::GaborImageSource<ImageType> GaborSourceType;
	GaborSourceType::Pointer gaborImage = GaborSourceType::New();
	gaborImage->Print(std::cout);

	//convert opencv image to itk image
	InputImageType::Pointer itkImage =
	BridgeType::CVMatToITKImage< InputImageType >( inputImage );
	
	GaborSourceType::ArrayType sigma;
	sigma[0] = 2.0;
	sigma[1] = 5.0;
	//gaussian is isotropic
	//sigma[0] = iso_sigma;
	//sigma[1] = iso_sigma;
	
	ImageType::SizeType size;
	size.Fill( hks );//N*N=256*256

	gaborImage->SetSize( size );

	gaborImage->SetSigma( sigma );
	gaborImage->SetFrequency( 1.0/lmbd /*0.1*/ );
	gaborImage->SetCalculateImaginaryPart( false);

	try
	{
		gaborImage->Update();
		gaborImage->Print(std::cout);
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}
/*
	typedef itk::ImageFileWriter<ImageType> WriterType;
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName( argv[1] );
	writer->SetInput( gaborImage->GetOutput() );
	writer->Update();
*/
	//get the output of itk filter
	//convert to opencv image, pixel value is float,32bits,[0.0-1.0]
	kernelImage =
	BridgeType::ITKImageToCVMat< ImageType >( gaborImage->GetOutput() );

    /**
     * Convolve the input image with the resampled gabor image kernel.
     */
    typedef itk::ConvolutionImageFilter<ImageType> ConvolutionFilterType;
    typename ConvolutionFilterType::Pointer convoluter
      = ConvolutionFilterType::New();
    convoluter->SetInput( itkImage );
#if ITK_VERSION_MAJOR >= 4
	convoluter->SetKernelImage(gaborImage->GetOutput());
#else
	convoluter->SetImageKernelInput(gaborImage->GetOutput());
#endif
    convoluter->NormalizeOn();
	try
	{
		convoluter->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}

	//convert to opencv image
	resultImage =
	BridgeType::ITKImageToCVMat< OutputImageType >( convoluter->GetOutput() );
	
	return EXIT_SUCCESS;
}

/** 2.7.3 Edge Preserving Smoothing
 * Introduction to Anisotropic Diffusion
 * 
 * Gradient Anisotropic Diffusion

 A similar nonlinear diffusion processes to model human vision. 
 The motivation for anisotropic diffusion (also called nonuniform or variable conductance diffusion)

The source code for this section can be found in the file
GradientAnisotropicDiffusionImageFilter.cxx.
The itk::GradientAnisotropicDiffusionImageFilter implements an N-dimensional version of the classic Perona-Malik anisotropic diffusion equation for scalar-valued images [46].
The conductance term for this implementation is chosen as a function of the gradient magnitude of the image at each point, reducing the strength of diffusion at edge pixels.

 C (x)= e−(∥∇U(Kx)∥)2
(2.10)
The numerical implementation of this equation is similar to that described in the Perona-Malik paper [46], but uses a more robust technique for gradient magnitude estimation and has been generalized to N-dimensions.
The first step required to use this filter is to include its header file.
Types should be selected based on the pixel types required for the input and output images. The image types are defined using the pixel type and the dimension.

Typical values for the time step are 0.25 in 2D images and 0.125 in 3D images. 
The number of iterations is typically set to 5; 
more iterations result in further smoothing and will increase the computing time linearly.

resultImage, inputImage: 8bit grey
*/
int cvitk_AnisotropicDiffusionFilter(cv::Mat &inputImage, cv::Mat &resultImage, 
			int numberOfIterations, const double & conductance, float timeStep)
{
    typedef    float    InputPixelType;
    typedef    float    OutputPixelType;
	const unsigned int Dimension =                   2;
    typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
    typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
	typedef itk::OpenCVImageBridge                   BridgeType;//opencv to itk

	//The filter type is now instantiated using both the input image and the output image types. 
	//The filter object is created by the New() method.
    typedef itk::GradientAnisotropicDiffusionImageFilter<
                 InputImageType, OutputImageType >  FilterType; 
	//convert opencv image to itk image
	InputImageType::Pointer itkImage =
	BridgeType::CVMatToITKImage< InputImageType >( inputImage );
	
    FilterType::Pointer filter = FilterType::New();

	//This filter requires three parameters: the number of iterations to be performed, 
	//the time step and the conductance parameter used in the computation of the level set evolution.
	//These parameters are set using the methods SetNumberOfIterations(), SetTimeStep() and 
	//SetConductanceParameter() respectively. The filter can be executed by invoking Update().
    filter->SetNumberOfIterations( numberOfIterations );
    filter->SetTimeStep( timeStep ); 
    filter->SetConductanceParameter( conductance );

	//The input image can be obtained from the output of another filter. 
	//Here, an image reader is used as source.
    //filter->SetInput( reader->GetOutput() );//from itk
	filter->SetInput( itkImage );	//from opencv instead
	try
	{
		filter->Update();
	}
	catch( itk::ExceptionObject & excp )
	{
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}
	//get the output of itk filter
	//convert to opencv image
	resultImage =
	BridgeType::ITKImageToCVMat< OutputImageType >( filter->GetOutput() );
	return EXIT_SUCCESS;
}

int cvitk_CurvatureAnisotropicDiffusionFilter(cv::Mat &inputImage, cv::Mat &resultImage, 
				int numberOfIterations, const double & conductance, float timeStep,
				int useImageSpacing)
{
    typedef    float    InputPixelType;
    typedef    float    OutputPixelType;
	const unsigned int Dimension =                   2;
    typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
    typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
	typedef itk::OpenCVImageBridge                   BridgeType;//opencv to itk

	//The filter type is now instantiated using both the input image and the output image types. 
	//The filter object is created by the New() method.
    typedef itk::CurvatureAnisotropicDiffusionImageFilter<
                 InputImageType, OutputImageType >  FilterType; 
	//convert opencv image to itk image
	InputImageType::Pointer itkImage =
	BridgeType::CVMatToITKImage< InputImageType >( inputImage );
	
    FilterType::Pointer filter = FilterType::New();

	//This filter requires three parameters: the number of iterations to be performed, 
	//the time step and the conductance parameter used in the computation of the level set evolution.
	//These parameters are set using the methods SetNumberOfIterations(), SetTimeStep() and 
	//SetConductanceParameter() respectively. The filter can be executed by invoking Update().
    filter->SetNumberOfIterations( numberOfIterations );
    filter->SetTimeStep( timeStep ); 
    filter->SetConductanceParameter( conductance );
	if (useImageSpacing)
	{ 
		filter->UseImageSpacingOn();
	}
	//The input image can be obtained from the output of another filter. 
	//Here, an image reader is used as source.
    //filter->SetInput( reader->GetOutput() );//from itk
	filter->SetInput( itkImage );	//from opencv instead
	try
	{
		filter->Update();
	}
	catch( itk::ExceptionObject & excp )
	{
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}
	//get the output of itk filter
	//convert to opencv image
	resultImage =
	BridgeType::ITKImageToCVMat< OutputImageType >( filter->GetOutput() );
	return EXIT_SUCCESS;
}
