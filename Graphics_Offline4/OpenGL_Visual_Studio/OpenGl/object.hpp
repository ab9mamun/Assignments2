#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "common.hpp"
#include "threed.hpp"
using namespace std;

void drawTile(Point ref, double a, bool color)
{
	glColor3f(color, color, color);
	glBegin(GL_QUADS); {
		glVertex3f(ref.x, ref.y, ref.z);
		glVertex3f(ref.x, ref.y + a, ref.z);
		glVertex3f(ref.x + a, ref.y + a, ref.z);
		glVertex3f(ref.x + a, ref.y, ref.z);
	}glEnd();
}


void draw_light(Point reference_point) {
	glPushMatrix(); {
		glColor3f(1.0, 1.0, 0); ///yellow color
		glTranslatef(reference_point.x, reference_point.y, reference_point.z);
		glutSolidSphere(2, 40, 40);
		glPopMatrix();
	}
}

class Object {
public:
	Point reference_point;
	double height, width, length;
	int shine;
	double color[3];
	double co_efficients[4];
	
	Object() { }
	virtual void draw() {
		ASSERT(false, "should not reach here");
	}
	void setColor(double r, double g, double b){
		color[R] = r;
		color[G] = g;
		color[B] = b;
	}
	void setShine(int s){
		shine = s;
	}
	void setCoEfficients(double ambient, double diffuse, double specular, double reflection){
		co_efficients[AMBIENT] = ambient;
		co_efficients[DIFFUSE] = diffuse;
		co_efficients[SPECULAR] = specular;
		co_efficients[REFLECTION] = reflection;
	}

	void print() {
		cout << "Hi, I'm an object..\n";
	}

};


class Sphere: Object{
public:
	Sphere(Point center, double radius) {
	reference_point = center;
	length = radius;
	} 
	void draw() {
		//write codes for drawing sphere
		glPushMatrix(); {
			glColor3f(color[R], color[G], color[B]);
			glTranslatef(reference_point.x, reference_point.y, reference_point.z);
			glutSolidSphere(length, 50, 50);
			glPopMatrix();
		}
	}
};

class Floor:Object {
public:
	Floor(double FloorWidth, double TileWidth) {
		reference_point = Point(-FloorWidth / 2, -FloorWidth / 2, 0);
		length = TileWidth;
	//	floor_width = FloorWidth;
	} 
	void draw() {//write codes for drawing black and white floor
		double startx, starty, endx, endy;
		bool rowColor;
		startx = reference_point.x;
		starty = reference_point.y;
		endx = -startx;
		endy = -starty;
		rowColor = false;
		bool cellColor;

		for (double x = startx; x < endx; x += length) {
			cellColor = rowColor;
			for (double y = starty; y < endy; y += length) {
				drawTile(Point(x, y, reference_point.z), length, cellColor);
				cellColor = !cellColor;
			}
			rowColor = !rowColor;
		}
	}

	void getColorAt() {}
};

#endif //OBJECT_HPP