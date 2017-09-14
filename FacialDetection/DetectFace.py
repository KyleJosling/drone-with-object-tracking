import numpy as np
import cv2
#Load files for eye and face detection
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')


# This function detects eyes + face and returns rectangles
def Detect(frame):

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


            # print "rEye Found! : " + str(rEye)
            # print "lEye Found! : " + str(lEye)
        return faceRects

    else:
        return [], [], []
