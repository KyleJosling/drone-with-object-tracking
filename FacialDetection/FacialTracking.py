import numpy as np
import cv2
import sys


def main():

    #Create tracker, using KCF
    tracker =cv2.Tracker_create("KCF")

    #We capture the first frame in the video
    cap=cv2.VideoCapture(0)

    ret,frame=cap.read()

    #Define initial ROI as the whole frame
    InitROI=cv2.selectROI(frame,False)

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
        if k == 27 : break

if __name__=='__main__':
    main()
