#$URL: svn+ssh://teboul@athena/home/svnrepositories/BuildingModeling/branches/Olivier/CMakeLists.txt $
#$Author: simon $
#$Date: 2010-02-18 13:14:50 +0100 (jeu., 18 févr. 2010) $

cmake_minimum_required(VERSION 2.6)

project(RFlib)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

include_directories(${CMAKE_SOURCE_DIR})
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES core)

# Library
set(RF_SRCS
	inc/Histogram.h
	inc/IClassifier.h
	inc/IFeatureVector.h
	inc/LeafNode.h
	inc/RandomizedForest.h
	inc/RandomizedTest.h
	inc/RandomizedTree.h
	src/Histogram.cpp
	src/IClassifier.cpp
	src/IFeatureVector.cpp
	src/LeafNode.cpp
	src/RandomizedForest.cpp
	src/RandomizedTest.cpp
	src/RandomizedTree.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
add_library(RFlib ${RF_SRCS})

# Executable
#find_package(OpenCV)
SET(OpenCV_LINK_DIRECTORIES "C:/opencv/build/x64/vc14/lib")
SET(OpenCV_INCLUDE_DIRS "C:/opencv/build/include")

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LINK_DIRECTORIES})

set(Patch_SRCS
	inc/Patch.h
	inc/Color.h
	src/Patch.cpp
	src/Color.cpp
	src/run_patch.cpp
)

add_executable(run_patch ${Patch_SRCS})
target_link_libraries(run_patch RFlib opencv_world310.lib)
