///利用opencv提供的函数cvKMeans2()实现图像聚类

///////运行环境：VC6.0+opencv1.0

///////////////使用的时候，改变flag的初始值，即可实现对灰度图、彩色图基于色彩的聚类，以及根据位置和色彩对彩色图像聚类。
////////////////最后结果显示的时候请注意pResult 和pResult3
//#include <opencv2/world.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//using  namespace cv;
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#define nClusters 6


int main () {
    IplImage *img = cvLoadImage ("1.jpg", 1);//
    IplImage *pResult = cvCreateImage (cvSize (img->width, img->height), IPL_DEPTH_8U, 1);
    IplImage *pResult3 = cvCreateImage (cvSize (img->width, img->height), IPL_DEPTH_8U, 3);
    if (img == NULL)
        return 0;


    CvScalar color_tab[6];
    color_tab[0] = CV_RGB(255, 0, 0);///给5中颜色分别赋值
    color_tab[1] = CV_RGB(0, 255, 0);
    color_tab[2] = CV_RGB(0, 0, 255);
    color_tab[3] = CV_RGB(255, 0, 255);
    color_tab[4] = CV_RGB(255, 255, 0);
    color_tab[5] = CV_RGB(255, 0, 255);

    CvMat *samples;
    CvMat *clusters;
    int flag = 2;///0——灰度   1——彩色  2——位置+彩色
    switch (flag) {
        case 0:///Gray
        {
            IplImage *pImg = cvCreateImage (cvSize (img->width, img->height), IPL_DEPTH_8U, 1);
            cvCvtColor (img, pImg, CV_BGR2GRAY);
            assert(pImg != NULL && pImg->nChannels == 1);
            //创建样本矩阵，CV_32FC1代表位浮点通道（灰度图像）
            samples = cvCreateMat ((pImg->width) * (pImg->height), 1, CV_32FC1);
            //创建类别标记矩阵，CV_32SF1代表位整型通道
            clusters = cvCreateMat ((pImg->width) * (pImg->height), 1, CV_32SC1);
            //创建类别中心矩阵
//            CvMat*centers = cvCreateMat(nClusters, 1, CV_32FC1);
            // 将原始图像转换到样本矩阵
            {
                int k = 0;
                CvScalar s;
                for (int i = 0 ; i < pImg->width ; i++) {
                    for (int j = 0 ; j < pImg->height ; j++) {
                        s.val[0] = (float) cvGet2D (pImg, j, i).val[0];
                        cvSet2D (samples, k++, 0, s);
                    }
                }
            }
            //开始聚类，迭代次，终止误差1.0
            cvKMeans2 (samples, nClusters, clusters, cvTermCriteria (CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1.0));
            // 无需排序直接输出时
            int sortFlag = 0;
            if (sortFlag == 0) {
                int k = 0;
                int val = 0;
                float step = 255 / ((float) nClusters - 1);
                CvScalar s;
                for (int i = 0 ; i < pImg->width ; i++) {
                    for (int j = 0 ; j < pImg->height ; j++) {
                        val = (int) clusters->data.i[k++];
                        s.val[0] = 255 - val * step;//这个是将不同类别取不同的像素值
                        cvSet2D (pResult, j, i, s);  //将每个像素点赋值
                    }
                }
            }
            printf ("Gray Image\n");
            cvNamedWindow ("src", 1);
            cvShowImage ("src", pImg);
            //释放图像内存资源
            cvReleaseImage (&pImg);
            break;
        }//case0
        case 1:///RGB
        {
            assert(img != NULL && pResult != NULL);
            assert(img->nChannels == 3 && pResult->nChannels == 1);
            samples = cvCreateMat ((img->width) * (img->height), 1, CV_32FC3);//创建样本矩阵，CV_32FC3代表位浮点通道（彩色图像）
            clusters = cvCreateMat ((img->width) * (img->height), 1, CV_32SC1);//创建类别标记矩阵，CV_32SF1代表位整型通道
            int i, j, k = 0;
            for (i = 0 ; i < img->width ; i++) {
                for (j = 0 ; j < img->height ; j++) {
                    CvScalar s;
                    //获取图像各个像素点的三通道值（RGB）
                    s.val[0] = (float) cvGet2D (img, j, i).val[0];
                    s.val[1] = (float) cvGet2D (img, j, i).val[1];
                    s.val[2] = (float) cvGet2D (img, j, i).val[2];
                    cvSet2D (samples, k++, 0, s);//将像素点三通道的值按顺序排入样本矩阵
                }
            }
            //cvTermCriteria(CV_TERMCRIT_ITER,100,1.0)
            cvKMeans2 (samples, nClusters, clusters,
                       cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 1.0));//开始聚类，迭代次，终止误差1.0
            k = 0;
            int val = 0;
            float step = 255 / (nClusters - 1);
            for (i = 0 ; i < img->width ; i++) {
                for (j = 0 ; j < img->height ; j++) {
                    val = (int) clusters->data.i[k++];
                    CvScalar s;
                    s.val[0] = 255 - val * step;//这个是将不同类别取不同的像素值
                    cvSet2D (pResult3, j, i, color_tab[val]);        //将每个像素点赋值
                }
            }
            printf ("RGB Image\n");
            cvNamedWindow ("src", 1);
            cvShowImage ("src", img);
            break;
        }//case1
        case 2:///Position+RGB
        {
            assert(img != NULL && pResult != NULL);
            assert(img->nChannels == 3 && pResult->nChannels == 1);
            samples = cvCreateMat ((img->width) * (img->height), 5, CV_32FC1);//创建样本矩阵，CV_32FC3代表位浮点通道（彩色图像）
            clusters = cvCreateMat ((img->width) * (img->height), 1, CV_32SC1);//创建类别标记矩阵，CV_32SF1代表位整型通道
            int i = 0, j, k = 0;

            int x, y;
            float *data = samples->data.fl;
            int matcol = samples->cols;
            int step1 = samples->step / sizeof (float);

            for (x = 0 ; x < img->width ; x++) {
                for (y = 0 ; y < img->height ; y++) {
//                    CvScalar s;
                    //获取图像各个像素点的三通道值（RGB）
//                    s.val[0]=(float)cvGet2D(img,y,x).val[0];
//                    s.val[1]=(float)cvGet2D(img,y,x).val[1];
//                    s.val[2]=(float)cvGet2D(img,y,x).val[2];
//                    cvSet2D(samples,k++,0,s);//将像素点三通道的值按顺序排入样本矩阵
                    data[i * matcol + 0] = (float) x;
                    data[i * matcol + 1] = (float) y;
                    data[i * matcol + 2] = (float) cvGet2D (img, y, x).val[0];///cvGet2D(img,j,i).val[0];
                    data[i * matcol + 3] = (float) cvGet2D (img, y, x).val[1];
                    data[i * matcol + 4] = (float) cvGet2D (img, y, x).val[2];
                    i++;
                }
            }
            //cvTermCriteria(CV_TERMCRIT_ITER,100,1.0)
            cvKMeans2 (samples, nClusters, clusters,
                       cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 100, 1.0));//开始聚类，迭代次，终止误差1.0
            k = 0;
            int val = 0;
            float step = 255 / (nClusters - 1);
            for (i = 0 ; i < img->width ; i++) {
                for (j = 0 ; j < img->height ; j++) {
                    val = (int) clusters->data.i[k++];
                    CvScalar s;
                    s.val[0] = 255 - val * step;//这个是将不同类别取不同的像素值
                    cvSet2D (pResult, j, i, s);        //将每个像素点赋值
                }
            }
            printf ("Positon+RGB Result\n");
            cvNamedWindow ("src", 1);
            cvShowImage ("src", img);
            break;
        }//case2
    }//switch
    cvSaveImage ("kmeansResult.jpg", pResult); //存储结果
    cvNamedWindow ("result", 1);
    cvMoveWindow ("result", 600, 50);
    cvShowImage ("result", pResult);
    cvWaitKey (0);

    //释放矩阵内存资源
    cvReleaseMat (&samples);
    cvReleaseMat (&clusters);
    //释放图像内存资源
    cvReleaseImage (&img);
    cvReleaseImage (&pResult);
    cvReleaseImage (&pResult3);
    //释放窗口内存资源
    cvDestroyWindow ("src");
    cvDestroyWindow ("result");

    return 1;
}
