#thomas tsai, thomas@life100.cc
#http://docs.opencv.org/3.1.0/db/df5/tutorial_linux_gcc_cmake.html#gsc.tab=0
cmake_minimum_required(VERSION 2.8)
project( SIFTKP )

# --------------------------------------------------
# Organize output files.  On Windows this also keeps .dll files next
# to the .exe files that need them, making tests easy to run.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SIFTKP_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SIFTKP_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SIFTKP_BINARY_DIR}/lib)
#-----------------------------------------------------
#export OpenCV_DIR=/ssd/opencv/build_2.4.12.2/debug
#export OpenCV_DIR=/home/thomas/build/bcv-x86.gl/out/opencv/2.4.x-static/x86_64
#ITK 4.8.x is not compatible to opencv 3.x : cv::MAT
#export OpenCV_DIR=/ssd/opencv/build_3.0.0/x86_64_debug
#export OpenCV_DIR=/ssd/opencv-3.1.0/build/debug-x86-64
set(OpenCV_DIR $ENV{OpenCV_DIR})
message("++++OpenCV_DIR:${OpenCV_DIR}")
FIND_PACKAGE( OpenCV REQUIRED)
if(OpenCV_FOUND)
	message("found")
	message( "OpenCV_INCLUDE_DIRS:${OpenCV_INCLUDE_DIRS}" )
	message( "OpenCV_INCLUDES:${OpenCV_INCLUDES}")
	message( "OpenCV_LIBRARIES:${OpenCV_LIBRARIES}")
	message( "OpenCV_LIBS:${OpenCV_LIBS}")
	message( "OpenCV_DEFINITIONS:${OpenCV_DEFINITIONS}")
else()
	message(FATAL_ERROR "openCV not foud")
endif()
#include_directories( ${OpenCV_INCLUDE_DIRS} )
message("*** OpenCV_LIBS:${OpenCV_LIBS}")
message("*** OpenCV_LIBRARIES:${OpenCV_LIBRARIES}")
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()
#---------------------------------------------------------
# Widgets finds its own dependencies (QtGui and QtCore).
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
#  find_package(Qt5Declarative)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
#  set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#---------------------------------------------------------
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
#---------------------------------------------------------
add_executable( siftkp
			kp.cpp )

target_link_libraries( siftkp ${OpenCV_LIBS} )
qt5_use_modules(siftkp Widgets  OpenGL Test Concurrent)
