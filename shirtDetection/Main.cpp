// ShirtDetection v1.2: Find the approximate color type of a person's tshirt. by Shervin Emami (shervin.emami@gmail.com), 30th Aug 2010.

// If trying to debug the color detector code, enable SHOW_DEBUG_IMAGE:
#define SHOW_DEBUG_IMAGE


#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
//#include <stdio.h>
//#include <tchar.h>

#include <cstdio>	// Used for "printf"
#include <string>	// Used for C++ strings
#include <iostream>	// Used for C++ cout print statements
//#include <cmath>	// Used to calculate square-root for statistics

// Include OpenCV libraries
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "ImageUtils.h"		// Used for easy image cropping, resizing, rotating, etc.

using namespace std;
using namespace cv;

// Various color types for detected shirt colors.
enum                             {cBLACK=0,cWHITE, cGREY, cRED, cORANGE, cYELLOW, cGREEN, cAQUA, cBLUE, cPURPLE, cPINK,  NUM_COLOR_TYPES};
char* sCTypes[NUM_COLOR_TYPES] = {"Black", "White","Grey","Red","Orange","Yellow","Green","Aqua","Blue","Purple","Pink"};
uchar cCTHue[NUM_COLOR_TYPES] =    {0,       0,      0,     0,     20,      30,      55,    85,   115,    138,     161};
uchar cCTSat[NUM_COLOR_TYPES] =    {0,       0,      0,    255,   255,     255,     255,   255,   255,    255,     255};
uchar cCTVal[NUM_COLOR_TYPES] =    {0,      255,    120,   255,   255,     255,     255,   255,   255,    255,     255};


// Face Detection HaarCascade Classifier file for OpenCV (downloadable from "http://alereimondo.no-ip.org/OpenCV/34").
const char* cascadeFileFace = "haarcascades\\haarcascade_frontalface_alt.xml";	// Path to the Face Detection HaarCascade XML file


// Perform face or nose or mouth feature detection on the input image, using the given Haar cascade classifier.
// You can specify min detectable feature size, in case you are looking for small features like eyes.
// Returns a list of rectangles for detected regions.
// Remember to call "cvReleaseHaarClassifierCascade( &cascade );" later.
vector<CvRect> findObjectsInImage(IplImage *origImg, CvHaarClassifierCascade* cascade, CvSize minFeatureSize = cvSize(20, 20))
{
	CvMemStorage* storage;
	vector<CvRect> detRects;// = new vector<CvRect>;

	storage = cvCreateMemStorage(0);
	cvClearMemStorage( storage );

	// If the image is color, use a greyscale copy of the image.
	IplImage *detectImg = origImg;	// Assume the input image is to be used.
	IplImage *greyImg = 0;
	if (origImg->nChannels > 1) {
		greyImg = cvCreateImage(cvSize(origImg->width, origImg->height), 8, 1 );
		std::cout << "[Converting detectImg " << greyImg->width << "x" << greyImg->height << "]" << std::endl;
		printImageInfo(greyImg);
		cvCvtColor( origImg, greyImg, CV_BGR2GRAY );
		std::cout << "Got greyscale img." << std::endl;
		detectImg = greyImg;	// Use the greyscale version as the input.
	}
	
	// Enhance / Normalise the image contrast (optional)
	//cvEqualizeHist(detectImg, detectImg);

    double t = (double)cvGetTickCount();
    CvSeq* rects = cvHaarDetectObjects( detectImg, cascade, storage,
                                        1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                        minFeatureSize );	// Minimum face size changed from "cvSize(30, 30)"
	t = (double)cvGetTickCount() - t;
	std::cout << "detection time = " << t/((double)cvGetTickFrequency()*1000.) << "ms\n";

	// Get all the detected regions
	for(int i = 0; i < (rects ? rects->total : 0); i++ )
    {
        CvRect *r = (CvRect*)cvGetSeqElem( rects, i );

		detRects.push_back(*r);
		std::cout << "Found object at (" << r->x << ", " << r->y << ") of size (" << r->width << "x" << r->height << ").\n";
    }
	std::cout << "Found " << detRects.size() << " objects." << std::endl;

	//cvReleaseHaarClassifierCascade( &cascade );
	if (greyImg)
		cvReleaseImage( &greyImg );
	cvReleaseMemStorage( &storage );
	return detRects;
}

// Determine what type of color the HSV pixel is. Returns the colorType between 0 and NUM_COLOR_TYPES.
int getPixelColorType(int H, int S, int V)
{
	int color;
	if (V < 75)
		color = cBLACK;
	else if (V > 190 && S < 27)
		color = cWHITE;
	else if (S < 53 && V < 185)
		color = cGREY;
	else {	// Is a color
		if (H < 14)
			color = cRED;
		else if (H < 25)
			color = cORANGE;
		else if (H < 34)
			color = cYELLOW;
		else if (H < 73)
			color = cGREEN;
		else if (H < 102)
			color = cAQUA;
		else if (H < 127)
			color = cBLUE;
		else if (H < 149)
			color = cPURPLE;
		else if (H < 175)
			color = cPINK;
		else	// full circle 
			color = cRED;	// back to Red
	}
	return color;
}

// C/C++ entry point
int main(int argc, char **argv)
{
	cout << "Shirt Color Detection, by Shervin Emami, 7th Nov 2009." << endl;
	cout << "usage: ShirtDetection [image_file]" << endl;

	char *strFileImage = "n7.jpg";	// default file
	if (argc == 2)
		strFileImage = argv[1];	// Get image from first argument.
	if (!strFileImage) {
		cerr << "ERROR: No image was given on the command line!" << endl;
		return 1;
	}

	// Load the HaarCascade classifier for face detection. Added by Shervin on 22/9/09
	cout << "Loading Face HaarCascade in '" << cascadeFileFace << "'" << endl;
	CvHaarClassifierCascade* cascadeFace = (CvHaarClassifierCascade*)cvLoad(cascadeFileFace, 0, 0, 0 );
	if( !cascadeFace ) {
		cerr << "ERROR: Couldn't load face detector classifier in '" << cascadeFileFace << "'\n";
		exit(1);
	}

	// Open the image, either as greyscale or color
	IplImage* imageIn = cvLoadImage(strFileImage, CV_LOAD_IMAGE_UNCHANGED);
	if (!imageIn) {
		cerr << "Couldn't load image file '" << strFileImage << "'" << endl;
		exit(1);
	}
	if (imageIn->nChannels != 3) {
		cerr << "Input image isn't a color RGB image!" << endl;
		exit(1);
	}
	std::cout << "(got a " << imageIn->width << "x" << imageIn->height << " color image)." << std::endl;
	IplImage* imageDisplay = cvCloneImage(imageIn);

	// If trying to debug the color detector code, enable this:
	#ifdef SHOW_DEBUG_IMAGE
		// Create a HSV image showing the color types of the whole image, for debugging.
		IplImage *imageInHSV = cvCreateImage(cvGetSize(imageIn), 8, 3);
		cvCvtColor(imageIn, imageInHSV, CV_BGR2HSV);	// (note that OpenCV stores RGB images in B,G,R order.
		IplImage* imageDisplayHSV = cvCreateImage(cvGetSize(imageIn), 8, 3);	// Create an empty HSV image
		//cvSet(imageDisplayHSV, cvScalar(0,0,0, 0));	// Clear HSV image to blue.
		int hIn = imageDisplayHSV->height;
		int wIn = imageDisplayHSV->width;
		int rowSizeIn = imageDisplayHSV->widthStep;		// Size of row in bytes, including extra padding
		char *imOfsDisp = imageDisplayHSV->imageData;	// Pointer to the start of the image HSV pixels.
		char *imOfsIn = imageInHSV->imageData;	// Pointer to the start of the input image HSV pixels.
		for (int y=0; y<hIn; y++) {
			for (int x=0; x<wIn; x++) {
				// Get the HSV pixel components
				uchar H = *(uchar*)(imOfsIn + y*rowSizeIn + x*3 + 0);	// Hue
				uchar S = *(uchar*)(imOfsIn + y*rowSizeIn + x*3 + 1);	// Saturation
				uchar V = *(uchar*)(imOfsIn + y*rowSizeIn + x*3 + 2);	// Value (Brightness)
				// Determine what type of color the HSV pixel is.
				int ctype = getPixelColorType(H, S, V);
				//ctype = x / 60;
				// Show the color type on the displayed image, for debugging.
				*(uchar*)(imOfsDisp + (y)*rowSizeIn + (x)*3 + 0) = cCTHue[ctype];	// Hue
				*(uchar*)(imOfsDisp + (y)*rowSizeIn + (x)*3 + 1) = cCTSat[ctype];	// Full Saturation (except for black & white)
				*(uchar*)(imOfsDisp + (y)*rowSizeIn + (x)*3 + 2) = cCTVal[ctype];		// Full Brightness
			}
		}
		// Display the HSV debugging image
		IplImage *imageDisplayHSV_RGB = cvCreateImage(cvGetSize(imageDisplayHSV), 8, 3);
		cvCvtColor(imageDisplayHSV, imageDisplayHSV_RGB, CV_HSV2BGR);	// (note that OpenCV stores RGB images in B,G,R order.
		cvNamedWindow("Colors", 1);
		cvShowImage("Colors", imageDisplayHSV_RGB);
	#endif	// SHOW_DEBUG_IMAGE


	// First, search for all the frontal faces in the image
	CvRect foundFace = cvRect(0, 0, 0, 0);	// Set init values if nothing was detected.
	vector<CvRect> rectFaces;
	double timeFaceDetectStart = (double)cvGetTickCount();	// Record the timing.
	rectFaces = findObjectsInImage(imageIn, cascadeFace);
	double tallyFaceDetectTime = (double)cvGetTickCount() - timeFaceDetectStart;
	cout << "Found " << rectFaces.size() << " faces in " << tallyFaceDetectTime/((double)cvGetTickFrequency()*1000.) << "ms\n";

	// Process each detected face
	cout << "Detecting shirt colors below the faces." << endl;
	for (int r=0; r<rectFaces.size(); r++) {
		float initialConfidence = 1.0f;
		int bottom;
		CvRect rectFace = rectFaces[r];
		drawRectangle(imageDisplay, rectFace, CV_RGB(255,0,0));

		// Create the shirt region, to be below the detected face and of similar size.
		float SHIRT_DY = 1.4f;	// Distance from top of face to top of shirt region, based on detected face height.
		float SHIRT_SCALE_X = 0.6f;	// Width of shirt region compared to the detected face
		float SHIRT_SCALE_Y = 0.6f;	// Height of shirt region compared to the detected face
		CvRect rectShirt;
		rectShirt.x = rectFace.x + (int)(0.5f * (1.0f-SHIRT_SCALE_X) * (float)rectFace.width);
		rectShirt.y = rectFace.y + (int)(SHIRT_DY * (float)rectFace.height) + (int)(0.5f * (1.0f-SHIRT_SCALE_Y) * (float)rectFace.height);
		rectShirt.width = (int)(SHIRT_SCALE_X * rectFace.width);
		rectShirt.height = (int)(SHIRT_SCALE_Y * rectFace.height);
		cout << "Shirt region is from " << rectShirt.x << ", " << rectShirt.y << " to " << rectShirt.x + rectShirt.width - 1 << ", " << rectShirt.y + rectShirt.height - 1 << endl;

		// If the shirt region goes partly below the image, try just a little below the face
		bottom = rectShirt.y+rectShirt.height-1;
		if (bottom > imageIn->height-1) {
			SHIRT_DY = 0.95f;	// Distance from top of face to top of shirt region, based on detected face height.
			SHIRT_SCALE_Y = 0.3f;	// Height of shirt region compared to the detected face
			// Use a higher shirt region
			rectShirt.y = rectFace.y + (int)(SHIRT_DY * (float)rectFace.height) + (int)(0.5f * (1.0f-SHIRT_SCALE_Y) * (float)rectFace.height);
			rectShirt.height = (int)(SHIRT_SCALE_Y * rectFace.height);
			initialConfidence = initialConfidence * 0.5f;	// Since we are using a smaller region, we are less confident about the results now.
			cout << "Warning: Shirt region goes past the end of the image. Trying to reduce the shirt region position to " << rectShirt.y << " with a height of " << rectShirt.height << endl;
		}

		// Try once again if it is partly below the image.
		bottom = rectShirt.y+rectShirt.height-1;
		if (bottom > imageIn->height-1) {
			bottom = imageIn->height-1;	// Limit the bottom
			rectShirt.height = bottom - (rectShirt.y-1);	// Adjust the height to use the new bottom
			initialConfidence = initialConfidence * 0.7f;	// Since we are using a smaller region, we are less confident about the results now.
			cout << "Warning: Shirt region still goes past the end of the image. Trying to reduce the shirt region height to " << rectShirt.height << endl;
		}

		// Make sure the shirt region is in the image
		if (rectShirt.height <= 1) {
			cout << "Warning: Shirt region is not in the image at all, so skipping this face." << endl;
		}
		else {

			// Show the shirt region
			drawRectangle(imageDisplay, rectShirt, CV_RGB(255,255,255));

			// Convert the shirt region from RGB colors to HSV colors
			//cout << "Converting shirt region to HSV" << endl;
			IplImage *imageShirt = cropRectangle(imageIn, rectShirt);
			IplImage *imageShirtHSV = cvCreateImage(cvGetSize(imageShirt), 8, 3);
			cvCvtColor(imageShirt, imageShirtHSV, CV_BGR2HSV);	// (note that OpenCV stores RGB images in B,G,R order.
			if( !imageShirtHSV ) {
				cerr << "ERROR: Couldn't convert Shirt image from BGR2HSV." << endl;
				exit(1);
			}

			//cout << "Determining color type of the shirt" << endl;
			int h = imageShirtHSV->height;				// Pixel height
			int w = imageShirtHSV->width;				// Pixel width
			int rowSize = imageShirtHSV->widthStep;		// Size of row in bytes, including extra padding
			char *imOfs = imageShirtHSV->imageData;	// Pointer to the start of the image HSV pixels.
			// Create an empty tally of pixel counts for each color type
			int tallyColors[NUM_COLOR_TYPES];
			for (int i=0; i<NUM_COLOR_TYPES; i++)
				tallyColors[i] = 0;
			// Scan the shirt image to find the tally of pixel colors
			for (int y=0; y<h; y++) {
				for (int x=0; x<w; x++) {
					// Get the HSV pixel components
					uchar H = *(uchar*)(imOfs + y*rowSize + x*3 + 0);	// Hue
					uchar S = *(uchar*)(imOfs + y*rowSize + x*3 + 1);	// Saturation
					uchar V = *(uchar*)(imOfs + y*rowSize + x*3 + 2);	// Value (Brightness)

					// Determine what type of color the HSV pixel is.
					int ctype = getPixelColorType(H, S, V);
					// Keep count of these colors.
					tallyColors[ctype]++;
				}
			}

			// Print a report about color types, and find the max tally
			//cout << "Number of pixels found using each color type (out of " << (w*h) << ":\n";
			int tallyMaxIndex = 0;
			int tallyMaxCount = -1;
			int pixels = w * h;
			for (int i=0; i<NUM_COLOR_TYPES; i++) {
				int v = tallyColors[i];
				cout << sCTypes[i] << " " << (v*100/pixels) << "%, ";
				if (v > tallyMaxCount) {
					tallyMaxCount = tallyColors[i];
					tallyMaxIndex = i;
				}
			}
			cout << endl;
			int percentage = initialConfidence * (tallyMaxCount * 100 / pixels);
			cout << "Color of shirt: " << sCTypes[tallyMaxIndex] << " (" << percentage << "% confidence)." << endl << endl;

			// Display the color type over the shirt in the image.
			CvFont font;
			//cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,0.55,0.7, 0,1,CV_AA);	// For OpenCV 1.1
			cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,0.8,1.0, 0,1, CV_AA);	// For OpenCV 2.0
			char text[256];
			//sprintf_s(text, sizeof(text)-1, "%d%%", percentage);		
			cvPutText(imageDisplay, sCTypes[tallyMaxIndex], cvPoint(rectShirt.x, rectShirt.y + rectShirt.height + 12), &font, CV_RGB(255,0,0));
			cvPutText(imageDisplay, text, cvPoint(rectShirt.x, rectShirt.y + rectShirt.height + 24), &font, CV_RGB(255,0,0));


			// Free resources.
			cvReleaseImage( &imageShirtHSV );
			cvReleaseImage( &imageShirt );
		}//end if valid height
	}//end for loop

	// Display the RGB debugging image
	cvNamedWindow("Shirt", 1);
    cvShowImage("Shirt", imageDisplay);

	// Pause
	cvWaitKey();

	// Close everything
    cvDestroyWindow("Shirt");
	
	// Free resources.
	cvReleaseHaarClassifierCascade( &cascadeFace );
    cvReleaseImage(&imageDisplay);
    cvReleaseImage(&imageIn);
	return 0;
}