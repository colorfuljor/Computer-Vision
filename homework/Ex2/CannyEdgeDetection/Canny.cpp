//
//  Canny.cpp
//  Canny Edge Detector
//

#include "Canny.h"

#define PI 3.1415926

Canny::Canny(char filename[]) {
    original.load_bmp(filename);
    //original.display("original");
}

int Canny::cannyDetect(double lowThresh, double highThresh) {
	toGrayScale(original);
	filter(gray, 1.5);
	sobel(gauss);
	nonMaxSupp();
	hysterThres(lowThresh, highThresh);
	optimizeLine(edges, 2);
	return 1;
}

int Canny::toGrayScale(CImg<unsigned char> img)
{
	gray.assign(img.width(), img.height(), 1, 1, 0);
	cimg_forXY(img, x, y) {
		int R = img(x, y, 0);
		int G = img(x, y, 1);
		int B = img(x, y, 2);

		double A = (R * 0.212671 + G * 0.715160 + B * 0.072169);
		gray(x, y, 0) = gray(x, y, 0) = gray(x, y, 0) = A;
	}
    //gray.display("gray");
	return 1;
}

int Canny::sobel(CImg<unsigned char> img) {
    magnitude.assign(img.width(), img.height(), 1, 1, 0);
    orientation.assign(img.width(), img.height(), 1, 1, 0);
	double sumx, sumy;
	int width = img.width();
	int height = img.height();
    cimg_forXY(img, x, y) {
        if(x < 1 || x >= width - 1 || y < 1 || y >= height - 1){
            continue;
        } 
        sumx = (-1) * img(x-1, y-1) + 1 * img(x+1, y-1) + (-2) * img(x-1, y) + 2 * img(x+1, y) + (-1) * img(x-1, y+1) + 1 * img(x+1, y+1);
        sumy = (-1) * img(x-1, y-1) + 1 * img(x-1, y+1) + (-2) * img(x, y-1) + 2 * img(x, y+1) + (-1) * img(x+1, y-1) + 1 * img(x+1, y+1);
        magnitude(x, y) = sqrt(sumx*sumx + sumy*sumy);
        if (sumx == 0)
            orientation(x, y) = 90;
        else
            orientation(x, y) = atan(sumy / sumx) * (180.0 / PI);
    }
    //magnitude.display("magnitude");
	return 1;
}

int Canny::nonMaxSupp() {
    non.assign(magnitude.width(), magnitude.height(), 1, 1);

	cimg_forXY(magnitude, x, y) {
        if(x < 1 || x >= magnitude.width() -1 || y < 1 || y >= magnitude.height() - 1){
            continue;
        } //3x3的算子，图像的4条边需要跳过
        float Tangent = orientation(x, y);
        //cout << Tangent << " ";
        non(x, y) = magnitude(x, y);
        //有四个方向  
        //0: -0.4142 < tan θ <= 0.4142  Horizontal Edge
        //1: |tan θ| >= 2.4142  Vertical Edge
        //2: -2.4142 < tan θ <= -0.4142  -45 Degree Edge
        //3: 0.4142 < tan θ <= 2.4142  45 Degree Edge
        //画图更易理解

        //Horizontal Edge
        if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
        {
            //如果不是比左右两边大就为0
            if ((magnitude(x, y) < magnitude(x + 1, y)) || (magnitude(x, y) < magnitude(x - 1, y)))
                non(x, y) = 0;
        }
        //Vertical Edge
        if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
        {
            //如果不是比上下两边大就为0
            if ((magnitude(x, y) < magnitude(x , y + 1)) || (magnitude(x, y) < magnitude(x , y - 1)))
                non(x, y) = 0;
        }

        //-45 Degree Edge
        if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
        {
            //如果不是比-45度对角两边上大就为0
            if ((magnitude(x, y) < magnitude(x + 1, y + 1)) || (magnitude(x, y) < magnitude(x - 1, y - 1)))
                non(x - 1, y- 1) = 0;
        }

        //45 Degree Edge
        if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
        {
            //如果不是比45度对角两边上大就为0
            if ((magnitude(x, y) < magnitude(x - 1, y + 1)) || (magnitude(x, y) < magnitude(x + 1, y - 1)))
                non(x - 1, y - 1) = 0;
        }
	}
    //non.display("non");
    return 1;
}


int Canny::hysterThres(double lowThresh, double highThresh) {
    //If a pixel gradient is higher than the upper threshold, 
    // the pixel is accepted as an edge
    //If a pixel gradient value is below the lower threshold, 
    // then it is rejected.
    //If the pixel gradient is between the two thresholds, 
    // then it will be accepted only if it is connected to a pixel that is above the upper threshold.

    if (lowThresh > 255)
		lowThresh = 255;
	if (highThresh > 255)
		highThresh = 255;

	edges.assign(non.width(), non.height(), 1, 1, 0);

	cimg_forXY(non, x, y) {
		if (non(x, y) >= highThresh)
        {
            followEdges(x, y, lowThresh, highThresh);
        }
	}
    //edges.display("edges");
    return 1;
}

void Canny::followEdges(int x, int y, double lowThresh, double highThresh) {
    //递归的方法实现边缘跟踪
    edges(x, y) = 255;
    for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {

            if(i < 0 || i >= non.width()  || j < 0 || j >= non.height()) continue;
                
            if((j != y || i != x) && non(x, y) >= lowThresh && edges(x, y) != 255)
            {
                    followEdges(x, y, lowThresh, highThresh);
            }
        }
    }
}

float Canny::gaussian(int x, int y, double sigma) {
    return (1 / (sqrt(2 * PI) * sigma)) * exp(-(x * x + y * y) / (2 * sigma * sigma));
}

int Canny::filter(CImg<unsigned char> img, double sigma) {
    gauss.assign(img.width(), img.height(), 1, 1, 0);
    double sum, pSum;
    cimg_forXY(img, x, y) {
        if(x < 1 || x >= img.width() - 1 || y < 1 || y >= img.height() - 1){
            continue;
        } //3x3的filter，图像的4条边需要跳过
        sum = 0;
        pSum = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                sum += gaussian(i, j, sigma);
                pSum += gaussian(i, j, sigma) * img(x + i, y + j);
                
            }
        }
        gauss(x, y) = pSum / sum;
    }
	//gauss.display("gauss");
    return 1;
}

int Canny::optimizeLine(CImg<unsigned char> img, int distance) {//distance 是连接两个edge的最大距离
    line = img;
	//用于计算某一个像素点是否为边缘点
	//判断方法为查看以这个点为中心的八邻域，如果附近只有1个像素点为255, 其他7个为0则是边缘点
	bool isEdge[1000][1000];
    bool _isEdge[1000][1000];
	cimg_forXY(line, x, y) {
        isEdge[x][y] = false;
        if(x < 1 || x >= img.width() - 1 || y < 1 || y >= img.height() - 1 || line(x, y) == 0){
            continue;
        } //3x3的filter，图像的4条边需要跳过
        int edge = 0;
		for (int i = x - 1; i < x + 2; i++) {
			for (int j = y - 1; j < y + 2; j++) {
				if (!(i == x && j == y)) {
                    if (line(i, j) == 255)
                        edge++;
				}
			}
		}
		if (edge == 1) {}
			_isEdge[x][y] = isEdge[x][y] = true;
        //删除单个孤立的点
		if (edge == 0)
			line(x, y) = 0;
	}
	
	cimg_forXY(line, x, y) {
        if(x < distance || x >= img.width() - distance || y < distance || y >= img.height() - distance || !isEdge[x][y]){
            continue;
        } //3x3的filter，图像的4条边需要跳过
        for (int i = x - distance; i <= x + distance; i++) {
	        for (int j = y - distance; j <= y + distance; j++) {
			    if (isEdge[i][j]) {
                int white[] = {255, 255, 255};
				line.draw_line(x, y, i, j, white);
				isEdge[i][j] = false;
				isEdge[x][y] = false;
				}
		    }
        }
	}

    //删除长度少于20的连线
	//判断如果两个边界点的距离小于20，就删除这两个边界点组成的矩阵内所有白点，这样的话即使两个边界点分别是两条直线的话也无所谓
	//反正是这样的话这两边界点之间都是黑色区域，删除也无所谓
	cimg_forXY(line, x, y) {
		distance = 20;
		if (isEdge[x][y] == true) {
			int begin_x = x - distance > 0 ? x - distance : 0;
			int begin_y = y - distance > 0 ? y - distance : 0;
			int end_x = x + distance < img.width() - 1 ? x + distance : img.width() - 1;
			int end_y = y + distance < img.height() - 1 ? y + distance : img.height() - 1;
			for (int i = begin_x; i <= end_x; i++) {
				for (int j = begin_y; j <= end_y; j++) {
					if (isEdge[i][j] == true) {
						int max_x = x >= i ? x : i;
						int max_y = y >= j ? y : j;
						int min_x = max_x == x ? i : x;
						int min_y = max_y == y ? j : y;
						for (int ii = min_x; ii <= max_x; ii++) {
							for (int jj = min_y; jj <= max_y; jj++) {
								line(ii, jj) = 0;
							}
						}
						_isEdge[i][j] = false;
						_isEdge[x][y] = false;
					}
				}
			}
		}
	}
	//删除经过上一步处理完可能存在的单个孤立的噪声点
	cimg_forXY(line, x, y) {
		if(x < 1 || x >= img.width() - 1 || y < 1 || y >= img.height() - 1 || line(x, y) == 0){
            continue;
        } //3x3的filter，图像的4条边需要跳过
        int edge = 0;
		for (int i = x - 1; i < x + 2; i++) {
			for (int j = y - 1; j < y + 2; j++) {
				if (!(i == x && j == y)) {
                    if (line(i, j) == 255)
                        edge++;
				}
			}
		}
        //删除单个孤立的点
		if (edge == 0)
			line(x, y) = 0;
	}
	
    //line.display("line");
}

void Canny::save(CImg<unsigned char> img, char filename[]) {
    img.save_bmp(filename);
}

int main() {
    char testers1[] = "../test_data/lena.bmp";//受害者一号
    char testers2[] = "../test_data/20160326110137505.bmp";//受害者二号
    char testers3[] = "../test_data/3.bmp";//受害者三号
    char testers4[] = "../test_data/4.bmp";//受害者四号
    char testers5[] = "../test_data/bigben.bmp";//受害者五号
    char testers6[] = "../test_data/stpietro.bmp";//受害者六号

	//不同参数(参数也就只有双阈值)
    Canny test1(testers1);
    test1.cannyDetect(15, 35);
    test1.save(test1.line, "../output/different parameter/15_35.bmp");
    test1.cannyDetect(30, 70);
    test1.save(test1.line, "../output/different parameter/30_70.bmp");
    test1.cannyDetect(20, 100);
    test1.save(test1.line, "../output/different parameter/40_100.bmp");
    test1.cannyDetect(80, 100);
    test1.save(test1.line, "../output/different parameter/80_100.bmp");

    //每一步的结果输出
    test1.cannyDetect(30, 70);
    test1.save(test1.original, "../output/each step/0_original.bmp");
    test1.save(test1.gray, "../output/each step/1_gray.bmp");
    test1.save(test1.gauss, "../output/each step/2_gauss.bmp");
    test1.save(test1.magnitude, "../output/each step/3_magnitude.bmp");
    test1.save(test1.non, "../output/each step/4_non.bmp");
    test1.save(test1.edges, "../output/each step/5_edges.bmp");
    test1.save(test1.line, "../output/each step/6_line.bmp");

    //测试新增函数
    Canny test2(testers2);
    test2.cannyDetect(30, 70);
    test2.save(test2.edges, "../output/test optimizeLine/testers2_edges.bmp");
    test2.save(test2.line, "../output/test optimizeLine/testers2_line.bmp");
    test1.save(test1.edges, "../output/test optimizeLine/testers1_edges.bmp");
    test1.save(test1.line, "../output/test optimizeLine/testers1_line.bmp");

    //所有测试数据
    test1.cannyDetect(30, 70);
    test1.save(test1.edges, "../output/different test data/1.bmp");
    test2.cannyDetect(30, 70);
    test2.save(test2.edges, "../output/different test data/2.bmp");
    Canny test3(testers3);
    test3.cannyDetect(30, 70);
    test3.save(test3.edges, "../output/different test data/3.bmp");
    Canny test4(testers4);
    test4.cannyDetect(30, 70);
    test4.save(test4.edges, "../output/different test data/4.bmp");
    Canny test5(testers5);
    test5.cannyDetect(30, 70);
    test5.save(test5.edges, "../output/different test data/5.bmp");
    Canny test6(testers6);
    test6.cannyDetect(30, 70);
    test6.save(test6.edges, "../output/different test data/6.bmp");
}
