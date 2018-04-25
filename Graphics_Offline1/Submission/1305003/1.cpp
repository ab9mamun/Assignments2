#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>
//#include "threed.hpp"

#define pi (2*acos(0.0))

///class declarations---
class Vector{

public:
    double x,y,z;
    Vector(){}
    Vector(double x, double y, double z){
        this->x = x; this->y = y; this->z = z;
    }
    static Vector zero(){
        Vector v(0,0,0);
        return v;
    }
    static Vector X(){
        Vector v(1,0,0);
        return v;
    }
    static Vector Y(){
        Vector v(0,1,0);
        return v;
    }
    static Vector Z(){
        Vector v(0,0,1);
        return v;
    }


    Vector operator-(){
        Vector v(-x, -y, -z);
        return v;
    }
    Vector operator+(Vector v){
        Vector w(x+v.x, y+v.y, z+v.z);
        return w;
    }
    Vector operator-(Vector v){
        return (*this) + (-v);
    }
    double length2(){
        return x*x+y*y+z*z;
    }
    double length(){
        return sqrt(length2());
    }
    Vector operator*(double c){
        Vector v(x*c, y*c, z*c);
        return v;
    }

    Vector operator/(double c){
        return (*this)*(1.0/c);
    }
    Vector unit(){
        double len = length();
        return (*this)/len;
    }

    double dot(Vector v){
        return x*v.x + y*v.y + z*v.z;
    }

    Vector operator*(Vector v){
        Vector w(y * v.z - z * v.y,
                 -x * v.z + z * v.x,
                 x * v.y - y * v.x);
        return w;
    }

    /// n is a perpendicular vector to the surface
    Vector reflect(Vector n){
        n = n.unit();
        Vector a = *this;
        return a - n* 2* a.dot(n);
    }

    ///rotate respect to a perpendicular axis
    Vector rotatePA(double angle_deg, Vector axis = Z()){
        double A = angle_deg*pi/180.0;
        Vector r = axis.unit();
        Vector l = (*this);
        Vector u = r*l;
        u = l*cos(A) + u*sin(A);
        return u;
    }

    Vector perp2d(){
        Vector v(-y, x, 0);
        return v;
    }

    Vector rotate2d(double angle_deg){
        double A = angle_deg*pi/180.0;
        Vector u = (*this)*cos(A) + perp2d()*sin(A);
        return u;
    }

    void set(double x, double y, double z){
        this->x = x; this->y = y; this->z = z;
    }


};

Vector operator*(double c, Vector v){
    return v*c;
}


class Point
{
public:
	double x,y,z;
	Point(){}
	Point(Vector v){x= v.x; y = v.y; z = v.z;}

	Point(double x, double y, double z){
        this->x = x; this->y = y; this->z = z;
	}
	Vector asVector(){ Vector v(x,y,z); return v;}

	Point operator+(Vector v){
        Point p(x + v.x, y+v.y, z+v.z);
       // printf("%lf %lf %lf\n", p.x, p.y, p.z);
        return p;
	}
    Vector operator-(Point p){
        Vector v(x - p.x, y-p.y, z-p.z);
        return v;
    }

     void set(double x, double y, double z){
        this->x = x; this->y = y; this->z = z;
    }

};



///global variables
Point pos;
Vector u, r, l;
double movsens; ///move sensitivity
double rotsens; //rotate sensitivity;

double max_radius; ///max_radius of the sphere
double radius; /// current radius of the sphere
double radsens; ///radius sensitivity to home and end buttons.

int slices;




void drawAxes()
{
	glColor3f(0, 0, 1.0);
	glBegin(GL_LINES); {
		glVertex3f(100, 0, 0);
		glVertex3f(-100, 0, 0);

		glVertex3f(0, -100, 0);
		glVertex3f(0, 100, 0);

		glVertex3f(0, 0, 100);
		glVertex3f(0, 0, -100);
	}glEnd();
}

void drawSquare(double a) ///a is halflength;
{
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS); {
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
	}glEnd();
}

void drawCylinder1_4_origin(double r, int slices, double h) {
	Point points[100];
	int i;

	//generate points
	for (i = 0;i <= slices ; i++){
		points[i].x = r * cos(pi / 2 * i / slices);
		points[i].y = r * sin(pi / 2 * i / slices);
	}

	//draw quads using generated points
	for (i = 0;i < slices ;i++){
		glColor3f(0, (i + slices / 2.0) / (slices + slices / 2.0), 0);
		glBegin(GL_QUADS); {
			glVertex3f(points[i].x, points[i].y, -h/2);
			glVertex3f(points[i].x, points[i].y, h/2);
			glVertex3f(points[i + 1].x, points[i + 1].y, h/2);
			glVertex3f(points[i + 1].x, points[i + 1].y, -h/2);
		}glEnd();
	}
}

void drawSphere1_8_origin(double radius, int slices, int stacks)
{
	Point points[100][100];
	int i, j;
	double h, r;
	//generate points
	for (i = 0;i <= stacks;i++)
	{
		h = radius * sin(pi / 2 * i / stacks);
		r = radius * cos(pi / 2 * i / stacks);

		for (j = 0;j <= slices ;j++)
		{
			points[i][j].x = r * cos(pi / 2 * j /slices);
			points[i][j].y = r * sin(pi / 2 * j / slices);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0;i<stacks;i++)
	{
		glColor3f(( i+ stacks/4.0) / (stacks+stacks/4.0), 0, 0);
		for (j = 0;j<slices; j++)
		{
			glBegin(GL_QUADS); {
				//upper hemisphere
				glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
				glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
				glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
				glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
			}glEnd();
		}
	}
}


void drawCube1_3() {
	double a = max_radius - radius;
	glPushMatrix(); {
		glTranslatef(0, 0, max_radius);
		drawSquare(a);
		glTranslatef(0, 0, -2 * max_radius);
		drawSquare(a);
	}
	glPopMatrix();
}

void drawCube() {

	drawCube1_3();
	glPushMatrix(); {
		glRotatef(90, 1, 0, 0);
		drawCube1_3();
	}glPopMatrix();

	glPushMatrix(); {
		glRotatef(90, 0, 1, 0);
		drawCube1_3();
	}
	glPopMatrix();
}


void drawCylinder1_12() {
	double a = max_radius - radius;
	double h = 2 * a;
	// parallel to Z axis
	glPushMatrix(); {
		glTranslatef(a, a, 0);
		drawCylinder1_4_origin(radius, slices, h);
	}
	glPopMatrix();
}

void drawCylinder1_6() {
	drawCylinder1_12();
	glPushMatrix(); {
		glRotatef(90, 0, 0, 1);
		drawCylinder1_12();
	}glPopMatrix();
}
void drawCylinder1_2() {
	drawCylinder1_6();
	glPushMatrix(); {
		glRotatef(90, 1, 0, 0); //about X
		drawCylinder1_6();
	}glPopMatrix();
	glPushMatrix(); {
		glRotatef(90, 0, 1, 0); //about Y
		drawCylinder1_6();
	}glPopMatrix();
}

void drawCylinder() {
	drawCylinder1_2();
	glPushMatrix(); {
		glRotatef(180, 1, 0, 0);
		drawCylinder1_2();
	}glPopMatrix();
}


void drawSphere1_8() {
	double a = max_radius - radius;
	glPushMatrix(); {
		glTranslatef(a, a, a);
		drawSphere1_8_origin(radius, slices, slices);
	}glPopMatrix();
}
void drawSphere1_4() {

	drawSphere1_8();
	glPushMatrix(); {
		glRotatef(90, 0, 0, 1);
		drawSphere1_8();
	}glPopMatrix();
}
void drawSphere1_2() {
	drawSphere1_4();
	glPushMatrix(); {
		glRotatef(180, 0, 0, 1);
		drawSphere1_4();
	}glPopMatrix();

}

void drawSphere() {
	drawSphere1_2(); 	//upper hemisphere
	glPushMatrix(); {
		glRotatef(180, 1, 0, 0);
		drawSphere1_2();  //lower hemisphere
	}
	glPopMatrix();
}

void drawStoC() {
	drawCube();
	drawCylinder();
	drawSphere();
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':   ///rotate left;  r and l about u
			r = r.rotatePA(rotsens, u);
			l = l.rotatePA(rotsens, u);
			//printf("%lf %lf %lf %lf %lf %lf\n", r.x, r.y, r.z, l.x, l.y, l.z);
			break;
		case '2':   ///rotate right; r and l about u
			r = r.rotatePA(rotsens, -u);
			l = l.rotatePA(rotsens, -u);
			//printf("%lf %lf %lf %lf %lf %lf\n", r.x, r.y, r.z, l.x, l.y, l.z);
			break;
		case '3':   ///rotate up; l and u about r
			u = u.rotatePA(rotsens, r);
			l = l.rotatePA(rotsens, r);
			break;
		case '4':   ///rotate down; l and u about r
			u = u.rotatePA(rotsens, -r);
			l = l.rotatePA(rotsens, -r);
			break;
		case '5':   ///tilt clockwise; r and u about l
			r = r.rotatePA(rotsens, l);
			u = u.rotatePA(rotsens, l);
			break;
		case '6':   ///tilt counterclockwise; r and u about l
			r = r.rotatePA(rotsens, -l);
			u = u.rotatePA(rotsens, -l);
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//move backward
			pos = pos - movsens * l;
			break;
		case GLUT_KEY_UP:		// move forward
			pos = pos + movsens * l;
			break;

		case GLUT_KEY_RIGHT:   ///move left
			pos = pos + movsens * r;
			break;
		case GLUT_KEY_LEFT:  ///move right
			pos = pos - movsens * r;
			break;

		case GLUT_KEY_PAGE_UP:  ///move up
			pos = pos + movsens * u;
			break;
		case GLUT_KEY_PAGE_DOWN:  ///move down
			pos = pos - movsens * u;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			if (radius < max_radius)
				radius = radius + radsens;
			//printf("%lf \n", radius);
			break;
		case GLUT_KEY_END:
			if (radius > 0)
				radius = radius - radsens;
			//printf("%lf \n", radius);
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP

			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	gluLookAt(pos.x, pos.y, pos.z,
		pos.x + l.x, pos.y + l.y, pos.z + l.z,
		u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	drawAxes();

	drawStoC(); ///sphere to cube



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	double root2 = sqrt(2);
	u.set(0, 0, 1);
	r.set(-1 / root2, 1 / root2, 0);
	l.set(-1 / root2, -1 / root2, 0);
	pos.set(100, 100, 0);

	movsens = 2;
	rotsens = 3;

	max_radius = 40;
	radius = 16;
	radsens = 2;

	slices = 90;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
