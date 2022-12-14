#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat image;
    Mat hsv_image;
    VideoCapture cap(0);
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    if (!cap.isOpened()) {
        cout << "cannot open camera";
    }

    while (true) {
        cap >> image;
        cvtColor(image, hsv_image, COLOR_BGR2HSV);
        imshow("Source", image);
        imshow("HSV", hsv_image);
        char c = (char) waitKey(1);
        if (c == 27) break;
    }
    return 0;
}
