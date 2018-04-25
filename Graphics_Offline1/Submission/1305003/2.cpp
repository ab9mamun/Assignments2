#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>
//#include "threed.hpp"
#define pi (2*acos(0.0))

using namespace std;

///class definitions-------
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

///global variables-------------------------
double cameraHeight;
Vector v(1,1,0);
Point pos(0,0,0);
double halflength;
double arrowlength;
double velocity;


void drawEmptySquare(double a)
{
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);{
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);

        glVertex3f( a, -a,0);
        glVertex3f( -a,-a,0);

        glVertex3f( -a, -a,0);
        glVertex3f( -a, a,0);

        glVertex3f( -a, a,0);
        glVertex3f( a, a,0);
    }glEnd();
}

void drawArrow(){

  //  printf("%lf %lf %lf \n", v.x, v.y, v.z);
    Vector u = v.unit();
    Point tip = pos + arrowlength*u;

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);{
        glVertex3f( pos.x, pos.y, pos.z);
        glVertex3f( tip.x, tip.y, tip.z);
    }glEnd();

    Vector perp = u.perp2d();
    Point A,B, C, D;
    A = tip;
    double AD = arrowlength*0.3;
    D = tip - AD*u;
    B = D + perp*AD*0.6;
    C = D - perp*AD*0.6;

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);{
        glVertex3f( A.x, A.y, A.z);
        glVertex3f( B.x, B.y, B.z);
        glVertex3f( C.x, C.y, C.z);
    }glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
		//	cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			v = v.rotate2d(-3.0);
			break;
		case GLUT_KEY_LEFT:
			v = v.rotate2d(3.0);
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
				//drawaxes=1-drawaxes;
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

	gluLookAt(0, 0, cameraHeight,		0,0,0,		0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	drawEmptySquare(halflength);
	drawArrow();


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	pos = pos + velocity*v.unit();

	if(pos.x > halflength){
        v = v.reflect(- Vector::X());
	}
	if(pos.y > halflength){
        v = v.reflect(- Vector::Y());
	}
	if(pos.x < - halflength){
        v = v.reflect(Vector::X());
	}
	if(pos.y < - halflength){
        v = v.reflect(Vector::Y());
	}

	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	halflength = 100.0;
	arrowlength = 15;
	cameraHeight = 100;
    velocity = 0.02;
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
	gluPerspective(100,	1,	1,	1000.0);
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
