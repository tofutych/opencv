#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    Mat image;
    Mat image2;
    VideoCapture cap(0);
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    int centH = 480, centW = 640;
    while (true) {
        cap >> image;
        line(image, Point(centW / 2, centH / 2 - 30), Point(centW / 2, centH / 2 + 30), Scalar(0, 0, 255), 8, 8, 0);
        line(image, Point(centW / 2 - 30, centH / 2), Point(centW / 2 + 30, centH / 2), Scalar(0, 0, 255), 8, 8, 0);
        imshow("Red X", image);
        char c = (char) waitKey(1);
        if (c == 27) break;
    }
    return 0;
}
