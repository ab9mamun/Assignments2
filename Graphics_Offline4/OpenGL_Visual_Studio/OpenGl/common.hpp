#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <vector>
#include <windows.h>
#include <glut.h>
#include <algorithm>

#define R 0
#define G 1
#define B 2
#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3
#define VIEW_ANGLE 80
#define pi (2*acos(0.0))
#define DEBUG false


using namespace std;

double rad(double deg) {
	return deg * pi / 180;
}
double deg(double rad) {
	return rad * 180 / pi;
}
void ASSERT(bool cond, const char* str) {
	if (!cond) {
		cout << str << endl;
		cout << "Aborted" << endl;
		exit(1);
	}
}

template <typename T>
void copy_arr(T* dest, T* src, int length) {
	for (int i = 0; i < length; i++) {
		dest[i] = src[i];
	}
}


void debug_print(double a) {
	if (DEBUG)
		cout << a << endl;
}

void debug_print(double a, double b) {
	if (DEBUG)
		cout << a << " " << b << endl;
}
void debug_print(double a, double b, double  c) {
	if (DEBUG)
		cout << a << " " << b << " " << c << endl;
}

void debug_print(double a, double b, double  c, double d, double e, double f) {
	if (DEBUG)
		cout << a << " " << b << " " << c << " " << d << " " << e << " " << f << endl;
}

int roundToInt(double x) {
	return (int)(x + 0.5);
}


#endif //COMMON_HPP
