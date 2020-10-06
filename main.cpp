#include "tgaimage.h"
#include <math.h>
#include <iostream>

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void DrawLineDDA(float x1, float y1, float x2, float y2, TGAImage& image) {

	float E = 1 / fmax(abs(x2 - x1), abs(y2 - y1));
	float dx =  (x2 - x1) * E;
	float dy = (y2 - y1) * E;
	float x0 = x1;
	float y0 = y1;
	if ( abs(dx) > abs(dy)) {
		while (abs(x2-x0)!=0) {
			image.set(round(x0), round(y0), red);
			x0 += dx;
			y0 += dy;
		}
	}else {
		while (abs(y2 - y0) != 0) {
			image.set(round(x0), round(y0), red);
			x0 += dx;
			y0 += dy;
		}
	}
	image.set(x2, y2, red);
}

void DrawLineBresenham(float x1, float y1, float x2, float y2, TGAImage& image) {
	float dx, dy, d, del1, del2, signX,signY,x0, y0,Max;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	if (dy > dx) {
		del1 = 2 * dx;
		del2 = 2 * (dx - dy);
		d = 2 * dx - dy;
		if (y1 < y2) {
			y0 = y1;
			x0 = x1;
			Max = y2;
			(x1 < x2) ? signX = 1 : signX = -1;
		}
		else {
			y0 = y2;
			x0 = x2;
			Max = y1;
			(x1 > x2) ? signX = 1 : signX = -1;
		}
		image.set(x0, y0, red);
		while (y0 < Max) {
			y0++;
			if (d > 0)
			{
				x0 += signX;
				d += del2;
			}
			else {
				d += del1;	
			}
			image.set(x0, y0, red);
		}
		
	}
	else {
		del1 = 2 * dy;
		del2 = 2 * (dy - dx);
		d = 2 * dy - dx;
		if (x1 < x2) {
			x0 = x1;
			y0 = y1;
			Max = x2;
			(y1 < y2) ? signY = 1 : signY = -1;
		}
		else {
			x0 = x2;
			y0 = y2;
			Max = x1;
			(y1 > y2) ? signY = 1 : signY = -1;
		}
		image.set(x0, y0, red);
		while (x0 < Max) {
			++x0;
			if (d > 0) {
				y0 += signY;
				d += del2;
			}else{
				d += del1;
			}
			image.set(x0, y0, red);

		}
	}

}

void Circle_Pixel(float x0, float y0, float x, float y, TGAColor color, TGAImage& image) {
	image.set(x0 + x, y0 + y, color);
	image.set(x0 + y, y0 + x, color);
	image.set(x0 + y, y0 - x, color);
	image.set(x0 + x, y0 - y, color);
	image.set(x0 - x, y0 - y, color);
	image.set(x0 - y, y0 - x, color);
	image.set(x0 - y, y0 + x, color);
	image.set(x0 - x, y0 + y, color);
}
void SimpleCircle(float x0, float y0, float R, TGAColor color, TGAImage& image) {
	for (int x = 0; x <= R / sqrt(2);++x) {
		int y = sqrt(R*R - x*x);
		Circle_Pixel(x0, y0, x, y, color, image);
	}
}

void CircleDDA(float x0, float y0, float R, TGAColor color, TGAImage& image) {

	float x1, x2, y1, y2, E, x, y;
	x1 = 0; y1 = R;
	x = x1; y = y1;
	E = 1 / pow(2, ceil( log(R)/log(2)));

	do {

		x2 = x1 + y1 * E;
		y2 = y1 - E * x2;
		image.set(x0 + x2, y0 - y2, color);
		x1 = x2;
		y1 = y2;

	} while ((y1 - y) < E || (x - x1) > E);
}

void CircleBresenham(float x0, float y0, float R, TGAColor color, TGAImage& image) {
	int d, x, y;
	x = x0;
	y = y0 + R;
	d = 3 - 2 * R;
	
	do {
		Circle_Pixel(x0, y0, x - x0, y - y0,color,image);
		if (d < 0) {
			d += 4 * (x - x0) + 6;
		}
		else {
			d += 4 * (x - x0 - y + y0) + 10;
			--y;
		}
		++x;
	} while (x < x0 + R / sqrt(2));

}
int main(int argc, char** argv) {
	TGAImage imageDDA(100, 100, TGAImage::RGB);
	TGAImage imageBresenham(100, 100, TGAImage::RGB);
	TGAImage imageCircle(100, 100, TGAImage::RGB);
	TGAImage imageCircleDDA(100, 100, TGAImage::RGB);
	TGAImage imageCircleBresenham(100, 100, TGAImage::RGB);
	float x0, x1, y0, y1, R;

	cout << "Enter coordinates of A(x0,y0) and B(x1,y1)" << endl;
	cin >> x0 >> y0 >> x1 >> y1;
	DrawLineDDA( x0, y0, x1, y1, imageDDA);
	DrawLineBresenham(x0, y0, x1, y1, imageBresenham);


	cout << "Enter coordinates of the center O(x0,y0) and the radius value" << endl;
	cin >> x0 >> y0 >> R;
    SimpleCircle(x0, y0, R, red, imageCircle);
	CircleDDA(x0, y0, R, red, imageCircleDDA);
	CircleBresenham(x0, y0, R, red, imageCircleBresenham);

	imageDDA.flip_vertically();
	imageDDA.write_tga_file("outputCDA.tga");
	imageBresenham.flip_vertically();
	imageBresenham.write_tga_file("outputBr.tga");
	imageCircle.flip_vertically();
	imageCircle.write_tga_file("outputSimpleCircle.tga");
	imageCircleDDA.flip_vertically();
	imageCircleDDA.write_tga_file("outputCircleDDA.tga");
	imageCircleBresenham.flip_vertically();
	imageCircleBresenham.write_tga_file("outputCircleBresenham.tga");
	return 0;
}

