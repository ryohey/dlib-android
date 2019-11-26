/*
 * jni_pedestrian_det.cpp using google-style
 *
 *  Created on: Oct 20, 2015
 *      Author: Tzutalin
 *
 *  Copyright (c) 2015 Tzutalin. All rights reserved.
 */
#include <android/bitmap.h>
#include <jni_common/jni_bitmap2mat.h>
#include <jni_common/jni_primitives.h>
#include <jni_common/jni_fileutils.h>
#include <jni_common/jni_utils.h>
#include "detector.h"
#include <jni.h>


using namespace cv;

extern JNI_VisionDetRet* g_pJNI_VisionDetRet;

namespace {

#define JAVA_NULL 0
using DetectorPtr = DLibHOGFaceDetector*;

class JNI_FaceDet {
 public:
  JNI_FaceDet(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASSNAME_FACE_DET);
    mNativeContext = env->GetFieldID(clazz, "mNativeFaceDetContext", "J");
    env->DeleteLocalRef(clazz);
  }

  DetectorPtr getDetectorPtrFromJava(JNIEnv* env, jobject thiz) {
    DetectorPtr const p = (DetectorPtr)env->GetLongField(thiz, mNativeContext);
    return p;
  }

  void setDetectorPtrToJava(JNIEnv* env, jobject thiz, jlong ptr) {
    env->SetLongField(thiz, mNativeContext, ptr);
  }

  jfieldID mNativeContext;
};

// Protect getting/setting and creating/deleting pointer between java/native
std::mutex gLock;

std::shared_ptr<JNI_FaceDet> getJNI_FaceDet(JNIEnv* env) {
  static std::once_flag sOnceInitflag;
  static std::shared_ptr<JNI_FaceDet> sJNI_FaceDet;
  std::call_once(sOnceInitflag, [env]() {
    sJNI_FaceDet = std::make_shared<JNI_FaceDet>(env);
  });
  return sJNI_FaceDet;
}

DetectorPtr const getDetectorPtr(JNIEnv* env, jobject thiz) {
  std::lock_guard<std::mutex> lock(gLock);
  return getJNI_FaceDet(env)->getDetectorPtrFromJava(env, thiz);
}

// The function to set a pointer to java and delete it if newPtr is empty
void setDetectorPtr(JNIEnv* env, jobject thiz, DetectorPtr newPtr) {
  std::lock_guard<std::mutex> lock(gLock);
  DetectorPtr oldPtr = getJNI_FaceDet(env)->getDetectorPtrFromJava(env, thiz);
  if (oldPtr != JAVA_NULL) {
    DLOG(INFO) << "setMapManager delete old ptr : " << oldPtr;
    delete oldPtr;
  }

  if (newPtr != JAVA_NULL) {
    DLOG(INFO) << "setMapManager set new ptr : " << newPtr;
  }

  getJNI_FaceDet(env)->setDetectorPtrToJava(env, thiz, (jlong)newPtr);
}

}  // end unnamespace

#ifdef __cplusplus
extern "C" {
#endif


#define DLIB_FACE_JNI_METHOD(METHOD_NAME) \
  Java_com_tzutalin_dlib_FaceDet_##METHOD_NAME

JNIEXPORT jobject JNICALL
    DLIB_FACE_JNI_METHOD(jniBitmapDetect)(JNIEnv* env, jobject thiz,
                                          jobject bitmap,
                                          int left, int top, int right, int bottom) {
  LOG(INFO) << "jniBitmapFaceDet";
  cv::Mat rgbaMat;
  cv::Mat bgrMat;
  jniutils::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
  cv::cvtColor(rgbaMat, bgrMat, cv::COLOR_RGBA2BGR);
  auto rect = dlib::rectangle(left, top, right, bottom);
  auto detPtr = getDetectorPtr(env, thiz);
  auto shape = detPtr->det(bgrMat, rect);

  auto jDetRet = JNI_VisionDetRet::createJObject(env);
  for (auto j = 0; j < shape.num_parts(); j++) {
    int x = shape.part(j).x();
    int y = shape.part(j).y();
    // Call addLandmark
    g_pJNI_VisionDetRet->addLandmark(env, jDetRet, x, y);
  }
  return jDetRet;
}

jint JNIEXPORT JNICALL DLIB_FACE_JNI_METHOD(jniInit)(JNIEnv* env, jobject thiz,
                                                     jstring jLandmarkPath) {
  LOG(INFO) << "jniInit";
  std::string landmarkPath = jniutils::convertJStrToString(env, jLandmarkPath);
  DetectorPtr detPtr = new DLibHOGFaceDetector(landmarkPath);
  setDetectorPtr(env, thiz, detPtr);
  ;
  return JNI_OK;
}

jint JNIEXPORT JNICALL
    DLIB_FACE_JNI_METHOD(jniDeInit)(JNIEnv* env, jobject thiz) {
  LOG(INFO) << "jniDeInit";
  setDetectorPtr(env, thiz, JAVA_NULL);
  return JNI_OK;
}

#ifdef __cplusplus
}
#endif
