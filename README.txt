
Developed a vision system in C++ that recognized 2D objects in an image and then computed their position and orientations

image.* : Image class (2-D array of integers, along with size, number of colors)
                      (For our purposes the number of colors is 256)

image_demo.cc : Sample main() function for testing.

image.cc: Create four main function named, 
p1 is used to covert the gray scale image to binary image thershold for the image is 128, 
p2 is labeling each objects using the sequential labeling algorithm, 
p3 finding the center of the objects in the x and y axis, minimum moment of inertia and orientation of the objects. 
p4 is comparing two_objects.pgm to many_objects_1_.pgm. 
	Criteria Of compairing: finding the difference of mimimum momemt of inertia between all the
	objects of two_objects.pgm anf many_objects_1_.pgm, the getting the minmium of all and 		drawing a line from the center for that object of many_objects_1_.pgm image.   

----------------------
To compile in Linux:
----------
 
   make all

To run:
---------

To run p1:

./p1 two_objects.pgm two_objects_outputp1.pgm
This will open two_objects.pgm and change it into a binary image and save it in two_objects_outputp1.pgm

To run p2:

./p2 two_objects_outputp1.pgm two_objects_outputp2.pgm
This will open two_objects.pgm and labels each object, gives them different grayscale color and save it in two_objects_outputp2.pgm

To run p3:

./p3 two_objects_outputp2.pgm file_two_objects.txt two_objects_outputp2.pgm
This will open two_objects_outputp2.pgm and fins mimimum momemt of interia, center in x and y axis, orientation of each object in the image and saves it in two_objects.txt text file and outputs it in two_objects_outputp3.pgm

To run p4:

./p4 many_objects_1_outputp2.pgm file_two_objects.txt many_objects_1_outputp4.pgm
This will open many_objects_1_outputp2.pgm find each objects mimium of inertia, center in x and y axis, orientation and compares each of the objeccts information with each of objects information of two_objects image provided in file_two_objects.txt and draws a line on the object of many_objects image for the object that showed minimum difference.


Note: inorder to create many_objects_outputp2 we need to create a binary image of many_objects_1.pgm using the p1.

-----------

To view .pgm files you can use the open source program gimp:

https://www.gimp.org/



