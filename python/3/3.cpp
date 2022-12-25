
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void followEdges(int x, int y, Mat &magnitude, int tUpper, int tLower, Mat &edges) {
    
    edges.at<float>(y, x) = 255;
    
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if((i != 0) && (j != 0) && (x + i >= 0) && (y + j >= 0) && (x + i <= magnitude.cols) && (y + j <= magnitude.rows)){
                
                if((magnitude.at<float>(y + j, x + i) > tLower) && (edges.at<float>(y + j, x + i) != 255)) {
                    followEdges(x + i, y + j, magnitude, tUpper, tLower, edges);
                }
            }
        }
    }
}

void edgeDetect(Mat &magnitude, int tUpper, int tLower, Mat &edges) {
    
    int rows = magnitude.rows;
    int cols = magnitude.cols;
    
    edges = Mat(magnitude.size(), CV_32F, 0.0);
    
    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            if (magnitude.at<float>(y, x) >= tUpper){
                followEdges(x, y, magnitude, tUpper, tLower, edges);
            }
        }
    }
    
}

void nonMaximumSuppression(Mat &magnitudeImage, Mat &directionImage) {
    
    Mat checkImage = Mat(magnitudeImage.rows, magnitudeImage.cols, CV_8U);
    
    MatIterator_<float>itMag = magnitudeImage.begin<float>();
    MatIterator_<float>itDirection = directionImage.begin<float>();
    
    MatIterator_<unsigned char>itRet = checkImage.begin<unsigned char>();
    
    MatIterator_<float>itEnd = magnitudeImage.end<float>();
    
    for (; itMag != itEnd; ++itDirection, ++itRet, ++itMag) {
        const Point pos = itRet.pos();
        
        float currentDirection = atan(*itDirection) * (180 / 3.142);
        
        while(currentDirection < 0) currentDirection += 180;
        
        *itDirection = currentDirection;
        
        if (currentDirection > 22.5 && currentDirection <= 67.5) {
            if(pos.y > 0 && pos.x > 0 && *itMag <= magnitudeImage.at<float>(pos.y - 1, pos.x - 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
            if(pos.y < magnitudeImage.rows-1 && pos.x < magnitudeImage.cols-1 && *itMag <= magnitudeImage.at<float>(pos.y + 1, pos.x + 1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        }
        else if(currentDirection > 67.5 && currentDirection <= 112.5) {
            if(pos.y > 0 && *itMag <= magnitudeImage.at<float>(pos.y-1, pos.x)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
            if(pos.y<magnitudeImage.rows-1 && *itMag<=magnitudeImage.at<float>(pos.y+1, pos.x)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        
        }
        else if(currentDirection > 112.5 && currentDirection <= 157.5) {
            if(pos.y>0 && pos.x<magnitudeImage.cols-1 && *itMag<=magnitudeImage.at<float>(pos.y-1, pos.x+1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;;
            }
            if(pos.y < magnitudeImage.rows-1 && pos.x>0 && *itMag<=magnitudeImage.at<float>(pos.y+1, pos.x-1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        }
        else {
            if(pos.x > 0 && *itMag <= magnitudeImage.at<float>(pos.y, pos.x-1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
            if(pos.x < magnitudeImage.cols-1 && *itMag <= magnitudeImage.at<float>(pos.y, pos.x+1)) {
                magnitudeImage.at<float>(pos.y, pos.x) = 0;
            }
        }
        
        
        
    }
    
    
    
}


void MyCanny(Mat &src, Mat &edges, int upperThresh, int lowerThresh) {
    
    
    Mat image = src.clone();;
    
    GaussianBlur(src, image, Size(3, 3), 1.5);

    Mat magX = Mat(src.rows, src.cols, CV_32F);
    Mat magY = Mat(src.rows, src.cols, CV_32F);
    Sobel(image, magX, CV_32F, 1, 0, 3);
    Sobel(image, magY, CV_32F, 0, 1, 3);
    
    Mat slopes = Mat(image.rows, image.cols, CV_32F);
    divide(magY, magX, slopes);

    Mat sum = Mat(image.rows, image.cols, CV_64F);
    Mat prodX = Mat(image.rows, image.cols, CV_64F);
    Mat prodY = Mat(image.rows, image.cols, CV_64F);
    multiply(magX, magX, prodX);
    multiply(magY, magY, prodY);
    sum = prodX + prodY;
    sqrt(sum, sum);
    
    Mat magnitude = sum.clone();
    
    nonMaximumSuppression(magnitude, slopes);
    
    edgeDetect(magnitude, upperThresh, lowerThresh, edges);
    

}


int main(int argc, const char * argv[])
{
    
    Mat input;
    input = imread(argv[1], 0);
    int tUpper = atoi(argv[2]);
    int tLower = atoi(argv[3]);
    
    Mat edges;
    
    MyCanny(input, edges, tUpper, tLower);
    
    string s1 = "edge";
    string s2 = argv[1];
    string outName = s1 + s2;
    
    imshow("original", input);
    imshow(outName, edges);
    waitKey();

    
    return 0;
}