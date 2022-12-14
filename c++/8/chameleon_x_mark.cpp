#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat image;
    Mat image2;
    namedWindow("Display window");
    VideoCapture cap(0);
    int centH = 360, centW = 640;
    Vec3b vec;
    while (true) {
        cap >> image;
        cvtColor(image, image2, COLOR_BGR2HSV);
        imshow("Display window2", image2);
        vec = image2.at<Vec3b>(centH, centW);
        Vec3b vec2;
        int sec = 180 / 3;
        int sec2 = sec / 2;
        if ((vec[0] > sec2) && (vec[0] < (sec2 + sec))) {
            vec2[0] = 0;
            vec2[1] = 255;
            vec2[2] = 0;
        } else if ((vec[0] > sec2 + sec) && (vec[0] < 180 - sec2)) {
            vec2[0] = 255;
            vec2[1] = 0;
            vec2[2] = 0;
        } else {
            vec2[0] = 0;
            vec2[1] = 0;
            vec2[2] = 255;
        }
        line(image, Point(centW, centH - 30), Point(centW, centH + 30), Scalar(vec2), 8, 8, 0);
        line(image, Point(centW - 30, centH), Point(centW + 30, centH), Scalar(vec2), 8, 8, 0);
        imshow("Display window", image);
        char c = (char) waitKey(1);
        if (c == 27) break;
    }
    return 0;
}
