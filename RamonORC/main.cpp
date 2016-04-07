#include <opencv2/world.hpp>
//#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
//打开文件
    Mat img, oriImg;
    namedWindow("Origin Picture",CV_WINDOW_NORMAL);
    img = imread(argv[1]);
    oriImg = img;
    imshow("Origin Picture",img);
    waitKey(0);
    destroyWindow("Origin Picture");



    Mat cannyDst;
    cvtColor(img,img,CV_BGR2GRAY);
    blur(img,img,cvSize(3,3));
    Canny(img,cannyDst,20,120,3);
    namedWindow("Canny Result",CV_WINDOW_NORMAL);
    imshow("Canny Result",cannyDst);
    waitKey(0);
    destroyWindow("Canny Result");

//    Mat sobelDst;
//    Sobel(img, sobelDst, img.depth(),1,1);
//    namedWindow("Sobel Result",CV_WINDOW_NORMAL);
//    imshow("Sobel Result",sobelDst);
////      Sobel算法对该场景没有作用

    Mat imgThreshold;   // 二值化的图像
    threshold(cannyDst, imgThreshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

    Mat element = getStructuringElement(MORPH_RECT,cvSize(100,20));
    morphologyEx(imgThreshold, imgThreshold, CV_MOP_CLOSE, element);

    namedWindow("Threshold Result",CV_WINDOW_NORMAL);
    imshow("Threshold Result",imgThreshold);
    waitKey(0);
    destroyWindow("Threshold Result");

    vector< vector<Point> >     contours;
    findContours(imgThreshold, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    vector< vector<Point> >::iterator iter = contours.begin();
    vector<RotatedRect> rects;
    Mat rectImg(oriImg.size(), CV_8U, CvScalar(255));
    rectImg = oriImg;
    Point2f vertex[4];
    while(iter != contours.end())   {
        RotatedRect mr = minAreaRect(Mat (*iter));
        ++iter;
        rects.push_back(mr);
        mr.points(vertex);
        for(int i = 0; i < 4; i++)
        {
            line(rectImg, vertex[i], vertex[ (i+1) % 4], CvScalar(0), 2, LINE_AA);
        }

    }
    namedWindow("Rect Result",CV_WINDOW_NORMAL);
    imshow("Rect Result",rectImg);
    waitKey(0);
    destroyWindow("Rect Result");
    printf("Total Rects: %d",rects.size());

//    Mat contourImg(oriImg.size(), CV_8U, CvScalar(255));
//    drawContours(contourImg, contours, -1, CvScalar(0), 3);
//    namedWindow("Contour Result",CV_WINDOW_NORMAL);
//    imshow("Contour Result",contourImg);
//    waitKey(0);
//    destroyWindow("Contour Result");



    waitKey(0);
    return 0;
}
