#include <msp/MSP.hpp>
#include <msp/msp_msg.hpp>
#include <msp/msg_print.hpp>
#include <msp/FlightController.hpp>

#include <iostream>
#include <sys/stat.h>

#include "fc/fc.hpp"

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
