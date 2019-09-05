#include "CImg.h"
using namespace cimg_library;

class UseCImg {
public:
	UseCImg();
	void display();
	void displayX();
	void changeColor();
	void drawTri();
	void drawTriX();
	void drawCircle();
	void drawCircleX();
	void drawLine();
	void drawLineX();
	void saveOp();
	void saveOpX();
private:
	CImg<unsigned char> img;
	CImg<unsigned char> imgX;
};

