#include <opencv2/world.hpp>
//#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc , char *argv[]) {
    /*  打开文件    */
    Mat img , oriImg;
    namedWindow("Origin Picture" , CV_WINDOW_NORMAL);
    img = imread(argv[1]);
    oriImg = img;
    imshow("Origin Picture" , img);
    waitKey(0);
    destroyWindow("Origin Picture");

//  选用性能更好的Sobel算法
//    Mat cannyDst;
////    cvtColor(img , img , CV_BGR2GRAY);
////    blur(img , img , cvSize(3 , 3));
//    Canny(img , cannyDst , 50 , 60 , 3);
//    namedWindow("Canny Result" , CV_WINDOW_NORMAL);
//    imshow("Canny Result" , cannyDst);
////    waitKey(0);
////    destroyWindow("Canny Result");

    /*  Sobel算法提取边缘信息      */
    Mat sobelDst;
    blur(img , img , cvSize(5 , 5));
    //Sobel(img, sobelDst, img.depth(),1,1);
    Sobel(img , sobelDst , - 1 , 1 , 0 , 3 , 1 , 0 , BORDER_DEFAULT);
//    convertScaleAbs(sobelDst, sobelDst);
    namedWindow("Sobel Result" , CV_WINDOW_NORMAL);
    imshow("Sobel Result" , sobelDst);
    waitKey(0);
    destroyWindow("Sobel Result");
//    destroyWindow("Canny Result");

    /*  二值化    */
    cvtColor(sobelDst , sobelDst , CV_BGR2GRAY);
    Mat imgThreshold;   // 二值化的图像
    threshold(sobelDst , imgThreshold , 0 , 255 , CV_THRESH_OTSU + CV_THRESH_BINARY);
    namedWindow("Threshold Result" , CV_WINDOW_NORMAL);
    imshow("Threshold Result" , imgThreshold);
    waitKey(0);
    destroyWindow("Threshold Result");
//    imgThreshold = cannyDst;

    /*  形态学闭运算  */
    Mat element = getStructuringElement(MORPH_RECT , cvSize(50 , 10));
    morphologyEx(imgThreshold , imgThreshold , CV_MOP_CLOSE , element);
    namedWindow("Morphology Result" , CV_WINDOW_NORMAL);
    imshow("Morphology Result" , imgThreshold);
    waitKey(0);
    destroyWindow("Morphology Result");

//    element = getStructuringElement(MORPH_RECT , cvSize(20 , 10));
//    morphologyEx(imgThreshold , imgThreshold , CV_MOP_CLOSE , element);
//    namedWindow("Morphology2 Result" , CV_WINDOW_NORMAL);
//    imshow("Morphology2 Result" , imgThreshold);
//    waitKey(0);
//    destroyWindow("Morphology2 Result");


    /*  寻找包络，并对每个包络寻找外接旋转矩形    */
    vector<vector<Point> > contours;
    findContours(imgThreshold , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_NONE);
    vector<vector<Point> >::iterator iter = contours.begin();
    vector<RotatedRect> rects;
    Mat rectImg(oriImg.size() , CV_8U , CvScalar(255));
    rectImg = oriImg;
    Point2f vertex[4];
    while (iter != contours.end()) {
        RotatedRect mr = minAreaRect(Mat(*iter));
        ++ iter;
        rects.push_back(mr);
        mr.points(vertex);
        for (int i = 0; i < 4; i ++) {
            line(rectImg , vertex[i] , vertex[(i + 1) % 4] , CvScalar(0) , 2 , LINE_AA);
        }

    }
    namedWindow("Rect Result" , CV_WINDOW_NORMAL);
    imshow("Rect Result" , rectImg);
    waitKey(0);
    destroyWindow("Rect Result");
    printf("Total Rects: %d" , rects.size());

//    Mat contourImg(oriImg.size(), CV_8U, CvScalar(255));
//    drawContours(contourImg, contours, -1, CvScalar(0), 3);
//    namedWindow("Contour Result",CV_WINDOW_NORMAL);
//    imshow("Contour Result",contourImg);
//    waitKey(0);
//    destroyWindow("Contour Result");



    waitKey(0);
    return 0;
}
