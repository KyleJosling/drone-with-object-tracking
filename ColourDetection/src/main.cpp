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

using namespace cv;
using namespace msp;

void armFlightController(fcu::FlightController *fcu);

int main(int argc, char** argv){
    
    obj_point detectedPoint;
    //Declare device parameters
    const std::string device=(argc>1) ? std::string(argv[1]) :"/dev/ttyACM0";
    const size_t baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    //Initialise and arm flight controller
    fcu::FlightController fcu(device,baudrate);
    fcu.initialise();

    // if (fcu.isFirmwareCleanflight()) {
    //         std::cout << "Boepity boopity" << std::endl; 
    //     if (fcu.enableRxMSP()==1) {
    //     }
    // }

    armFlightController(&fcu);

    //Videocapture object
    VideoCapture cap(0);
    //if it fails return -1
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
    // PID pitchPid = PID(0.1,500,-500,0.702,4.9,0.00006);
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
        // std::cout<< detectedPoint.pt.x << std::endl;
        // std::cout<< detectedPoint.pt.y << std::endl;
        //Set process variable
        // yawPVar=detectedPoint.pt.x;
        // throttlePVar=detectedPoint.pt.y;
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
        // std::cout <<" yawPVar: " << yawPVar << " output: " << yawOutput << std::endl;
        // std::cout <<" pitchPVar: " << pitchPVar << " output: " << pitchOutput << std::endl;

        //Increment the frame counter
        frameCounter++;
        std::cout << "Counter: " << frameCounter << std::endl;

        int c = waitKey(10);
        if( (char)c == 'c' ) { break; }
         
         }
 
    // cv::destroyAllWindows();
    cap.release();
    return 0;
}


//Function that arms the motors
void armFlightController(fcu::FlightController *fcu){

    const uint16_t aux1 = 2000;
    while(fcu->isArmed()==false)
    {
        std::cout << "Attempting to arm" << std::endl;
        fcu->setRc(1500, 1500, 1500, 1000, aux1, 2000, 2000, 2000);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}
