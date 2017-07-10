import numpy as np
import cv2
import sys




#This function detects eyes + face and returns rectangles
def Detect(frame):
    #Load files for eye and face detection
    #Maybe gonna load these at beggining of program idk yet
    face_cascade= cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
    eye_cascade= cv2.CascadeClassifier('haarcascade_eye.xml')

    #Add an error in here just in case no face detected**
    #Convert to grayscale
    grayFrame=cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)

    #Find faces in image
    faces=face_cascade.detectMultiScale(grayFrame,1.3,5)

    return faces


def main():

    #Create tracker, using KCF
    tracker =cv2.Tracker_create("KCF")

    #We capture the first frame in the video
    cap=cv2.VideoCapture(0)

    ret,frame=cap.read()

    #Detect face
    faces=Detect(frame)

    #Define initial ROI as the face in the picture
    InitROI=tuple(faces[0])

    #Initialize the tracker with the first frame and the ROI
    ret=tracker.init(frame,InitROI)


    while(True):
        #Read a new frame

        ret,frame=cap.read()

        #Update tracker
        ret,newROI=tracker.update(frame)

        if ret:
            p1 = (int(newROI[0]), int(newROI[1]))
            p2 = (int(newROI[0] + newROI[2]), int(newROI[1] + newROI[3]))
            cv2.rectangle(frame, p1, p2, (0,0,255))


        cv2.imshow("Face Tracking", frame)


        k = cv2.waitKey(1) & 0xff
        if k == 27 :
            break
            #Release the capture, destroy windows.
            cap.release()
            cv2.destroyAllWindows()





if __name__=='__main__':
    main()
