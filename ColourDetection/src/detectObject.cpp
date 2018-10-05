#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include "detectObject/detectObject.hpp"


//Function that returns faces in frame
struct obj_point detectObject(cv::Mat frame, int hue, int sat, int val) {

    obj_point objectPoint;

    // Create object for eroding (2x2 rectangle)
    cv::Mat erode_rect = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
    // Create object for dilating (5x5 rectangle)
    cv::Mat dilate_rect = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
    
    cv::Mat frame_hsv;
    cv::Mat frame_threshold;

    //Convert the colour to hsv
    cv::cvtColor( frame, frame_hsv, CV_BGR2HSV );

    //Mask
    cv::inRange( frame_hsv, cv::Scalar(hue-7,sat,val), cv::Scalar(hue+7,255,255), frame_threshold);

    cv::erode(frame_threshold, frame_threshold, erode_rect, cv::Point(-1,-1),2);
    cv::dilate(frame_threshold, frame_threshold, dilate_rect, cv::Point(-1,-1), 2);

    //Display
    // namedWindow("video", CV_WINDOW_AUTOSIZE);
    // resizeWindow("Final", 500,500);
    // imshow("video", frame_threshold);

    //Find the contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(frame_threshold, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    int contour_no = 0;
    int largest_contour_no = 0;
    int largest_contour_area = 0;

    // Iterate through the contours
    for(contour_no; contour_no < contours.size(); contour_no++)
    {
        // Find the largest contour
        int current_area = cv::contourArea(contours[contour_no]);

        if(current_area > largest_contour_area)
        {
            largest_contour_area = current_area;
            largest_contour_no = contour_no;
        }
    }

    // If no contours found then mean point is 0
    if (largest_contour_area == 0 || largest_contour_area < 800) {
        objectPoint.pt= cv::Point2f(0,0);
        objectPoint.size=0;
    }

    else {
        // Find moments of largest contour
        cv::Moments obj_momts = moments(contours[largest_contour_no], false);
        // Calculate moment centre
        objectPoint.pt= cv::Point2f(obj_momts.m10 / obj_momts.m00, obj_momts.m01 / obj_momts.m00);
        objectPoint.size=largest_contour_area;

    }

    return objectPoint;
    // return 1;
}
