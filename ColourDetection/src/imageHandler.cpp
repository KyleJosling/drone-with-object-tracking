#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracking.hpp>

#include <iostream>
#include <ctime>

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

    cap.set(CV_CAP_PROP_FRAME_WIDTH,352);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,288);

    cv::Mat frame;
    cv::Rect2d roi;
    int frameCounter = 0;
    bool ok;

    // Frame loop
    while (true) {

        // Measure loop rate 
        std::clock_t begin = std::clock();

        //Get a frame
        cap >> frame;
        
        // Flip to hsv every frame
        frame = processImg(frame, 145, 50, 65);

        if (frameCounter == 0 || frameCounter > 10) {

            // Detect the object in the frame (hard coded values for now)
            roi = detectObject(frame);
            
            // Reinitialize tracker
            if (roi.height > 0) {
               std::cout << " Somethin detected " << std::endl;
               tracker.release(); 
               tracker = cv::TrackerKCF::create();
               tracker->init(frame, roi);
               frameCounter = 1;
            }
        } else {
 
             ok = tracker->update(frame, roi);
 
             if (ok) {
 
                 //Set process variable
                 yawPVar=roi.x + (roi.width/2);
 
             } else {
                 std::cout << "Not ok" << std::endl;
             }
         }
        

        //Increment the frame counter
        frameCounter++;
        std::cout << "Counter: " << frameCounter << std::endl;

        // Measure loop rate 
        std::clock_t end = std::clock();
        std::cout << (1/(double(end-begin)/CLOCKS_PER_SEC)) << std::endl;


        #ifdef DISPLAY 
        cv::rectangle(frame, roi.tl(), roi.br(), cv::Scalar(255,0,0));
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
