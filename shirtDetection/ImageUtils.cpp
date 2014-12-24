/**		ImageUtils.cpp:		Handy utility functions for dealing with images in OpenCV. by Shervin Emami, 21/9/09.
 **/

#include <stdio.h>
#include <tchar.h>
#include <vector>	// Used for C++ vectors
//#include <cstdio>	// Used for "printf"
#include <string>	// Used for C++ strings
//#include <cmath>	// Used to calculate square-root for statistics
#include <iostream>		// for printing streams in C++
#include <sstream>		// for printing floats in C++
#include <fstream>		// for opening files in C++

// Include OpenCV
#include <cv.h>
//#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>

#include "ImageUtils.h"		// easy image cropping, resizing, rotating, etc


using namespace std;


// Returns a new image that is a cropped version of the original image. 
IplImage* cropRectangle(IplImage *img, CvRect region)
{
	IplImage *imageTmp,*imageRGB;
	CvSize size;
	size.height = img->height;
	size.width = img-> width;

	if (img->depth != IPL_DEPTH_8U) {
		std::cerr << "ERROR: Unknown image depth of " << img->depth << " given in cropRectangle() instead of 8." << std::endl;
		exit(1);
	}

	// First create a new (color or greyscale) IPL Image and copy contents of img into it.
	imageTmp = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(img, imageTmp);

	// Create a new image of the detected region
	//printf("Cropping image at x = %d, y = %d...", faces[i].x, faces[i].y);
	//printf("Setting region of interest...");
	// Set region of interest to that surrounding the face
	cvSetImageROI(imageTmp, region);
	// Copy region of interest (i.e. face) into a new iplImage (imageRGB) and return it
	size.width = region.width;
	size.height = region.height;
	imageRGB = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
	cvCopy(imageTmp, imageRGB);	// Copy just the region.

    cvReleaseImage( &imageTmp );
	return imageRGB;		
}


// Draw a rectangle around the given object (defaults to a red color)
void drawRectangle(IplImage *img, CvRect face, CvScalar col)	{
	CvPoint p1, p2;
	p1.x = face.x;
	p1.y = face.y;
	p2.x = face.x + face.width;
	p2.y = face.y + face.height;
	cvRectangle(img, p1, p2, col, 2);
}

// Creates a new image copy that is of a different size.
// Remember to free the new image later.
IplImage* resizeImage(IplImage *origImg, int newWidth, int newHeight)
{
	if (newWidth <= 0 || newHeight <= 0) {
		std::cerr << "ERROR: Bad desired image size of " << newWidth << "x" << newHeight << " in resizeImage().\n";
		exit(1);
	}

	IplImage *outImg = cvCreateImage(cvSize(newWidth, newHeight), origImg->depth, origImg->nChannels);
	if (newWidth > origImg->width && newHeight > origImg->height) {
		// Make the image larger
		//printf("Making the image larger\n"); fflush(stdout);
		cvResetImageROI(origImg);
		cvResize(origImg, outImg, CV_INTER_CUBIC);	// CV_INTER_CUBIC is good for enlarging
	}
	else {
		// Make the image smaller
		//printf("Making the image smaller\n"); fflush(stdout);
		cvResetImageROI(origImg);
		cvResize(origImg, outImg, CV_INTER_AREA);	// CV_INTER_AREA is good for reduction / decimation.		
	}
	return outImg;
}


// Rotated clockwise by a number of radians and scale.
IplImage *rotateImage(IplImage *src, IplImage* dst, float angle, float scale)
{
	// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.
	float m[6];
	CvMat M = cvMat(2, 3, CV_32F, m);
	int w = src->width;
	int h = src->height;

	if (scale != 1.0 && scale > 1e-20)
		scale = 1.0f / scale;
	m[0] = (float)(scale * cos(angle));
	m[1] = (float)(scale * sin(angle));
	m[3] = -m[1];
	m[4] = m[0];
	m[2] = w*0.5f;  
	m[5] = h*0.5f;  

	// Transform the image
	cvGetQuadrangleSubPix( src, dst, &M);

	return dst;
}

// Save the given image to a JPG or BMP file, even if its format isn't an 8-bit image, such as a 32bit image.
int saveImage(char *filename, IplImage *image)
{
	int ret = -1;
	IplImage *image8Bit = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, image->nChannels);	// 8-bit greyscale image.
	if (image8Bit)
		cvConvert(image, image8Bit);	// Convert to an 8-bit image instead of potentially 16,24,32 or 64bit image.
	if (image8Bit)
		ret = cvSaveImage(filename, image8Bit);
	cvReleaseImage(&image8Bit);
	return ret;
}

// Store a greyscale floating-point CvMat image into a BMP/JPG/GIF/PNG image,
// since cvSaveImage() can only handle 8bit images (not 32bit float images).
void saveFloatMat(const char *filename, const CvMat *src)
{
	IplImage srcIplImg;
	//cout << "in Saving Image(" << ((CvMat*)src)->width << "," << ((CvMat*)src)->height << ") '" << filename << "'." << endl;
	cvGetImage(src, &srcIplImg);
	saveFloatImage(filename, &srcIplImg);
}


// Store a greyscale floating-point CvMat image into a BMP/JPG/GIF/PNG image,
// since cvSaveImage() can only handle 8bit images (not 32bit float images).
void saveFloatImage(const char *filename, const IplImage *srcImg)
{
	// Spread the 32bit floating point pixels to fit within 8bit pixel range.
	double minVal, maxVal;
	cvMinMaxLoc(srcImg, &minVal, &maxVal);

	cout << "Saving Image " << filename << " (" << srcImg->width << "," << srcImg->height << "). minV=" << minVal << ", maxV=" << maxVal << endl;

	// Deal with NaN and extreme values, since the DFT seems to give some NaN results.
	if (cvIsNaN(minVal) || minVal < -1e30)
		minVal = -1e30;
	if (cvIsNaN(maxVal) || maxVal > 1e30)
		maxVal = 1e30;
	if (maxVal-minVal == 0.0f)
		maxVal = minVal + 0.001;	// remove potential divide by zero errors.

	IplImage *dstImg = cvCreateImage(cvSize(srcImg->width, srcImg->height), 8, 1);
	cvConvertScale(srcImg, dstImg, 255.0 / (maxVal - minVal), - minVal * 255.0 / (maxVal-minVal));

	// Save the 8bit image
	cvSaveImage(filename, dstImg);

	cvReleaseImage(&dstImg);
	//cout << "done saveFloatImage()" << endl;
}

// Print some text info about the IplImage properties, to std::cout.
void printImageInfo(IplImage *image_tile)
{
	if (image_tile)
		std::cout << "Image: " << image_tile->width << "x" << image_tile->height << "px, " << image_tile->nChannels << "channels of " << image_tile->depth << "bit depth, ROI=[" << (int)image_tile->roi << "], widthStep=" << image_tile->widthStep << endl;
	else
		std::cout << "Image: <null>" << endl;
}
