#thomas tsai, thomas@life100.cc
#http://docs.opencv.org/3.1.0/db/df5/tutorial_linux_gcc_cmake.html#gsc.tab=0
cmake_minimum_required(VERSION 2.8)
project( GABOR )

# --------------------------------------------------
# Organize output files.  On Windows this also keeps .dll files next
# to the .exe files that need them, making tests easy to run.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GABOR_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GABOR_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GABOR_BINARY_DIR}/lib)

#OPENCV_OUT=opencv/build/2.4.x-static/x86_64
set(OpenCV_DIR $ENV{OPENCV_OUT})
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

message("*** OpenCV_LIBS:${OpenCV_LIBS}")
message("*** OpenCV_LIBRARIES:${OpenCV_LIBRARIES}")
#---------------------------------------------------------
# Widgets finds its own dependencies (QtGui and QtCore).
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
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage DisplayImage.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )
qt5_use_modules(DisplayImage Widgets  OpenGL Test Concurrent)
