import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.graphics.Rect;
import android.support.test.filters.SmallTest;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;
import com.tzutalin.dlib.Constants;
import com.tzutalin.dlib.FaceDet;
import com.tzutalin.dlib.VisionDetRet;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import junit.framework.Assert;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import static org.hamcrest.core.IsNull.notNullValue;
import static org.junit.Assert.assertThat;

/**
 * Created by houzhi on 16-10-20.
 */
@RunWith(AndroidJUnit4.class)
@SmallTest
public class FaceDetTest {

    @Before
    public void setup() {
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testDetBitmapFaceLandmarkDect() {
        final Bitmap bitmap = BitmapFactory.decodeFile("/sdcard/test.png");
        assertThat(bitmap, notNullValue());
        final FaceDet faceDet = new FaceDet(Constants.getFaceShapeModelPath());

        long startTime = System.currentTimeMillis();
        List<VisionDetRet> result = Arrays.asList(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10).parallelStream().map((i) -> {
            return faceDet.detect(bitmap,
                new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight()));
        }).collect(Collectors.toList());

        long time = System.currentTimeMillis() - startTime;
        Log.d("testDetBitmapFaceLandmarkDect", "take " + time + " ms");

        ArrayList<Point> landmarks = result.get(0).getFaceLandmarks();
        Assert.assertTrue(landmarks.size() > 0);
        faceDet.release();
    }
}
