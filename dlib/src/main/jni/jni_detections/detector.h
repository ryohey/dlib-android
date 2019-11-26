/*
 * detector.h using google-style
 *
 *  Created on: May 24, 2016
 *      Author: Tzutalin
 *
 *  Copyright (c) 2016 Tzutalin. All rights reserved.
 */

#pragma once

#include <jni_common/jni_fileutils.h>
#include "../dlib/dlib/image_loader/load_image.h"
#include "../dlib/dlib/image_processing.h"
#include "../dlib/dlib/image_processing/frontal_face_detector.h"
#include "../dlib/dlib/image_processing/render_face_detections.h"
#include "../dlib/dlib/opencv/cv_image.h"
#include "../dlib/dlib/image_loader/load_image.h"
#include "../dlib/dlib/custom_deserialization.h"
#include "../miniglog/glog/logging.h"
#include <jni.h>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

/*
 * DLib face feature extractor
 */
class DLibHOGFaceDetector {
 private:
  dlib::shape_predictor msp;

  inline void init() {
    LOG(INFO) << "Init mFaceDetector";
  }

 public:
  DLibHOGFaceDetector() { init(); }

  DLibHOGFaceDetector(const std::string& landmarkmodel) {
    init();
    if (!landmarkmodel.empty() && jniutils::fileExists(landmarkmodel)) {
      dlib::deserializeMinified(msp, landmarkmodel);
      LOG(INFO) << "Load landmarkmodel from " << landmarkmodel;
    }
  }

  virtual inline dlib::full_object_detection det(const cv::Mat& image, dlib::rectangle rect) {
      LOG(INFO) << "com_tzutalin_dlib_PeopleDet go to detLandmark(mat)";
      if (image.channels() == 1) {
          cv::cvtColor(image, image, cv::ColorConversionCodes::COLOR_GRAY2BGR);
      }

      CHECK(image.channels() == 3);

      dlib::cv_image<dlib::bgr_pixel> img(image);
      dlib::full_object_detection shape = msp(img, rect);
      return shape;
  }
};
