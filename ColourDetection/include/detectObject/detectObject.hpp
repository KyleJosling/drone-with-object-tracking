#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

struct obj_point {

    cv::Point2f pt;
    int size;

};

cv::Mat processImg(cv::Mat frame, int hue, int sat, int val); 
cv::Rect2d detectObject(cv::Mat frame_threshold);
