//
//  Canny.h
//  Canny Edge Detector
//


#ifndef _CANNY_
#define _CANNY_

using namespace std;

#include "CImg.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace cimg_library;

class Canny {
public:
    CImg<unsigned char> original; //Original Image
    CImg<unsigned char> gray; // Gray Scale
    CImg<unsigned char> gauss;//After Gaussian Filtering
    CImg<unsigned char> dx;
    CImg<unsigned char> dy;
    CImg<unsigned char> magnitude; //Sobel Filtered，梯度幅值
    CImg<unsigned char> orientation;  //梯度方向
    CImg<unsigned char> non; // Non-maximum Suppression
    CImg<unsigned char> edges; //Hysteresis Thresholding
    CImg<unsigned char> line;
    void followEdges(int, int, double, double);
    float gaussian(int x, int y, double sigma);
public:
//-------------------图像的Canny边缘检测------------------------------
// 算法原理：
//     图像的Canny边缘检测算法，是一种理论和实际效果比较靠谱的算法，它
//     大致有下面几个步骤：
//     1. 将输入的RGB图像转换成单通道的灰度图像
//     2. 计算灰度图像的一阶梯度，该算法选择Sobel算子计算dx和dy两个方向
//        的梯度
//     3. 对图像的梯度幅值进行非极大值抑制，这一步是Canny算法的独到之处
//        经过这一步后，真正的边缘点会被暴露出来，并将其位置保存下来
//     4. 双阈值检测：将保存下来的已确认为边缘的点，以每个点为中心，将
//        相邻的8个像素由1变成2，即将曲线的轮廓进行不断的延伸。
//
//   Sobel算子模板：https://en.wikipedia.org/wiki/Sobel_operator
//            | -1  0  +1 |          | -1  -2  -1 |
//       Sx = | -2  0  +2 |     Sy = |  0   0   0 |
//            | -1  0  +1 |          | +1  +2  +1 |
//

    //Canny算法
    Canny(char []);
    int cannyDetect(double, double);
    int toGrayScale(CImg<unsigned char>);
    int sobel(CImg<unsigned char> ); //Sobel filtering
    int nonMaxSupp(); //Non-maxima supp.
    int hysterThres(double, double); //Double threshold and finalize picture
    int filter(CImg<unsigned char>, double);
    int optimizeLine(CImg<unsigned char>, int);
    void save(CImg<unsigned char>, char []);
};

#endif
