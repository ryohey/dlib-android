/*
*  Copyright (C) 2015 TzuTaLin
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

package com.tzutalin.dlib;

/**
 * Created by Tzutalin on 2015/10/20.
 */

import android.graphics.Point;

import java.util.ArrayList;

/**
 * A VisionDetRet contains all the information identifying the location and confidence value of the detected object in a bitmap.
 */
public final class VisionDetRet {
    private ArrayList<Point> mLandmarkPoints = new ArrayList<>();

    public VisionDetRet() {
    }

    /**
     * Add landmark to the list. Usually, call by jni
     * @param x Point x
     * @param y Point y
     * @return true if adding landmark successfully
     */
    public boolean addLandmark(int x, int y) {
        return mLandmarkPoints.add(new Point(x, y));
    }

    /**
     * Return the list of landmark points
     * @return ArrayList of android.graphics.Point
     */
    public ArrayList<Point> getFaceLandmarks() {
        return mLandmarkPoints;
    }
}
