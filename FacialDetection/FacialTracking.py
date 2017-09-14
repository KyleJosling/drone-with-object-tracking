import numpy as np
import cv2
import sys
from DetectFace import Detect

def main():

    # Create tracker, using KCF
    faceTracker = cv2.Tracker_create("KCF")
    leftTracker=cv2.Tracker_create("KCF")
    rightTracker=cv2.Tracker_create("KCF")

    cap = cv2.VideoCapture(0)

    faceFound = False
    rightEyeFound = False
    leftEyeFound = False

    while (faceFound == False and rightEyeFound == False and leftEyeFound == False):
        ret, frame = cap.read()
        # Detect face
        faces, rightEye, leftEye = Detect(frame)
        print faces
        print rightEye
        print leftEye
        if len(faces):
            # Define initial ROI as the face in the picture
            InitROI = tuple(faces[0])
            faceFound = True
        if len(rightEye):
            # Define initial ROI of right eye
            rightEyeInitROI = tuple(rightEye[0])
            rightEyeFound = True
        if len(leftEye):
            # Define initial ROI of left eye
            leftEyeInitROI = tuple(leftEye[0])
            leftEyeFound = True

    # Initialize the face tracker with the first frame and the ROI
    faceOk = faceTracker.init(frame, InitROI)
    # Initialize the right eye tracker
    rightEyeOk = rightTracker.init(frame, rightEyeInitROI)
    # Initialize the left eye tracker
    leftEyeOk = leftTracker.init(frame, leftEyeInitROI)

    # count the frames
    frameCounter = 1

    while(True):

        # Read a new frame

        ret, frame = cap.read()

        if (frameCounter > 99):
            # Do detection every 100 frames
            faces, rightEye, leftEye = Detect(frame)
            print "Faces: " + str(faces)
            print "Right Eye: " + str(rightEye)
            print "Left Eye: " + str(leftEye)

            if (len(faces) and len(rightEye) and len(leftEye)):
                newROI = tuple(faces[0])
                frameCounter = 0
                # Clear tracker, reinitialize
                faceTracker.clear()
                print "Tracker cleared"
                faceTracker = cv2.Tracker_create("KCF")
                faceOk = faceTracker.init(frame, newROI)
                print faceOk
                print "Tracker reinitialized"

            else:
                faceOk, newROI = faceTracker.update(frame)

        else:
            # Update tracker
            print "Tracker updated"
            faceOk, newROI = faceTracker.update(frame)
            rightOk,newRightROI=rightTracker.update(frame)
            leftOk,newLeftROI=leftTracker.update(frame)


#Update the variables here.
#Draw rectangles.
        if faceOk:

            p1 = (int(newROI[0]), int(newROI[1]))
            p2 = (int(newROI[0] + newROI[2]), int(newROI[1] + newROI[3]))
            cv2.rectangle(frame, p1, p2, (0, 0, 255))

        if leftOk:

            p3 = (int(newLeftROI[0]), int(newLeftROI[1]))
            p4 = (int(newLeftROI[0] + newLeftROI[2]), int(newLeftROI[1] + newLeftROI[3]))
            cv2.rectangle(frame, p3, p4, (0, 0, 255))

        if rightOk:

            p5 = (int(newRightROI[0]), int(newRightROI[1]))
            p6 = (int(newRightROI[0] + newRightROI[2]), int(newRightROI[1] + newRightROI[3]))
            print type(p5)
            cv2.rectangle(frame, p5, p6, (0, 0, 255))


        cv2.imshow("Face Tracking", frame)

        # Increment frame counter
        frameCounter += 1
        print "Frame number: " + str(frameCounter)

        k = cv2.waitKey(1) & 0xff
        if k == 27:
            break
            # Release the capture, destroy windows.
            cap.release()
            cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
