#include "Canny.h"

int main() {
	Canny test;
	char filename[] = "../test_Data/3.bmp";
	test.openImage(filename, test.img);
	test.cannyDetect(15, 35);
}