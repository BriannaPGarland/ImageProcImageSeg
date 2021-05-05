/*How to use if using visual studio on windows:
	1. ensure that the image you wish to process is in the SAME folder as the .exe file and that it is a .jpg or .jpeg
	2. Then esure you build the project in visual studio
	3. Using the cmd line of your choosing cd into the file location where the exe is
	3. Then Run the command: ./462FinalProject
	4. You will then be asked to enter the file name of the image you put in that folder, BE SURE to include the file type at the end (.jpg/.jpeg)
	5. Then enter the name of what you want the output image to be named, again INCLUDE the file type (can be anything you want it to be)
	6. Then you will be asked to select which imag processing process you would like to take part in, enter the number of which one and hit enter
		Edge Detection(1): after you type 1 and hit enter in the previous step, a black and white version of your image will apear, then click 
				anywhere on the image and hit any key to continue, this will bring up the image processed edge detection image and will prompt you input
				a threshold, then click any key to close the window.
		Image Segment Selector(2) (THE MAIN PROGRAM ~ End Result): after you type 2 and hit enter in the previous step, a black and white version of your
									   image will apear, Then with your mouse, click on the part of the image you would like to
									   select and turn white. Then hit a key to continue. This will prompt you to input a threshold and then display an image 
									   with just the part of the image you selected whited out. Press any key to close the window
		Shade Selector (3) (an intermediate step): after you type 2 and hit enter in the previous step, a black and white version of your image will apear,
							   Then with your mouse, click on the part of the image you would like to select every instance or shade of
							   the color you clicked on. Then press any key, then an image where every instance of the color you selected
							   will now be white. Press any key to close the window.
*/

//Libraries 
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

//Global Variables
using namespace std;
using namespace cv;
int mouseX = 0;
int mouseY = 0;

//This function allows the user to click on the screen and assigns the value of where in the image there was a mouse click 
void handleMouseSelect(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		mouseX = x;
		mouseY = y;
	}
}

//--------------------------------------------------------------------------------------------------------------------------	
//------ 				Different image processing methods for different actions         	       -----
//--------------------------------------------------------------------------------------------------------------------------

//**************************************** Edge Detection method 1 from the hw **********************************************

void edgeDetect1(int** image_out, int** image_in, int** image_edge, int height, int width, int threshold)
{

	int prewitt1[3][3] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int prewitt2[3][3] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

	for (int i = 1; i < height - 1; i++)
		for (int j = 1; j < width - 1; j++) {

			for (int fy = 0; fy < 3; fy++)
				for (int fx = 0; fx < 3; fx++) {
					image_out[i][j] = image_out[i][j] + image_in[i - 1 + fy][j - 1 + fx] * prewitt1[fy][fx];
					image_edge[i][j] = image_edge[i][j] + image_in[i - 1 + fy][j - 1 + fx] * prewitt2[fy][fx];
				}

		}

	for (int i = 1; i < height - 1; i++)
		for (int j = 1; j < width - 1; j++) {
			image_out[i][j] = abs(image_out[i][j]) + abs(image_edge[i][j]);
			if (image_out[i][j] < threshold)
				image_out[i][j] = 0;
			if (image_out[i][j] > threshold)
				image_out[i][j] = 255;
		}
}
//****************************************************************************************************************************

//***************** Make selected color pixels negative at every instance ( has a range of color value of 20)*****************
void shadeSelector(int** image_out, int** image_in, int height, int width) {
	for (int j = 0; j < height; j++)
		for (int k = 0; k < width; k++) {
			if (image_in[j][k] > image_in[mouseX][mouseY] - 10 && image_in[j][k] <= image_in[mouseX][mouseY] + 10)
			{
				image_out[j][k] = 255 - image_in[j][k];
			}
			else {
				image_out[j][k] = image_in[j][k];
			}
		}
}

//****************************************************************************************************************************

//******** Image Segment Selection Method (Click the image and select only that segment of the image and turn it white *******
void imageSegmentation(int** image_out, int** image_in, int height, int width, int InputThreshold) {
	int threshold = InputThreshold;
	int selectedValue = image_in[mouseX][mouseY];

	int i = mouseY;
	int j = mouseX;
	int nextX = selectedValue;
	int nextY = selectedValue;

	//-----------------------------------------------------------------------------------
	//------ 		First edge detection run  (width then height)		-----
	//-----------------------------------------------------------------------------------

	//~~ Right - Bottom Quadrant:  Going down and then too the right until edge ~~ 
	//going down
	while (abs(selectedValue - nextX) < threshold && i < height) {

		image_out[i][j] = 255;
		nextX = image_in[i++][j];

		//going to the right
		while (abs(selectedValue - nextY) < threshold && j < width) {
			image_out[i][j] = 255;
			nextY = image_in[i][j++];
		}
		j = mouseX;
		nextY = selectedValue;
	}

	//~~ Right - Top Quadrant: Going up and then too the right until edge ~~
	//resetting the values
	i = mouseY + 1;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//going up
	while (abs(selectedValue - nextX) < threshold && i < height) {

		image_out[i][j] = 255;
		nextX = image_in[i--][j];

		//going to the right
		while (abs(selectedValue - nextY) < threshold && j < width) {
			image_out[i][j] = 255;
			nextY = image_in[i][j++];
		}
		j = mouseX;
		nextY = selectedValue;
	}

	//~~ Left - Top Quadrant: Going up and then too the left until edge ~~
	//resetting the values
	i = mouseY + 1;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//going up
	while (abs(selectedValue - nextX) < threshold && i < height) {

		image_out[i][j] = 255;
		nextX = image_in[i--][j];

		//going to the left
		while (abs(selectedValue - nextY) < threshold && j < width) {
			image_out[i][j] = 255;
			nextY = image_in[i][j--];
		}
		j = mouseX;
		nextY = selectedValue;
	}

	//~~ Left - Bottom  Quadrant: Going down and then too the left until edge ~~
	//resetting the values
	i = mouseY;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//going down
	while (abs(selectedValue - nextX) < threshold && i < height) {

		image_out[i][j] = 255;
		nextX = image_in[i++][j];

		//going to the left
		while (abs(selectedValue - nextY) < threshold && j < width) {
			image_out[i][j] = 255;
			nextY = image_in[i][j--];
		}
		j = mouseX;
		nextY = selectedValue;
	}



	//-----------------------------------------------------------------------------------
	//------ 			2nd edge detection run  (Height then width)	-----
	//-----------------------------------------------------------------------------------

	//Right - Bottom Quadrant: Going to the right and then down until edge ~~
	i = mouseY;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//going right
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j++];

		//going to the down
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i++][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}

	//Right - Top Quadrant: Going to the right and then up until edge ~~
	//resetting the values
	i = mouseY;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//going right 
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j++];

		//going to the up
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i--][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}

	//~~ Left - Bottom Quadrant: Going to the left and then down until edge ~~
	//resetting the values
	i = mouseY;
	j = mouseX + 1;
	nextX = selectedValue;
	nextY = selectedValue;
	//going left
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j--];

		//going to the down
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i++][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}

	//~~ Left - Top  Quadrant: Going to the left and then up until edge ~~
	//resetting the values
	i = mouseY;
	j = mouseX + 1;
	nextX = selectedValue;
	nextY = selectedValue;
	//going left
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j--];

		//going to the up
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i--][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}
}
//****************************************************************************************************************************



int main()
{
	int** image_in, ** image_out, ** image_edge;
	int j, i, width, height, inputThreshold;
	Mat M_in;
	string inImage;
	string outImage;
	int action;

	//user input for file name and which image processing action they would like to perform 
	cout << "Please enter the file name of the original image" << endl;
	cin >> inImage;
	cout << "Please enter the file name of what you want the output image to be named" << endl;
	cin >> outImage;
	cout << "What would you like to do with this image? Please Type the number of the action you want and hit enter \n ([1] Edge Detection / [2] Image Segment Selection / [3] Shade Selector )";
	cin >> action;

	//**************************************** Allocating space in Memory for the image **********************************************
	M_in = imread(inImage);
	Mat_<uchar> M_in_g(M_in.rows, M_in.cols);
	cvtColor(M_in, M_in_g, COLOR_BGR2GRAY);

	String window1 = "Original Image";
	namedWindow(window1);
	imshow(window1, M_in_g);

	//set the callback function for any mouse event
	setMouseCallback("Original Image", handleMouseSelect, nullptr);

	cout << "Click Anywhere On The Image And Press Any Key To Continue" << endl;
	waitKey(0);
	destroyWindow(window1);

	height = M_in_g.rows;
	width = M_in_g.cols;

	image_in = (int**)calloc(height, sizeof(int*));
	if (!image_in) {
		printf("Error: Can't allocate memory.\n");
		return (1);
	}

	image_out = (int**)calloc(height, sizeof(int*));
	if (!image_out) {
		printf("Error: Can't allocate memory.\n");
		return (1);
	}
	//****************************************************
	image_edge = (int**)calloc(height, sizeof(int*));
	if (!image_edge) {
		printf("Error: Can't allocate memory.\n");
		return (1);
	}
	//****************************************************
	for (j = 0; j < height; j++) {
		image_in[j] = (int*)calloc(width, sizeof(int));
		if (!image_in[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}

		image_out[j] = (int*)calloc(width, sizeof(int));
		if (!image_out[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}
		//****************************************************
		image_edge[j] = (int*)calloc(width, sizeof(int));
		if (!image_edge[j])
		{
			printf("Error: Can't allocate memmory!\n");
			return(1);
		}
		//****************************************************

	}
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			image_in[i][j] = M_in_g(i, j);
	//****************************************************************************************************************************

////************************************************* Image Processing Begins ********************************************************

// ~~ Action Switch statement that controls the logic of which image processing process takes place ~~ 
	switch (action)
	{
	case (1): // Edge Detection
		cout << "Input your threshold " << endl;
		cin >> inputThreshold;
		edgeDetect1(image_out, image_in, image_edge, height, width, inputThreshold);

		break;
	case (2): // Image Segment Selection
		//loading the initial image out 
		for (int j = 0; j < height; j++)
			for (int k = 0; k < width; k++)
				image_out[j][k] = image_in[j][k];
		//actual image segmentation 
		cout << "Input your threshold " << endl;
		cin >> inputThreshold;
		imageSegmentation(image_out, image_in, height, width, inputThreshold);

		break;
	case (3): // Shade Selector 
		shadeSelector(image_out, image_in, height, width);
		break;
	default:
		cout << "Sorry, that's not an option." << endl;
		return (1);
		break;
	}
	//**************************************** Image Processing Ends & Print Output Image **********************************************	

	Mat_<uchar> M_out(height, width);
	for (int ii = 0; ii < height; ii++)
		for (int jj = 0; jj < width; jj++)
			M_out(ii, jj) = image_out[ii][jj];

	String window2 = "Output Image";
	namedWindow(window2);
	imshow(window2, M_out);
	waitKey(0);
	destroyWindow(window2);

	bool success = imwrite(outImage, M_out);
	if (!success) {
		cout << "Failed to save the image" << endl;
		return -1;
	}

	for (j = 0; j < height; j++) {
		free(image_in[j]);
		free(image_out[j]);
	}
	free(image_in);
	free(image_out);


	return 0;
}


