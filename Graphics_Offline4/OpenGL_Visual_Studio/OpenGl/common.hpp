#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <vector>
#include <windows.h>
#include <glut.h>

#define R 0
#define G 1
#define B 2
#define AMBIENT 0
#define DIFFUSE 1
#define SPECULAR 2
#define REFLECTION 3


using namespace std;

void ASSERT(bool cond, const char* str) {
	if (!cond) {
		cout << str << endl;
		cout << "Aborted" << endl;
		exit(1);
	}
}


#endif //COMMON_HPP
