#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include <iostream>

#include "globals.hpp"

#include "detectObject/detectObject.hpp"
#include "imageHandler/imageHandler.hpp"


void imageHandler() {
    
    // Detected object
    obj_point detectedPoint;

    // Tracker object
    cv::Ptr<cv::TrackerKCF> tracker = cv::TrackerKCF::create();

    // Open video capture
    cv::VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cout << "Fail" << std::endl;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    int frameCounter=0;
    cv::Mat frame;
    cv::Rect2d roi;
    bool ok;
    // Frame loop
    while (true) {

        //Get a frame
        cap >> frame;
        
        if (frameCounter == 0 || frameCounter > 100) {

            // Detect the object in the frame (hard coded values for now)
            roi = detectObject(frame, 145, 50, 65);
            
            // Reinitialize tracker
            if (roi.height > 0) {

               tracker.release(); 
               tracker = cv::TrackerKCF::create();
               tracker->init(frame, roi);
            }
        } else {

            ok = tracker->update(frame, roi);

            if (ok) {

                std::cout<< detectedPoint.pt.x << std::endl;
                std::cout<< detectedPoint.pt.y << std::endl;

                //Set process variable
                yawPVar=detectedPoint.pt.x;

            }
        }
        

        //Increment the frame counter
        frameCounter++;
        std::cout << "Counter: " << frameCounter << std::endl;

        #ifdef DISPLAY 
        cv::namedWindow("video", CV_WINDOW_AUTOSIZE);
        cv::resizeWindow("Final", 500,500);
        cv::imshow("video", frame);
        #endif

        int c = cv::waitKey(10);
        if ( (char)c == 'c' ) break; 

    }

    cv::destroyAllWindows();
    cap.release();

}
