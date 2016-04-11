#include  "opencv2/text.hpp"
#include  "opencv2/highgui.hpp"
#include  "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/opencv_modules.hpp"

#include  <vector>
#include  <iostream>
#include  <iomanip>
#include <afxres.h>

using namespace std;
using namespace cv;
uchar buf[] =
        {
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255,
                255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255,
                255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255,
                255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
                255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
        };

static void renderMSERs(const Mat& gray, Mat& img, const vector<vector<Point> >& msers)
{
    cvtColor(gray, img, COLOR_GRAY2BGR);
    RNG rng((uint64)1749583);
    for( int i = 0; i < (int)msers.size(); i++ )
    {
        uchar b = rng.uniform(0, 256);
        uchar g = rng.uniform(0, 256);
        uchar r = rng.uniform(0, 256);
        Vec3b color(b, g, r);

        const Point* pt = &msers[i][0];
        size_t j, n = msers[i].size();
        for( j = 0; j < n; j++ )
            img.at<Vec3b>(pt[j]) = color;
    }
}

int main(int argc, const char *argv[]) {
    cout << "This is a MSER Feature Test" << endl;

    /*  打开文件    */
    Mat img , oriImg;
    namedWindow("Origin Picture" , CV_WINDOW_NORMAL);
    img = imread(argv[1]);
    oriImg = img;
    imshow("Origin Picture" , img);


    /*  MSER    */
    static const int thresharr[] = { 0, 70, 120, 180, 255 };
    const int kDelta = 10;
    /*CV_WRAP static Ptr<MSER> create( int _delta=5, int _min_area=60, int _max_area=14400,
          double _max_variation=0.25, double _min_diversity=.2,
          int _max_evolution=200, double _area_threshold=1.01,
          double _min_margin=0.003, int _edge_blur_size=5 );
*/
    Ptr<MSER> mserExtractor = MSER::create (  5 , 20,  cvRound(0.1*(img.cols)*0.20*(img.rows)),0.25 , 0.2, 200, 1.01, 0.003, 5) ;
    vector<vector<Point> > msers;
    vector<Rect> boxes;
    mserExtractor->detectRegions(img, msers, boxes);
//    vector<vector<Point>> ::iterator iter = msers.begin();
//    while(iter != msers.end()){
//
//    }
    cout << "MSER Contours Count:" << msers.size()<< endl;
    cout << "MSER BBoxes Count:" << boxes.size()<< endl;
//    drawContours(img,msers,-1,CV_8U);
//    namedWindow("MSER Result" , CV_WINDOW_NORMAL);
//    imshow("MSER Result" , img);

    Mat imgMser(img.size(),CV_8U,cvScalar(0,0,0));
    drawContours(imgMser,msers,-1,cvScalar(255,255,255));
    namedWindow("MSER Region" , CV_WINDOW_NORMAL);
    imshow("MSER Region", imgMser);

    /*  形态学闭运算    */
    Mat imgMorph;
    Mat element = getStructuringElement(MORPH_RECT , cvSize(50 , 10));
    morphologyEx(imgMser , imgMorph , CV_MOP_CLOSE , element);
    namedWindow("Morphology Result" , CV_WINDOW_NORMAL);
    imshow("Morphology Result" , imgMorph);

    /*  寻找包络，求外接矩形*/
    vector<vector<Point> > contours;
    findContours(imgMorph , contours , CV_RETR_EXTERNAL , CV_CHAIN_APPROX_NONE);
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
            line(rectImg , vertex[i] , vertex[(i + 1) % 4] , CvScalar(0) , 10 , LINE_AA);
        }

    }
    namedWindow("Rect Result" , CV_WINDOW_NORMAL);
    imshow("Rect Result" , rectImg);


//    Mat imgRect = oriImg;
//    vector<Rect>::iterator iter = boxes.begin();
//    while(iter != boxes.end()){
//        Point pt1, pt2;
//        pt1.x = iter->x;
//        pt1.y = iter ->y;
//        pt2.x = iter ->x + iter->width;
//        pt2.y = iter->y + iter->height;
//        rectangle(imgRect, pt1,pt2,cvScalar(255,0,0));
//        iter ++;
////        rectangle(img,  iter->Rect_(), cvScalar(255,0,0));
//    }
//
//    namedWindow("MSER Boxes Result" , CV_WINDOW_NORMAL);
//    imshow("MSER Boxes Result" , imgRect);
    waitKey(0);
    destroyAllWindows();

    return 0;
}


