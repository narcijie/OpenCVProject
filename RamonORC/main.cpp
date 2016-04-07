#include <opencv2/world.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    Mat img;
    namedWindow("TestWindow",CV_WINDOW_NORMAL);
    img = imread("1.jpg");
    imshow("TestWindow",img);

    waitKey(0);

    return 0;
}
