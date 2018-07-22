#include <msp/MSP.hpp>
#include <msp/msp_msg.hpp>
#include <msp/msg_print.hpp>
#include <msp/FlightController.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include "Include/pid.h"

#include <iostream>

using namespace cv;
using namespace msp;

struct obj_point
{
    Point2f pt;
    int size;
};

obj_point detectObject(Mat frame, int hue, int sat, int val);

void armFlightController(fcu::FlightController *fcu);


int main(int argc, char** argv){

    obj_point detectedPoint;
    //Declare device parameters
    const std::string device=(argc>1) ? std::string(argv[1]) :"/dev/ttyUSB0";
    const size_t baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    //Initialise and arm flight controller
    fcu::FlightController fcu(device,baudrate);
    fcu.initialise();
    fcu.reboot();
    //armFlightController(&fcu);

    //Videocapture object
    VideoCapture cap(0);
    //if it fails return -1
    if(!cap.isOpened()) return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    //Declare PID controllers
    //( double dt, double max, double min, double Kp, double Kd, double Ki );
    //PID for yaw control
    PID yawPid = PID(0.1,500,-500,0.702,4.9,0.00006);
    PID pitchPid = PID(0.1,500,-500,0.702,4.9,0.00006);
    PID throttlePid = PID (0.1,500,-500,0.702,4.9,0.00006);

    //The set variable is half the width of the window
    int yawSVar=320;
    int throttleSVar=240;
    //The process variable is the location of the face in the frame
    int yawPVar=0;
    int throttlePVar=0;
    //Calculated output
    double yawOutput;
    double throttleOutput;

    int frameCounter=0;
    Mat frame;

  while(true)
  {
        //Get a frame
        cap >> frame;
        //~ int hue;
        //~ int sat;
        //~ int val;
        //~ std::cin >> hue;
        //~ std::cin >> sat;
        //~ std::cin >>val;
        detectedPoint=detectObject(frame,100,60,60);

        //Set process variable
        yawPVar=detectedPoint.pt.x;
        throttlePVar=detectedPoint.pt.y;
        if (yawPVar!=0){
            yawOutput=(yawPid.calculate(yawSVar,yawPVar)+1500);
        }
        else{
        yawOutput=1500;
        }
        if (throttlePVar!=0)
        {
            throttleOutput=(throttlePid.calculate(throttleSVar,throttlePVar)+1000);
        }
        else{
            throttleOutput=1200;
        }
        fcu.setRc(1500, 1500, yawOutput, 1200, 1000, 1000, 1000, 1000);
        //Output
        std::cout <<" yawPVar: " << yawPVar << " output: " << yawOutput << std::endl;
        std::cout <<" pitchPVar: " << pitchPVar << " output: " << pitchOutput << std::endl;

        //Increment the frame counter
        frameCounter++;
        std::cout << "Counter: " << frameCounter << std::endl;
        
        int c = waitKey(10);
    if( (char)c == 'c' ) { break; }

        }

    cv::destroyAllWindows();
    cap.release();
    return 0;
}

//Function that returns faces in frame
obj_point detectObject(Mat frame, int hue, int sat, int val){

    obj_point objectPoint;
    // Create object for eroding (2x2 rectangle)
    Mat erode_rect = getStructuringElement(MORPH_RECT,Size(2,2));
    // Create object for dilating (5x5 rectangle)
    Mat dilate_rect = getStructuringElement(MORPH_RECT,Size(5,5));

    Mat frame_hsv;
    Mat frame_threshold;

    //Convert the colour to hsv
    cvtColor( frame, frame_hsv, CV_BGR2HSV );

    //Mask
    cv::inRange(frame_hsv,Scalar(hue-7,sat,val),Scalar(hue+7,255,255),frame_threshold);

    cv::erode(frame_threshold, frame_threshold, erode_rect,Point(-1,-1),2);
    cv::dilate(frame_threshold, frame_threshold, dilate_rect, Point(-1,-1), 2);

    //Display
    namedWindow("video", CV_WINDOW_AUTOSIZE);
    resizeWindow("Final", 500,500);
    imshow("video", frame_threshold);

    //Find the contours
    std::vector<std::vector<Point>> contours;
    std::vector<Vec4i> hierarchy;

    findContours(frame_threshold, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    int contour_no=0, largest_contour_no, largest_contour_area=0;

    // Iterate through the contours
    for(contour_no; contour_no < contours.size(); contour_no++)
    {
        // Find the largest contour
        int current_area = contourArea(contours[contour_no]);std::vector<std::vector<Point>> contours;
        std::vector<Vec4i> hierarchy;

        if(current_area > largest_contour_area)
        {
            largest_contour_area = current_area;
            largest_contour_no = contour_no;
        }
    }

    // If no contours found then mean point is 0
    if(largest_contour_area ==0 || largest_contour_area<800)
    {
        objectPoint.pt= Point2f(0,0);
        objectPoint.size=0;
    }

    else
    {
        // Find moments of largest contour
        Moments obj_momts = moments(contours[largest_contour_no], false);
        // Calculate moment centre
        objectPoint.pt= Point2f(obj_momts.m10 / obj_momts.m00, obj_momts.m01 / obj_momts.m00);
        objectPoint.size=largest_contour_area;

    }

    return objectPoint;
}

//Function that arms the motors
void armFlightController(fcu::FlightController *fcu){

    const uint16_t yaw = 2000;
    while(fcu->isArmed()==false)
    {
        std::cout << "not ready 1" << std::endl;
        fcu->setRc(1500, 1500, yaw, 1010, 1000, 1000, 1000, 1000);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}
