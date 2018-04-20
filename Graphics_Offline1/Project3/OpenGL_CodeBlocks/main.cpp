#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>
#include "threed.hpp"

#define pi (2*acos(0.0))
#define RAD(x) (x*pi/180)

double cameraHeight;
double cameraAngle;

double armAroundY; ///-45 to 45 ---1,2
double armAroundX; /// 0 to 90 ----q,w
double elbow; ///0 to 90 ---3, 4
double palm; // - 45 to 45--- 5,6
double finger1; //-45 to 45 --7,8
double finger2; //0 to 90 -- 9,0

double rotsens;



void drawAxes()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);{
		glVertex3f( 100,0,0);
		glVertex3f(-100,0,0);

		glVertex3f(0,-100,0);
		glVertex3f(0, 100,0);

		glVertex3f(0,0, 100);
		glVertex3f(0,0,-100);
	}glEnd();
	
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			if (armAroundY < 45)
				armAroundY += rotsens;
			break;
		case '2':
			if (armAroundY > -45)
				armAroundY -= rotsens;
			break;
		case '3':
			if (elbow < 90)
				elbow += rotsens;
			break;
		case '4':
			if (elbow > 0)
				elbow -= rotsens;
			break;
		case '5':
			if (palm < 45)
				palm += rotsens;
			break;
		case '6':
			if (palm > -45)
				palm -= rotsens;
			break;
		case '7':
			if (finger1 < 45)
				finger1 += rotsens;
			break;
		case '8':
			if (finger1 > - 45)
				finger1 -= rotsens;
			break;
		case '9':
			if (finger2 < 90)
				finger2 += rotsens;
			break;
		case '0':
			if (finger2 > 0)
				finger2 -= rotsens;
			break;
		case 'q':
			if (armAroundX < 90)
				armAroundX += rotsens;
			break;
		case 'w':
			if (armAroundX > 0)
				armAroundX -= rotsens;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
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






void drawRoboticArm() {

	glColor3f(1.0, 1.0, 1.0);
	
	glRotatef(-armAroundX, 1, 0, 0);
	glRotatef(-armAroundY, 0, 1, 0);
	glTranslatef(0, 0, -25);
	
	glPushMatrix(); {
		glScalef(10, 10, 30);
		glutWireSphere(1, 10, 8); //arm
	}
	glPopMatrix();

	glTranslatef(0, 0, -1 * 30);
	glRotatef(-elbow, 0, 1, 0);
	glTranslatef(0, 0, -0.6 * 30);
	glPushMatrix(); {
		glScalef(10, 10, 30);
		glutWireSphere(0.6, 10, 8); //elbow
	} glPopMatrix();

	glTranslatef(0, 0, -0.6 * 30);
	glRotatef(-palm, 0, 0, 1);
	glPushMatrix(); {
		glScalef(10, 10, 10);
		glBegin(GL_TRIANGLES); {  //palm
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0.7, -1);
			glVertex3f(0, -0.7, -1);
		}
		glEnd();
	}
	glPopMatrix();
	
	glPushMatrix(); {
		glTranslatef(0, -0.7 * 10, -10);
		glRotatef(-finger1, 0, 0, 1);
		glTranslatef(0, 0, - 0.3 * 30);
		glScalef(10, 10, 30);
		glutWireSphere(0.3, 5, 4);  //finger1
	}glPopMatrix();
	
	glPushMatrix(); {
		glTranslatef(0, 0.7 * 10, -10);
		glRotatef(-finger2, 0, 1, 0);
		glTranslatef(0, 0, -0.3 * 30);
		glScalef(10, 10, 30);
		glutWireSphere(0.3, 5, 4);  //finger2
	}glPopMatrix();
	

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

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(175*cos(cameraAngle), 175*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawRoboticArm();



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	
	cameraHeight=-50;
	cameraAngle=0.5;

	armAroundY = 0; ///-45 to 45 ---1,2
	armAroundX = 0; /// 0 to 90 ----q,w
	elbow = 0; ///0 to 90 ---3, 4
	palm = 0; // - 45 to 45--- 5,6
	finger1 = 0; //-45 to 45 --7,8
	finger2 = 0; //0 to 90 -- 9,0

	rotsens = 3;

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
