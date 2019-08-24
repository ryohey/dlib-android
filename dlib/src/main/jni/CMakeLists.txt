cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti -fexceptions -ffast-math -D_GLIBCXX_HAS_GTHREADS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")

include_directories(miniglog)
include_directories(OpenCV-android-sdk/sdk/native/jni/include)

add_library(android_dlib SHARED
    dlib/dlib/threads/threads_kernel_shared.cpp
    dlib/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp
    dlib/dlib/base64/base64_kernel_1.cpp
    dlib/dlib/threads/threads_kernel_1.cpp
    dlib/dlib/threads/threads_kernel_2.cpp
    jni_common/jni_bitmap2mat.cpp
	jni_common/jni_fileutils.cpp
	jni_common/jni_utils.cpp
	jni_common/rgb2yuv.cpp
	jni_common/yuv2rgb.cpp
	jni_detections/jni_face_det.cpp
	jni_detections/jni_imageutils.cpp
	jni_detections/jni_pedestrian_det.cpp
	miniglog/glog/logging.cc
)
