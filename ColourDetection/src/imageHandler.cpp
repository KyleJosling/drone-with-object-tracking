#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include <iostream>

#include "globals.hpp"

#include "detectObject/detectObject.hpp"
#include "imageHandler/imageHandler.hpp"


void imageHandler() {

    obj_point detectedPoint;

    // Open video capture
    cv::VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cout << "Fail" << std::endl;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    int frameCounter=0;
    cv::Mat frame;
    
    while (true) {

        //Get a frame
        cap >> frame;
        
        // Detect the object in the frame (hard coded values for now)
        detectedPoint=detectObject(frame, 145, 50, 65);

        std::cout<< detectedPoint.pt.x << std::endl;
        std::cout<< detectedPoint.pt.y << std::endl;

        //Set process variable
        yawPVar=detectedPoint.pt.x;

        //Increment the frame counter
        frameCounter++;
        std::cout << "Counter: " << frameCounter << std::endl;

        #ifdef DISPLAY 
        cv::namedWindow("video", CV_WINDOW_AUTOSIZE);
        cv::resizeWindow("Final", 500,500);
        cv::imshow("video", frame);
        #endif

        int c = cv::waitKey(10);
        if( (char)c == 'c' ) { break; }

    }

    cv::destroyAllWindows();
    cap.release();

}
