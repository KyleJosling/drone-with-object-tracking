#include <msp/MSP.hpp>
#include <msp/msp_msg.hpp>
#include <msp/FlightController.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/tracking/tracker.hpp>

#include "Include/pid.h"

#include <iostream>

using namespace cv;
using namespace msp;

std::vector<Rect> DetectFace(Mat frame);
bool armFlightController();

 String face_cascade_name = "haarcascade_frontalface_default.xml";
 CascadeClassifier face_cascade;

int main(int argc, char** argv){

	//Declare device parameters
	const std::string device=(argc>1) ? std::string(argv[1]) :"/dev/ttyUSB0";
	const size_t baudrate = (argc>2) ? std::stoul(argv[2]) : 115200;

	//initialise flight controller
	// fcu::FlightController fcu(device,baudrate);
	// fcu.initialise();


	//Videocapture object
	VideoCapture cap(0);

	//if it fails return -1
	if(!cap.isOpened()) return -1;

	//Tracker object for face
	Ptr<TrackerKCF> tracker=TrackerKCF::create();

	//Declare PID controller
	//( double dt, double max, double min, double Kp, double Kd, double Ki );
	PID pid = PID(0.1,250,-250,0.001,0.01,0.5);

	//The set variable is half the width of the window
	int sVar=0;
	//The process variable is the location of the face in the frame
	int pVar;
	//Calculated output
	double pidOutput;

	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	int frameCounter=0;
	Mat frame;
	Rect2d roi;
	std::vector<Rect> rFaces;

  while(true)
  {
			//Get a frame
			cap >> frame;


			//Reset tracker every 100 frames
			if (frameCounter==0 || frameCounter>100){
				rFaces = DetectFace(frame);

				if (!rFaces.empty()){
				std::cout << "Reinit" << std::endl;
				roi=rFaces[0];
				tracker->init(frame,roi);
				frameCounter=1;
				}

			}

			//Update tracker and calculate PID control output
			else{

				//Update the tracker
				bool ok=tracker->update(frame,roi);

				//Draw circle around faces
				if (ok){
					std::cout <<"drawing"<<std::endl;
					//Set process variable
					pVar=(roi.x)-250;
					pidOutput=(pid.calculate(sVar,pVar))+1250;

					//Output
					std::cout <<" pVar: " << pVar << " output: " << pidOutput << std::endl;

					//Paint a picture
					Point center( roi.x + roi.width*0.5, roi.y + roi.height*0.5 );
					ellipse( frame, center, Size( roi.width*0.5, roi.height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

				}

				//Display
				namedWindow("video", CV_WINDOW_AUTOSIZE);
				resizeWindow("Final", 500,500);
				imshow("video", frame);

				//Increment the frame counter
				frameCounter++;
				std::cout << "COunter: " << frameCounter << std::endl;

			}


			int c = waitKey(10);
      if( (char)c == 'c' ) { break; }

  }
	return 0;
}

std::vector<Rect> DetectFace(Mat frame){

//Vector that contains the faces
std::vector <Rect> faces;
Mat frame_gray;

//Convert the colour to gray
cvtColor( frame, frame_gray, CV_BGR2GRAY );

//Raise the contrast
equalizeHist(frame_gray,frame_gray);

face_cascade.detectMultiScale(frame_gray,faces,1.1,2,0|CV_HAAR_SCALE_IMAGE,Size(30,30));
std::cout << "number of faces: " << faces.size() << std::endl;
	return faces;
}

//Function that arms the motors
bool armFlightController(fcu::FlightController fcu){

	//Arm the motors by setting yaw to 2000 for a few seconds
	bool rc = fcu.setRc(1500,1500,2000,1000);
	std::this_thread::sleep_for(std::chrono::seconds(3));
	rc = fcu.setRc(1500,1500,1500,1500);
	return fcu.isArmed();

}