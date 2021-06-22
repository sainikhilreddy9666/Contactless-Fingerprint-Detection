#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
 
using namespace cv;
using namespace std;

Mat Org_Frame;
Mat Img_hsv;
Mat Handed_Thres;
Mat HandOriginal;
Mat canny_OP;
int Low_H = 0;
int High_H = 179;
int Low_S = 60;
int High_S = 255;
int Low_V = 30;
int High_V = 255;
int blurSize = 5;
int BlobSize = 1;
int element = 10;
int Thresh = 100;
int min_rect_region = 1000;

Point2f center;
vector<Point> pts;

int main(int argc, char** argv)
{
    VideoCapture cap(0);
    if (!cap.isOpened())

    {
        cout << "Unable to access the Camera" << endl;
        return -1;
    }   

    namedWindow("Control", WINDOW_AUTOSIZE);
    createTrackbar("LOW H", "Control", &Low_H, 179);
    createTrackbar("HIGH H", "Control", &High_H, 255);
    createTrackbar("LOW S", "Control", &Low_S, 255);
    createTrackbar("HIGH S", "Control", &High_S, 255);
    createTrackbar("LOW V", "Control", &Low_V, 255);
    createTrackbar("HIGH V", "Control", &High_V, 255);

    while (1)
    {
        bool bSuccess = cap.read(Org_Frame);

        if (!bSuccess)
        {
            cout << "Unable to read a Frame" << endl;
            break;
        }

        cap >> Org_Frame;

        cvtColor(Org_Frame, Img_hsv, COLOR_BGR2HSV);
        inRange(Img_hsv, Scalar(Low_H, Low_S, Low_V), Scalar(High_H, High_S, High_V), Handed_Thres);
        line(Handed_Thres, { 0, Org_Frame.rows }, { Org_Frame.cols, Org_Frame.rows }, Scalar(0, 30, 0), 380, LINE_8);
        medianBlur(Handed_Thres, Handed_Thres, 7);

        erode(Handed_Thres, Handed_Thres, getStructuringElement(MORPH_ELLIPSE, Size(element, element)));
        dilate(Handed_Thres, Handed_Thres, getStructuringElement(MORPH_ELLIPSE, Size(element, element)));

        dilate(Handed_Thres, Handed_Thres, getStructuringElement(MORPH_ELLIPSE, Size(element, element)));
        erode(Handed_Thres, Handed_Thres, getStructuringElement(MORPH_ELLIPSE, Size(element, element)));

        imshow("HSV Image", Img_hsv);
        imshow("Handed Threshold", Handed_Thres);

        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

        Canny(Handed_Thres, canny_OP, Thresh, Thresh * 2, 3);

        findContours(Handed_Thres, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        cout << "Total number of contours are:" << contours.size() << endl;

        Mat drawing = Mat::zeros(Handed_Thres.size(), CV_8UC3);

        vector<RotatedRect> Min_Rect(contours.size());
        for (float j = 0; j < contours.size(); j++)
        {
            float rect_region = contourArea(contours[j]);

           
        }
       
        imshow("Original_Image", Org_Frame);


        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed" << endl;
            break;
        }
    }

    return 0;

}
