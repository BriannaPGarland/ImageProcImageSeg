
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int mouseX = 0;
int mouseY = 0;

// function that allows functionality for the mouse
void handleMouseSelect(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN) 
	{
		mouseX = x;
		mouseY = y;
	}
}

// Edge Detection method 1 from the hw 
void edgeDetect1(int** image_out, int** image_in, int** image_edge, int height, int width)
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
				if (image_out[i][j] < 0)
					image_out[i][j] = 0;
				if (image_out[i][j] > 255)
					image_out[i][j] = 255;
		}
}

// Make selected color pixels negative at every instance ( has a range of color value of 20)
void shadeSelector(int** image_out, int** image_in,int height, int width) {
	for (int j = 0; j < height; j++)
		for (int k = 0; k < width; k++) {
			if (image_in[j][k] > image_in[mouseX][mouseY] - 10  && image_in[j][k] <= image_in[mouseX][mouseY] + 10  )
			{
				image_out[j][k] = 255 - image_in[j][k];	
			}
			else{
				image_out[j][k] = image_in[j][k];
			}
		}
}

void imageSegmentation (int** image_out, int** image_in, int height, int width) {
	int threshold = 50;
	int selectedValue = image_in[mouseX][mouseY];

	int i = mouseY;
	int j = mouseX;
	int nextX = selectedValue;
	int nextY = selectedValue;
	
	//-----------------------------------------------------------------------------------
	//------ 			First edge detection run  (width then height)				-----
	//-----------------------------------------------------------------------------------

	//Right - Bottom Quadrant
	//going down
	while (abs(selectedValue - nextX) < threshold && i < height) {
		
		image_out[i][j] = 255;
		nextX = image_in[i++][j];
		
		//going to the right
		while (abs(selectedValue - nextY) < threshold && j < width ){
			image_out[i][j] = 255;
			nextY = image_in[i][j++];
		}
		 j = mouseX;
		 nextY = selectedValue;
	}

	//resetting the values
	i = mouseY+1;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//Right - Top Quadrant
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

	//Left - Top  Quadrant
	//resetting the values
	i = mouseY+1;
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
	//Left - Bottom  Quadrant
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
	//------ 			2nd edge detection run  (Height then width)				-----
	//-----------------------------------------------------------------------------------
	
	i = mouseY;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;

	//Right - Bottom Quadrant
	//going down
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j++];

		//going to the right
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i++][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}
	
	//Right - Top Quadrant
	//resetting the values
	i = mouseY;
	j = mouseX;
	nextX = selectedValue;
	nextY = selectedValue;
	//going up 
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j++];

		//going to the left
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i--][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}

	//resetting the values
	i = mouseY;
	j = mouseX + 1;
	nextX = selectedValue;
	nextY = selectedValue;
	//Left Bottom  Quadrant
	//going up
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j--];

		//going to the right
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i++][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}

	//Left - Top  Quadrant
	//resetting the values
	i = mouseY;
	j = mouseX + 1;
	nextX = selectedValue;
	nextY = selectedValue;
	//going up
	while (abs(selectedValue - nextY) < threshold && j < width) {

		image_out[i][j] = 255;
		nextY = image_in[i][j--];

		//going to the left
		while (abs(selectedValue - nextX) < threshold && i < height) {
			image_out[i][j] = 255;
			nextX = image_in[i--][j];
		}
		i = mouseY;
		nextX = selectedValue;
	}
	
	
}



int main()
{
	int** image_in, ** image_out, ** image_edge;
	int j, i, width, height;
	Mat M_in;
	string inImage;
	string outImage;
	int action;
	
	cout << "Please enter the file name of the original image" << endl;
	cin >> inImage;
	cout << "Please enter the file name of what you want the output image to be named" << endl;
	cin >> outImage;
	cout << "What would you like to do with this image? Please Type the number of the action you want and hit enter \n ([1] Edge Detection / [2] Image Segment Selection / [3] Shade Selector )";
	cin >> action;

	M_in = imread(inImage);
	Mat_<uchar> M_in_g(M_in.rows, M_in.cols);
	cvtColor(M_in, M_in_g, COLOR_BGR2GRAY);

	String window1 = "Original Image";
	namedWindow(window1);
	imshow(window1, M_in_g);

	//set the callback function for any mouse event
	setMouseCallback("Original Image", handleMouseSelect, nullptr);

	cout << "Press Any Key To Continue" << endl;
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

cout << " Test: "<< image_in[mouseX][mouseY] << endl;
//Image Processing Begins

// Action Switch ******************************************************************
switch (action)
{
case (1): // Edge Detection
	edgeDetect1(image_out, image_in, image_edge, height, width);
	break;
case (2): // Image Segment Selection
	//loading the initial image out 
	for (int j = 0; j < height; j++)
		for (int k = 0; k < width; k++) 
				image_out[j][k] = image_in[j][k];	
	//actual image segmentation 
	imageSegmentation(image_out, image_in, height, width);
	break;
case (3): // Shade Selector 
	shadeSelector(image_out, image_in, height, width);
	break;
default:
	cout << "Sorry, that's not an option." << endl;
	return (1);
	break;
}
//	Image Processing Ends & Print Output Image

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



