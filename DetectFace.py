import numpy as np
import cv2


#In the first part of the code we train the face cascade with the xml file.
face_cascade= cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

#Start capturing video
cap=cv2.VideoCapture(0)



while(True):

    #These return a true/false if the frame is read correctly
    ret,frame=cap.read()

    #Convert the frame to grayscale so we can analyze it using a Haar cascade
    gray=cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)


    #Now, find faces in the image


    #detectMultiScale detects the face and takes parameters: (img, continue

    #Faces are returned as a list of rectangles: Rect(x,y,w,h)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    #loop test

    #Loops for every rectangle 
    for(x,y,w,h) in faces:


        #Draw a box around the face.
        frame=cv2.rectangle(frame,(x,y),(x+w,y+h), (255,0,0),2)

    #Display face + rectangle
    cv2.imshow('frame',frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


#Release the capture, destroy windows.
cap.release()
cv2.destroyAllWindows()



