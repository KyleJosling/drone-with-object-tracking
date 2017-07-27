import numpy as np
import cv2
import sys


# This function detects eyes + face and returns rectangles
def Detect(frame):
    # Load files for eye and face detection
    # Maybe gonna load these at beggining of program idk yet
    face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
    eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')

    # Add an error in here just in case no face detected**
    # Convert to grayscale
    grayFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Find faces in image
    faceRects = face_cascade.detectMultiScale(grayFrame, 1.3, 5)

    # Add a check here to see if there is anything in faces
    if len(faceRects):
        for (x, y, w, h) in faceRects:

            # Set right eye ROI
            rightGrayROI = grayFrame[y:y + h, x:x + (w / 2)]
            rightColourROI = frame[y:y + h, x:x + (w / 2)]
            rEye = eye_cascade.detectMultiScale(rightGrayROI)

            # Set left eye ROI
            leftGrayROI = grayFrame[y:y + h, (x + (w / 2)):x + w]
            leftColourROI = frame[y:y + h, (x + (w / 2)):x + w]
            lEye = eye_cascade.detectMultiScale(leftGrayROI)

        return faceRects, rEye, lEye

    else:
        return [], [], []


def main():

    # Create tracker, using KCF
    tracker = cv2.Tracker_create("KCF")

    cap = cv2.VideoCapture(0)

    faceFound = False
    rightEyeFound = False
    leftEyeFound = False

    while (faceFound == False and rightEyeFound == False and leftEyeFound == False):
        ret, frame = cap.read()
        # Detect face
        faces, rightEye, leftEye = Detect(frame)
        print faces
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
    faceOk = tracker.init(frame, InitROI)
    # Initialize the right eye tracker
    rightEyeOk = tracker.init(frame, rightEyeInitROI)
    # Initialize the left eye tracker
    leftEyeOk = tracker.init(frame, leftEyeInitROI)

    # count the frames
    frameCounter = 1

    while(True):

        # Read a new frame

        ret, frame = cap.read()

        if (frameCounter > 99):
            # Do detection every 100 frames
            faces, rightEye, leftEye = Detect(frame)
            print type(faces)

            if len(faces):
                newROI = tuple(faces[0])
                frameCounter = 0
                # Clear tracker, reinitialize
                tracker.clear()
                print "Tracker cleared"
                tracker = cv2.Tracker_create("KCF")
                faceOk = tracker.init(frame, newROI)
                print faceOk
                print "Tracker reinitialized"

            else:
                faceOk, newROI = tracker.update(frame)

        else:
            # Update tracker
            faceOk, newROI = tracker.update(frame)

        if faceOk:

            p1 = (int(newROI[0]), int(newROI[1]))
            p2 = (int(newROI[0] + newROI[2]), int(newROI[1] + newROI[3]))
            cv2.rectangle(frame, p1, p2, (0, 0, 255))

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
