#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "common.hpp"
#include "threed.hpp"
using namespace std;

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
			glTranslatef(reference_point.x, reference_point.y, reference_point.z);
			glutSolidSphere(length, 50, 50);
			glPopMatrix();
		}
	}
};


#endif //OBJECT_HPP