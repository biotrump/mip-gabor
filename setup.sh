#!/bin/bash
#using noqt build to build itk with opencv brdige
#export OpenCV_DIR=/ssd/opencv/build_2.4.12.2/debug-noqt
#normal openCV build with qt no mather itk-to-opencv is built without qt.
export OpenCV_DIR=/ssd/opencv/build_2.4.12.2/debug
#export OpenCV_DIR=/home/thomas/build/bcv-x86.gl/out/opencv/2.4.x-static/x86_64
#ITK 4.8.x is not compatible to opencv 3.x : cv::MAT
#export OpenCV_DIR=/ssd/opencv/build_3.0.0/x86_64_debug
#export OpenCV_DIR=/ssd/opencv-3.1.0/build/debug-x86-64

export VTK_DIR=/home/thomas/MIP/VTK/build/debug.6.3
#building ITK
export ITK_OUT=/home/thomas/MIP/ITK/build/debug.4.8.1
