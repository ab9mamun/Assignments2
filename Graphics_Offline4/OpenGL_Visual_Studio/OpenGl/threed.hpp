#ifndef THREED_H
#define THREED_H

///class declarations----------------------
#include <cstdio>
#include <cmath>
#define pi (2*acos(0.0))

///class declarations---
class Vector {

public:
	double x, y, z;
	Vector() {}
	Vector(double x, double y, double z) {
		this->x = x; this->y = y; this->z = z;
	}
	static Vector zero() {
		Vector v(0, 0, 0);
		return v;
	}
	static Vector X() {
		Vector v(1, 0, 0);
		return v;
	}
	static Vector Y() {
		Vector v(0, 1, 0);
		return v;
	}
	static Vector Z() {
		Vector v(0, 0, 1);
		return v;
	}


	Vector operator-() {
		Vector v(-x, -y, -z);
		return v;
	}
	Vector operator+(Vector v) {
		Vector w(x + v.x, y + v.y, z + v.z);
		return w;
	}
	Vector operator-(Vector v) {
		return (*this) + (-v);
	}
	double length2() {
		return x * x + y * y + z * z;
	}
	double length() {
		return sqrt(length2());
	}
	Vector operator*(double c) {
		Vector v(x*c, y*c, z*c);
		return v;
	}

	Vector operator/(double c) {
		return (*this)*(1.0 / c);
	}
	Vector unit() {
		double len = length();
		return (*this) / len;
	}

	double dot(Vector v) {
		return x * v.x + y * v.y + z * v.z;
	}

	Vector operator*(Vector v) {
		Vector w(y * v.z - z * v.y,
			-x * v.z + z * v.x,
			x * v.y - y * v.x);
		return w;
	}

	/// n is a perpendicular vector to the surface
	Vector reflect(Vector n) {
		n = n.unit();
		Vector a = *this;
		return a - n * 2 * a.dot(n);
	}

	///rotate respect to a perpendicular axis
	Vector rotatePA(double angle_deg, Vector axis = Z()) {
		double A = angle_deg * pi / 180.0;
		Vector r = axis.unit();
		Vector l = (*this);
		Vector u = r * l;
		u = l * cos(A) + u * sin(A);
		return u;
	}

	Vector perp2d() {
		Vector v(-y, x, 0);
		return v;
	}

	Vector rotate2d(double angle_deg) {
		double A = angle_deg * pi / 180.0;
		Vector u = (*this)*cos(A) + perp2d()*sin(A);
		return u;
	}

	void set(double x, double y, double z) {
		this->x = x; this->y = y; this->z = z;
	}


};

Vector operator*(double c, Vector v) {
	return v * c;
}


class Point
{
public:
	double x, y, z;
	Point() {}
	Point(Vector v) { x = v.x; y = v.y; z = v.z; }

	Point(double x, double y, double z) {
		this->x = x; this->y = y; this->z = z;
	}
	Vector asVector() { Vector v(x, y, z); return v; }

	Point operator+(Vector v) {
		Point p(x + v.x, y + v.y, z + v.z);
		// printf("%lf %lf %lf\n", p.x, p.y, p.z);
		return p;
	}
	Vector operator-(Point p) {
		Vector v(x - p.x, y - p.y, z - p.z);
		return v;
	}

	void set(double x, double y, double z) {
		this->x = x; this->y = y; this->z = z;
	}

};

#endif // THREED_H
