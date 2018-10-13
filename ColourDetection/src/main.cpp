#include <msp/MSP.hpp>
#include <msp/msp_msg.hpp>
#include <msp/msg_print.hpp>
#include <msp/FlightController.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include "detectObject/detectObject.hpp"

#include "pid.h"

#include <iostream>
#include <sys/stat.h>

#define NODISPLAY
#define FLIGHT_CONTROLLER

using namespace msp;

void armFlightController(fcu::FlightController *fcu);

bool findController(const std::string name);

int main(int argc, char** argv){

    //Declare device parameters
    
    const std::string device=(argc>1) ? std::string(argv[1]) :"/dev/ttyACM0";
    const size_t baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    #ifdef FLIGHT_CONTROLLER
    
    // Check if the controller exists
    if (!findController(device)) {
        std::cout << "Flight controller at " << device << " does not exist. Is the flight controller plugged in?" << std::endl;
        return 1;
    }

    //Initialise and arm flight controller
    fcu::FlightController fcu(device, baudrate);
    fcu.initialise();
    armFlightController(&fcu);

    #endif


    obj_point detectedPoint;

    // Open video capture
    cv::VideoCapture cap(0);

    if(!cap.isOpened()){
        std::cout << "Fail" << std::endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    //Declare PID controllers
    //( double dt, double max, double min, double Kp, double Kd, double Ki );
    //PID for yaw control
    PID yawPid = PID(0.1,500,-500,0.702,4.9,0.00006);

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

        if (yawPVar!=0) {
            yawOutput=(yawPid.calculate(yawSVar,yawPVar)+1500);
        } else {
            yawOutput=1500;
        }
       
        #ifdef FLIGHT_CONTROLLER
        fcu.setRc(1500, 1500, 1500, 1400, 2000, 1000, 1000, 1000);

        // Check if FCU is still armed
        if (!fcu.isArmed()) {
            std::cout << "No longer armed" << std::endl;
        }
        #endif

        //Output
        std::cout <<" yawPVar: " << yawPVar << " output: " << yawOutput << std::endl;

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
    return 0;
}


//Function that arms the motors
void armFlightController(fcu::FlightController *fcu){

    const uint16_t aux1 = 2000;
    while(fcu->isArmed()==false)
    {
        std::cout << "Attempting to arm" << std::endl;
        fcu->setRc(1500, 1500, 1500, 1100, aux1, 1000, 1000, 1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}

bool findController(const std::string name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
