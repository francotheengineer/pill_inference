#include "opencv2/opencv.hpp" //load opencv libary
#include <iostream>
using namespace cv; //load OpenCV namespace
using namespace std;

string doubletoStr(double n) //method to convert double from iterator to double for file name
{
    stringstream result;
    result << n;
    return result.str();
}

int main(int, char)
 {
    VideoCapture cap(1); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
    return -1;

    namedWindow("frame", 1); //open webcam frame sample from serial bus
    int i = 0 ;
    vector<int> compression_params; //compression values for MATRIX to file saving
    compression_params.push_back(IMWRITE_JPEG_QUALITY); //define a jpeg quality object of pushback
    compression_params.push_back(100); //write jepg with 100% of matrix data

    for (;;) // for loop with delay to pull image from webcam
    {
        Mat frame; //define new matrix
        cap >> frame; // get a new frame from camera
        imshow("frame", frame); //show matrix in window

        string finalfilename; //variable for string filename of the matix to be written to disk

        //allows file location to be specified along with class specific name plus unique image number using method above
        finalfilename = "L:/testimages/calcium3" + doubletoStr(i) + ".jpg";


        imwrite(finalfilename, frame, compression_params); //write matrix to disk will 'finalfilename' with quality parameters

        cout << finalfilename << " \n"; //print file name to user in consol

        waitKey(30000); //variable wait key to prevent CPU overload - Using SSD allows for <2ms betweenframes.

        i++;
    }

    return 0;// the camera will be deinitialized automatically in VideoCapture destructor
}
