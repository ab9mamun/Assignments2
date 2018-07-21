#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "common.hpp"
#include "threed.hpp"
using namespace std;

//external functions---------------
void debug_print(double a);
void debug_print(double a, double b);
void debug_print(double a, double b, double  c);
void debug_print(double a, double b, double  c, double d, double e, double f);
template <typename T>
void copy_arr(T* dest, T* src, int length);


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

class Ray {
public:
	Point start;
	Vector dir;

	Ray(Point start, Vector dir) {
		this->start = start;
		this->dir = dir;
	}
};


class Object {
public:
	Point reference_point;
	double height, width, length;
	int shine;
	double color[3];
	double co_efficients[4];
	int pixels_covered;
	
	Object() { pixels_covered = 0; }
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

	virtual double intersect(Ray ray, double current_color[3], int level) {
		return -1;
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

	double intersection_t(Ray ray) {

		Vector Ro = (ray.start - reference_point);
		Vector Rd = ray.dir.normalize();
		double a, b, c, d, t1, t2;

		a = Rd.dot(Rd);
		b = 2 * Rd.dot(Ro);
		c = Ro.dot(Ro) - length * length;
		
		//debug_print(Ro.x, Ro.y, Ro.z, Rd.x, Rd.y, Rd.z);
		//debug_print(a, b, c);

		d = b * b - 4 * a*c;
		//debug_print(d);

		if (d < 0) return -1;

		t1 = (-b + sqrt(d)) / (2.0*a);
		t2 = (-b - sqrt(d)) / (2.0*a);

		//debug_print(t1, t2);
		Point T1, T2;
		T1 = ray.start + t1 * ray.dir;
		T2 = ray.start + t2 * ray.dir;

		debug_print(T1.x, T1.y, T1.z, T2.x, T2.y, T2.z);
		return min(t1, t2);
	}

	double intersect(Ray ray, double current_color[3], int level) {

		copy_arr(current_color, color, 3);
		double t =  intersection_t(ray);

		//debug_print(color[R], t);
		if (t <= 0) return -1;
		pixels_covered++;
		 return t;
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