#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>
#include "threed.hpp"

#define pi (2*acos(0.0))

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
	glColor3f(1.0, 1.0, 1.0);
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

void drawCylinder1_4(double r, int slices, double h) {
	Point points[100];
	int i;

	//generate points
	for (i = 0;i <= slices / 4;i++){
		points[i].x = r * cos(((double)i / (double)slices) * 2 * pi);
		points[i].y = r * sin(((double)i / (double)slices) * 2 * pi);
	}

	//draw quads using generated points
	for (i = 0;i < slices / 4;i++){
		glColor3f(0, (i + slices / 2.0) / (slices + slices / 2.0), 0);
		glBegin(GL_QUADS); {
			glVertex3f(points[i].x, points[i].y, -h/2);
			glVertex3f(points[i].x, points[i].y, h/2);
			glVertex3f(points[i + 1].x, points[i + 1].y, h/2);
			glVertex3f(points[i + 1].x, points[i + 1].y, -h/2);
		}glEnd();
	}
}

void drawSphere1_8(double radius, int slices, int stacks)
{
	Point points[100][100];
	int i, j;
	double h, r;
	//generate points
	for (i = 0;i <= stacks;i++)
	{
		h = radius * sin(((double)i / (double)stacks)*(pi / 2));
		r = radius * cos(((double)i / (double)stacks)*(pi / 2));
		for (j = 0;j <= slices/4;j++)
		{
			points[i][j].x = r * cos(((double)j / (double)slices) * 2 * pi);
			points[i][j].y = r * sin(((double)j / (double)slices) * 2 * pi);
			points[i][j].z = h;
		}
	}
	//draw quads using generated points
	for (i = 0;i<stacks;i++)
	{
		glColor3f(( i+ stacks/4.0) / (stacks+stacks/4.0), 0, 0);
		for (j = 0;j<slices/4;j++)
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


void drawCubeInPlace() {
	double a = max_radius - radius;
	glColor3f(1, 1, 1);

	glPushMatrix(); {
		glTranslatef(0, 0, max_radius);
		drawSquare(a);
		glTranslatef(0, 0, - 2*max_radius);
		drawSquare(a);
	}
	glPopMatrix();
	
	glPushMatrix(); {
		glTranslatef(0, -max_radius, 0);
		glRotatef(90, 1, 0, 0); ///rotate about X axis
		drawSquare(a);
	}
	glPopMatrix();

	glPushMatrix(); {
		glTranslatef(0, +max_radius, 0);
		glRotatef(90, 1, 0, 0); ///rotate about X axis
		drawSquare(a);
	}
	glPopMatrix();

	glPushMatrix(); {
		glTranslatef(max_radius, 0, 0);
		glRotatef(90, 0, 1, 0); ///rotate about Y axis
		drawSquare(a);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(-max_radius, 0, 0);
		glRotatef(90, 0, 1, 0); ///rotate about Y axis
		drawSquare(a);
	}
	glPopMatrix();
}

void drawCylindersInPlace() {
	double a = max_radius - radius;
	double h = 2 * a;
	
	// parallel to Z axis
	glPushMatrix(); {
		glTranslatef(a, a, 0);
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(-a, a, 0);
		glRotatef(90, 0, 0, 1);
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(-a, -a, 0);
		glRotatef(180, 0, 0, 1);
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(a, -a, 0);
		glRotatef(90, 0, 0, -1);
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	/////////Parallel to X axis

	glPushMatrix(); {
		glTranslatef(0, a, -a);
		glRotatef(90, 0, 1, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(0, a, a);
		glRotatef(90, 1, 0, 0);
		glRotatef(90, 0, 1, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(0, -a, a);
		glRotatef(180, 1, 0, 0);
		glRotatef(90, 0, 1, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(0, -a, -a);
		glRotatef(90, -1, 0, 0);
		glRotatef(90, 0, 1, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();

	// parallel to Y axis
	glPushMatrix(); {
		glTranslatef(a, 0, a);
		glRotatef(90, 1, 0, 0);   // rotate about X axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(a, 0, -a);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(-a, 0,-a);
		glRotatef(180, 0, 1, 0);
		glRotatef(90, 1, 0, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	glPushMatrix(); {
		glTranslatef(-a, 0, a);
		glRotatef(90, 0, -1, 0);
		glRotatef(90, 1, 0, 0);   // rotate about Y axis.
		drawCylinder1_4(radius, slices, h);
	}
	glPopMatrix();
	
}

void drawStoC() {
	drawCubeInPlace();
	drawCylindersInPlace();
	//drawCylinder1_4(30, 80, 30);
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
			printf("%lf \n", radius);
			break;
		case GLUT_KEY_END:
			if (radius > 0)
				radius = radius - radsens;
			printf("%lf \n", radius);
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
	
	slices = 80;

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
