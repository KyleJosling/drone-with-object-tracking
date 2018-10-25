#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include "detectObject/detectObject.hpp"

// Image processing function
cv::Mat processImg(cv::Mat frame, int hue, int sat, int val) {

    // Create object for eroding (2x2 rectangle)
    cv::Mat erode_rect = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2,2));
    // Create object for dilating (5x5 rectangle)
    cv::Mat dilate_rect = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
    
    cv::Mat frame_hsv;
    cv::Mat frame_threshold;

    //Convert the colour to hsv
    cv::cvtColor( frame, frame_hsv, CV_BGR2HSV );

    //Mask
    cv::inRange( frame_hsv, cv::Scalar(hue-40,sat,val), cv::Scalar(hue+40,255,255), frame_threshold);

    cv::erode(frame_threshold, frame_threshold, erode_rect, cv::Point(-1,-1),2);
    cv::dilate(frame_threshold, frame_threshold, dilate_rect, cv::Point(-1,-1), 2);

    return frame_threshold;
}

//Function that returns object in frame
cv::Rect2d detectObject(cv::Mat frame_threshold) {

    obj_point objectPoint;

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
    if (largest_contour_area == 0 || largest_contour_area < 400) {
        std::cout<< "No objects found " << std::endl;
        return cv::Rect2d(0,0,0,0);
    } else {
        return cv::boundingRect(cv::Mat(contours[largest_contour_no]));
    }
    
    #ifdef DISPLAY
    cv::namedWindow("video", CV_WINDOW_AUTOSIZE);
    cv::resizeWindow("Final", 500,500);
    cv::imshow("video1", frame_threshold);
    #endif
}
