#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

int main(int argc, char** argv){

    obj_point detectedPoint;
    //Declare device parameters
    const std::string device=(argc>1) ? std::string(argv[1]) :"/dev/ttyUSB0";
    const size_t baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

    //Initialise and arm flight controller
    fcu::FlightController fcu(device,baudrate);
    fcu.initialise();
    fcu.reboot();
    armFlightController(&fcu);

    //Videocapture object
    VideoCapture cap(0);
    //if it fails return -1
    if(!cap.isOpened()) return -1;
    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    //Declare PID controller
    //( double dt, double max, double min, double Kp, double Kd, double Ki );
    PID yawPid = PID(0.1,500,-500,0.702,4.9,0.00006);

    //The set variable is half the width of the window
    int sVar=320;
    //The process variable is the location of the face in the frame
    int pVar=0;
    //Calculated output
    double yawOutput;

    int frameCounter=0;
    Mat frame;
