#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "common.hpp"
#include "threed.hpp"
using namespace std;

//external variables

//external functions---------------
extern void debug_print(double a);
extern void debug_print(double a, double b);
extern void debug_print(double a, double b, double  c);
extern void debug_print(double a, double b, double  c, double d, double e, double f);
template <typename T>
extern void copy_arr(T* dest, T* src, int length);


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
	double source_factor = 1.0, refIdx = 1.5;
	bool refraction_enabled = false;
	
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

	virtual void getColorAt(double colorAt[3], Point intersectionPoint) {
		copy_arr(colorAt, color, 3);
	}

	void setColorAt(double current_color[3], double colorAt[3]) {
		for (int i = 0; i < 3; i++) {
			current_color[i] = colorAt[i] * co_efficients[AMBIENT];
		}
		//cout << " I'm here" << endl;
	}
	virtual Vector getNormal(Point intersectionPoint) = 0;
	Vector getReflection(Ray ray, Vector normal) {
		Vector ref = 2 * ray.dir.dot(normal)*normal- ray.dir ;
		return ref.normalize();
	}


	double intersect(Ray ray, double current_color[3], int level);

	virtual double getIntersectingT(Ray ray) = 0;

	Vector  getRefraction(Ray ray, Vector normal);

	void illuminati(Ray ray, Point intersectionPoint, double current_color[3], int level);
};

class Sphere: public Object{
public:
	Sphere(Point center, double radius) {
	reference_point = center;
	length = radius;
	} 
	void draw(); 

	double getIntersectingT(Ray ray);

	Vector getNormal(Point intersectionPoint) {
		return (intersectionPoint - reference_point).normalize();
	}
};

class Floor: public Object {
public:
	Floor(double FloorWidth, double TileWidth) {
		reference_point = Point(-FloorWidth / 2, -FloorWidth / 2, 0);
		length = TileWidth;
	//	floor_width = FloorWidth;
	} 
	void draw(); 

	void getColorAt(double colorAt[3], Point intersectionPoint) {
		int pos_x = (intersectionPoint.x - reference_point.x)/length;
		int pos_y = (intersectionPoint.y - reference_point.y) / length;
		if ((pos_x + pos_y) % 2 ==0) {
			for(int c=0; c<3; c++) colorAt[c] = 0;

		}
		else {
			for (int c = 0; c<3; c++) colorAt[c] = 1;
		}
	}

	Vector getNormal(Point intersectionPoint) {
		return Vector::Z();
	}
	double getIntersectingT(Ray ray);
};


//--function implementations=====

/******
DRAW
*******/

void Sphere::draw() {
	//write codes for drawing sphere
	glPushMatrix(); {
		glColor3f(color[R], color[G], color[B]);
		glTranslatef(reference_point.x, reference_point.y, reference_point.z);
		glutSolidSphere(length, 50, 50);
		glPopMatrix();
	}
}
void Floor::draw() {//write codes for drawing black and white floor
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

/******
		INTERSECTING_T
*******/
double Sphere::getIntersectingT(Ray ray) {

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

	//debug_print(T1.x, T1.y, T1.z, T2.x, T2.y, T2.z);
	return min(t1, t2);
}

double Floor::getIntersectingT(Ray ray) {

	Vector normal = getNormal(reference_point);

	//https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
	double t = - Vector::dot(ray.start.asVector() ,normal) / Vector::dot( ray.dir, normal);
	return t > 0 ? t : -1;
}






//************external variables--------------

extern vector<Object*> objects;
extern vector<Point> lights;
extern int recursion_level;
///================================

double Object::intersect(Ray ray, double current_color[3], int level) {

	copy_arr(current_color, color, 3);
	double t = getIntersectingT(ray);

	//debug_print(color[R], t);
	if (t <= 0) return -1;
	pixels_covered++;

	if (level == 0) return t;
	Point intersectionPoint = ray.start + ray.dir*t;
	double colorAt[3];

	getColorAt(colorAt, intersectionPoint);
	setColorAt(current_color, colorAt);

	illuminati(ray, intersectionPoint, current_color, level);

	return t;


}

Vector Object::getRefraction(Ray ray, Vector normal) {
	Vector refraction(0, 0, 0);

	double dot = Vector::dot(normal, ray.dir);
	double k = 1.0 - refIdx * refIdx * (1.0 - dot * dot);

	if (k >= 0) {
		refraction = ray.dir * refIdx - normal * (refIdx * dot + sqrt(k));
		refraction = refraction.normalize();
	}

	return refraction;
}


void Object::illuminati(Ray ray, Point intersectionPoint, double current_color[3], int level) {

	Vector normal = getNormal(intersectionPoint);
	Vector reflection = getReflection(ray, normal);
	Vector refraction = getRefraction(ray, normal);
	double t, t_min;
	bool obscured;

	for (int i = 0; i<lights.size(); i++) {
		

		Vector dir = lights[i] - intersectionPoint;
		double len = dir.length();
		dir = dir.normalize();

		if (pixels_covered % 100 == 0) {
			debug_print(lights[i].x, lights[i].y, lights[i].z);
			debug_print(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
			debug_print(dir.x, dir.y, dir.z);
		}

		Point start = intersectionPoint + dir * 1.0;
		Ray L(start, dir);
		//cout<<intersectionPoint<<L.start<<L.dir;

		obscured = false;

		/*for each object check if the ray is obscured*/
		for (int j = 0; j < objects.size(); j++) {
			t = objects[j]->getIntersectingT(L);

			if (t >= 0 && t <= len) {
				obscured = true;
				break;
			}
		}

		obscured = true;
		if (!obscured) {

			double lambert = max(0, Vector::dot(L.dir, normal));
			double phong = max(0, pow(Vector::dot(reflection, ray.dir), shine));


			for (int c = 0; c<3; c++) {
				current_color[c] += source_factor * lambert * co_efficients[DIFFUSE]*color[c];
				current_color[c] += source_factor * phong * co_efficients[SPECULAR]*color[c];
			}
		}


		int nearest;
		/**now time for reflection */

		if (level < recursion_level) {

			start = intersectionPoint + reflection * 1.0;
			Ray reflectionRay(start, reflection);
			double ref_color[3];

			nearest = -1; //index of the nearest object--
			 t_min = 99999;
			////For each object k---------- 
			for (int k = 0; k < objects.size(); k++) {

				t = objects[k]->intersect(reflectionRay, ref_color, 0); /*dummy colorAt*/
													
				if (t <= 0) continue;
				//update t and nearest if t<t_min
				if (t < t_min) {
					t_min = t;
					nearest = k;
				}
			}
			if (nearest != -1) {

				objects[nearest]->intersect(reflectionRay, ref_color, level + 1);

				for (int c = 0; c<3; c++) {
					current_color[c] += ref_color[c] * co_efficients[REFLECTION];
				}
				for (int c = 0; c < 3; c++) {
					current_color[c] = max(0, min(1, current_color[c]));  //set between 0 to 1
				}
			}

			//end---
			if (!refraction_enabled) return;

			start = intersectionPoint + refraction * 1.0;
			Ray refractionRay(start, refraction);
			nearest = -1; //index of the nearest object--
			t_min = 99999;
			////For each object k---------- 
			for (int k = 0; k < objects.size(); k++) {

				t = objects[k]->intersect(refractionRay, ref_color, 0); /*dummy colorAt*/

				if (t <= 0) continue;
				//update t and nearest if t<t_min
				if (t < t_min) {
					t_min = t;
					nearest = k;
				}
			}
			if (nearest != -1) {

				objects[nearest]->intersect(refractionRay, ref_color, level + 1);

				for (int c = 0; c<3; c++) {
					current_color[c] += ref_color[c] * refIdx;
				}
				for (int c = 0; c < 3; c++) {
					current_color[c] = max(0, min(1, current_color[c]));  //set between 0 to 1
				}
			}
		}


	}

}


#endif //OBJECT_HPP