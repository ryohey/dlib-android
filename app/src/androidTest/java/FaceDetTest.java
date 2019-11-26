import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.support.test.filters.SmallTest;
import android.support.test.runner.AndroidJUnit4;
import com.tzutalin.dlib.Constants;
import com.tzutalin.dlib.FaceDet;
import com.tzutalin.dlib.VisionDetRet;
import java.util.ArrayList;
import junit.framework.Assert;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import static org.hamcrest.Matchers.notNullValue;
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
        Bitmap bitmap = BitmapFactory.decodeFile("/sdcard/test.jpg");
        assertThat(bitmap, notNullValue());
        FaceDet faceDet = new FaceDet(Constants.getFaceShapeModelPath());
        VisionDetRet ret = faceDet.detect(bitmap);
        ArrayList<Point> landmarks = ret.getFaceLandmarks();
        Assert.assertTrue(landmarks.size() > 0);
        faceDet.release();
    }
}
