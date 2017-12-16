import numpy as np 
import cv2 

x = 0

cap1 = cv2.VideoCapture(0)	#Camera 1
cap2 = cv2.VideoCapture(1)	#Camera 2 

imwidth = 0
imheight = 0

while(1):
	#Capturing frame by frame 
	ret, img1 = cap1.read() 	#Capture frame by frame from Camera 1
	ret, img2 = cap2.read() 	#Capture frame by frame from Camera 2 

	r1 = imwidth / img1.shape[1] 	#Dividing by the width 
	ratio1 = ( imwidth, int(img1.shape[0] * r1) )	# multiplying by the height/width 
	
	r2 = imwidth / img2.shape[1]
	ratio2 = ( imwidth, int(img2.shape[1] * r2) ) 	

	img_res1 = cv2.resize( img1, ratio1, interpolation = cv2.INTER_AREA ) #INTER_AREA optimum for scaling down 
	img_res2 = cv2.resize( img2, ratio2, interpolation = cv2.INTER_AREA ) 

	cv2.imshow( "IMG1", img_res1 ) 	
	cv2.imshow( "IMG2", img_res2 ) 

	if (cv.waitKey(30) > 0): 
		x += 1
		print "Saving img pair " + x

		filename1 = "%sleft%d.%s" % ( imgs_directory, x, extension ) 
		filename2 = "%sright%d.%s" % ( imgs_directory, x, extension ) 

		cv2.imwrite( filename1, im_res1 ) 
		cv2.imwrite( filename2, im_res2 )
