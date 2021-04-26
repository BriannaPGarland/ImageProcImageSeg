/**********************************************************************************
* imageproc.c
* Usage: imageproc in_file_name out_file_name width height
 **********************************************************************************/


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
	if (event == EVENT_LBUTTONDOWN) {
		mouseX = x;
		mouseY = y;
	}
}

int main()
{
	int** image_in, ** image_out;
	int j, k, width, height;
	Mat M_in;
	string inImage;
	string outImage;
	
	cout << "Please enter the file name of the original image" << endl;
	cin >> inImage;
	cout << "Please enter the file name of what you want the output image to be named" << endl;
	cin >> outImage;
	
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

	}

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			image_in[i][j] = M_in_g(i, j);

cout << " Test: "<< image_in[mouseX][mouseY] << endl;
//Image Processing Begins

//take the value from click 
	for (j = 0; j < height; j++)

		for (k = 0; k < width; k++) {
			if (image_in[j][k] > image_in[mouseX][mouseY] - 10  && image_in[j][k] <= image_in[mouseX][mouseY] + 10  )
			{
	
				image_out[j][k] = 255 - image_in[j][k];
				
			}
			else{
				image_out[j][k] = image_in[j][k];
			}
			
		}

	
//Image Processing Ends

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
