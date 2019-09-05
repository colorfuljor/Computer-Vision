#include "UseCImg.h"
#include "CImg.h"
#include <cmath>  
#define PI 3.1415926

UseCImg::UseCImg() {
	img.load_bmp("1.bmp");
	imgX.load_bmp("1.bmp");
}

void UseCImg::display() {
	img.display("img");
}

void UseCImg::displayX() {
	imgX.display("imgX");
}

void UseCImg::changeColor() {
	cimg_forXY(img, x, y) {
		if (img(x, y, 0) == 255 && img(x, y, 1) == 255 && img(x, y, 2) == 255) {
			img(x, y, 1) = 0;
			img(x, y, 2) = 0;
		}
		
		if (img(x, y, 0) == 0 && img(x, y, 1) == 0 && img(x, y, 2) == 0) {
			img(x, y, 1) = 255;
		}
	}
	cimg_forXY(imgX, x, y) {
		if (imgX(x, y, 0) == 255 && imgX(x, y, 1) == 255 && imgX(x, y, 2) == 255) {
			imgX(x, y, 1) = 0;
			imgX(x, y, 2) = 0;
		}
		
		if (imgX(x, y, 0) == 0 && imgX(x, y, 1) == 0 && imgX(x, y, 2) == 0) {
			imgX(x, y, 1) = 255;
		}
	}
}

void UseCImg::drawCircle() {
	unsigned char color[] = {255, 255, 0};
	img.draw_circle(50, 50, 15, color);
}

void UseCImg::drawTri() {
	unsigned char color[] = {0, 0, 255};
	int x1 = 30;
	int y1 = 50 + 20 * tan(PI / 6);
	int x2 = 50;
	int y2 = 50 - 20 * tan(PI / 6) * 2;
	int x3 = 70;
	int y3 = 50 + 20 * tan(PI / 6);

	img.draw_triangle(x1, y1, x2, y2, x3, y3, color);
}

void UseCImg::drawLine() {
	unsigned char color[] = {0, 255, 0};
	img.draw_line(0, 0, 100 * sin(PI / 4), 100 * cos(PI / 4), color); 
}

void UseCImg::drawCircleX() {
	cimg_forXY(imgX, x, y) { 
		if ((pow(x - 50, 2) + pow(y - 50, 2)) <= 15 * 15) {
			imgX(x, y, 0) = 255;
			imgX(x, y, 1) = 255;
			imgX(x, y, 2) = 0;
		}
	} 
}

void UseCImg::drawTriX() {
	cimg_forXY(imgX, x, y) {
		if (abs(x - 50) <= 20 && (50 + 20 * tan(PI / 6) - y) <= (20 - abs(x - 50)) * tan(PI / 3) && (50 + 20 * tan(PI / 6) - y) >= 0) {
			imgX(x, y, 0) = 0;
			imgX(x, y, 1) = 0;
			imgX(x, y, 2) = 255;
		}
	}
}

void UseCImg::drawLineX() {
	cimg_forXY(imgX, x, y) {
		if (x == y && x < 100 * sin(PI / 4)) {
			imgX(x, y, 0) = 0;
			imgX(x, y, 1) = 255;
			imgX(x, y, 2) = 0;
		}		
	}
}

void UseCImg::saveOp() {
	changeColor();
	drawTri();
	drawCircle();
	drawLine();	
	display();
	img.save("2.bmp");
}

void UseCImg::saveOpX() {
	changeColor();
	drawTriX();
	drawCircleX();
	drawLineX();
	displayX();	
	imgX.save("X.bmp");
}

int main() {
	//测试显示图片与改变颜色 
	UseCImg test;
	test.display(); 
	test.changeColor();
	test.display(); 
	
	//对比使用与不使用CImg库的差异
	UseCImg test1;
	test1.drawTri();
	test1.drawTriX();
	test1.display();
	test1.displayX();
	
	UseCImg test2;
	test2.drawCircle();
	test2.drawCircleX();
	test2.display();
	test2.displayX();
	
	UseCImg test3;
	test3.drawLine();
	test3.drawLineX();
	test3.display();
	test3.displayX();
	
	//测试保存图片 
	UseCImg test4; 
	test4.saveOp();
	test4.saveOpX();
} 
