#include <msp/MSP.hpp>
#include <msp/msp_msg.hpp>
#include <msp/msg_print.hpp>
#include <msp/FlightController.hpp>

#include "detectObject/detectObject.hpp"
#include "imageHandler/imageHandler.hpp"
#include "fc/fc.hpp"

#include "pid.h"
#include "globals.hpp"

#include <iostream>
#include <thread>


// Global process variables
int yawPVar = 0;

int main(int argc, char** argv){
    
    //Declare device parameters
    const std::string device = "/dev/ttyACM0";
    const size_t baudrate = 115200;

    //Declare PID controllers
    //( double dt, double max, double min, double Kp, double Kd, double Ki );
    //PID for yaw control
    PID yawPid = PID(0.1, 500, -500, 0.702, 4.9, 0.00006);

    //The set variable is half the width of the window
    int yawSVar = 320;
    int throttleSVar = 240;

    //Calculated output
    double yawOutput;
    double throttleOutput;
    
    #ifdef FLIGHT_CONTROLLER
    //Initialise and arm flight controller
    if (!(findController(device))) {
        std::cout << "No flight controller at " << device << "exists." << std::endl;
        return 1;
    }
    fcu::FlightController fcu(device, baudrate);
    fcu.initialise();
    armFlightController(&fcu);
    #endif
    
    // 
    std::thread detector(imageHandler); 
    detector.detach();
    
    // ----------- MAIN LOOP --------- // 
    while (true) {

        if (yawPVar != 0) {
            // yawOutput=(yawPid.calculate(yawSVar,yawPVar)+1500);
        } else {
            yawOutput = 1500;
        }
       
        #ifdef FLIGHT_CONTROLLER
        fcu.setRc(1500, 1500, 1500, 1400, 2000, 1000, 1000, 1000);

        // Check if FCU is still armed
        if (!fcu.isArmed()) {
            std::cout << "No longer armed" << std::endl;
        }
        #endif
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
