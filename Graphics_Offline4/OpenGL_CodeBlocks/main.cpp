#include "common.hpp"
#include "threed.hpp"
#include "object.hpp"
#include "bitmap_image.hpp"



using namespace std;
extern double rad(double);

///global variables
Point pos;
Vector u, r, l;
double movsens; ///move sensitivity
double rotsens; //rotate sensitivity;

double max_radius; ///max_radius of the sphere
double radius; /// current radius of the sphere
double radsens; ///radius sensitivity to home and end buttons.
int window_width, window_height;
int image_width, image_height;
int recursion_level;
vector<Object*> objects;
vector<Point> lights;


int slices;

void draw_light(Point ref);

void loadTestData() {
	pos.set(20, -70, 20);
	l = (Point(0, 0, 0) - pos).normalize();
	u.set(0, 0, 1);
	r = (l*u).normalize();
	recursion_level = 4;

	window_width = window_height = 500;

	Object* temp;
	temp = (Object*) new Sphere(Point(0,0,10), 10.0);
	temp->setColor(1, 0, 0);
	temp->setCoEfficients(0.4, 0.2, 0.2, 0.2);
	temp->setShine(1);
	temp->refraction_enabled = true;
	objects.push_back(temp);;

	Point light1(-50, 50, 50);
	lights.push_back(light1);

	temp = (Object*) new Floor(1000, 20);
	temp->setCoEfficients(0.6, 0.2, 0.2, 0.3);
	temp->setShine(3);
	objects.push_back(temp);


	temp = (Object*) new Sphere(Point(-20, -10, 10), 5.0);
	temp->setColor(1, 0.7, 0);
	temp->setCoEfficients(0.4, 0.2, 0.2, 0.2);
	temp->setShine(2);
	objects.push_back(temp);

	temp = (Object*) new Sphere(Point(0, 30, 15), 20);
	temp->setColor(0, 1, 1);
	temp->setCoEfficients(0.4, 0.2, 0.2, 0.2);
	temp->setShine(1);
	objects.push_back(temp);

	temp = (Object*) new Triangle(Point(10, -32, 2), Point(18, -28, 2), Point(14, -20, 12));
	temp->setColor(1, 0.1, 1);
	temp->setCoEfficients(0.4, 0.2, 0.2, 0.2);
	temp->setShine(1);
	objects.push_back(temp);


	image_width = image_height = 768;

}

void draw_everything() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}

	for (int i = 0; i < lights.size(); i++) {
		draw_light(lights[i]);
	}

}

void capture() {

	cout << "Capturing..." << endl;

	//**Initialize bitmap_image of image_width * image_height to black
	bitmap_image image(image_width, image_height);

	for (int i = 0;i<image_width;i++) {
		for (int j = 0;j<image_height;j++) {
			image.set_pixel(i, j, 0, 0, 0);
		}
	}

	double plane_distance = (window_height / 2) / tan(rad(VIEW_ANGLE) / 2);
	Point topleft = pos + l * plane_distance - r * window_width / 2 + u*window_height / 2;
	double du, dv;
	du = window_width * 1.0 / image_width;
	dv = window_height * 1.0 / image_height;
	Point corner;
	double t;
	double colorAt[3];
	int nearest;
	double t_min;
	int pixels = 0;

	for (int i = 0; i < image_width; i++) {
		for (int j = 0; j < image_height; j++) {
			 corner = topleft + r * (j*du+du/2) + - u * (i*dv + dv/2);
			 Ray ray(pos, (corner - pos).normalize());

			 nearest = -1; //index of the nearest object--
			 /** major bugfix*/ t_min = 99999;
			 ////For each object k----------
			 for (int k = 0; k < objects.size(); k++) {

				 t = objects[k]->intersect(ray, colorAt, 0); /*dummy colorAt*/
				/**dummyColorAt is the color array where pixel value
				will be stored in return time.As this is only for nearest
				object detection dummy should be sufficient.Level is 0
				here**/

				 if (t <= 0) continue;
				//update t and nearest if t<t_min
				 if (t < t_min) {
					 t_min = t;
					 nearest = k;
				 }
			 }
			 if (nearest != -1) {
				 t = objects[nearest]->intersect(ray, colorAt, 1);
				 //cout << "HERE "; debug_print(i, j);
				// debug_print(colorAt[R], colorAt[G], colorAt[B]);
				 pixels++;

				 image.set_pixel(j, i, roundToInt(colorAt[R]*255), roundToInt(colorAt[G]*255), roundToInt(colorAt[B]*255));
			 }
		}

	}

	//cout << "Total pixels colored: " << pixels << " "<<objects[0]->pixels_covered<<endl;

	image.save_image("test.bmp");
	cout << "Captured" << endl;
}







void drawAxes()
{

	glBegin(GL_LINES); {
		glColor3f(1.0, 0, 0.0);
		glVertex3f(100, 0, 0);
		glVertex3f(-100, 0, 0);

		glColor3f(0, 1.0, 0);
		glVertex3f(0, -100, 0);
		glVertex3f(0, 100, 0);

		glColor3f(0, 0, 1.0);
		glVertex3f(0, 0, 100);
		glVertex3f(0, 0, -100);
	}glEnd();
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '0':
			capture();
			break;

		case '1':   ///rotate left;  r and l about u
		    u = u.normalize();
			r = r.rotatePA(rotsens, u).normalize();
			l = l.rotatePA(rotsens, u).normalize();
			//printf("%lf %lf %lf %lf %lf %lf\n", r.x, r.y, r.z, l.x, l.y, l.z);
			break;
		case '2':   ///rotate right; r and l about u
		    u = u.normalize();
			r = r.rotatePA(rotsens, -u).normalize();
			l = l.rotatePA(rotsens, -u).normalize();
			//printf("%lf %lf %lf %lf %lf %lf\n", r.x, r.y, r.z, l.x, l.y, l.z);
			break;
		case '3':   ///rotate up; l and u about r
		    r = r.normalize();
			u = u.rotatePA(rotsens, r).normalize();
			l = l.rotatePA(rotsens, r).normalize();
			break;
		case '4':   ///rotate down; l and u about r
		    r = r.normalize();
			u = u.rotatePA(rotsens, -r).normalize();
			l = l.rotatePA(rotsens, -r).normalize();
			break;
		case '5':   ///tilt clockwise; r and u about l
		    l = l.normalize();
			r = r.rotatePA(rotsens, l).normalize();
			u = u.rotatePA(rotsens, l).normalize();
			break;
		case '6':   ///tilt counterclockwise; r and u about l
		    l = l.normalize();
			r = r.rotatePA(rotsens, -l).normalize();
			u = u.rotatePA(rotsens, -l).normalize();
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


	//cout << pos.x << " " << pos.y << " " << pos.z << " " << " " << pos.x + l.x << " " << pos.y + l.y << " " << pos.z + l.z << " "
	//	<<u.x<<" "<<u.y<<" "<<u.z<<endl;

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
	draw_everything();





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
	//u.set(0, 0, 1);
	//r.set(-1 / root2, 1 / root2, 0);
	//l.set(-1 / root2, -1 / root2, 0);
	//pos.set(100, 100, 30);

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
	gluPerspective(VIEW_ANGLE,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}



void absolute_test() {
	Sphere* sp = new Sphere(Point(0, 0, 10), 10);
	Ray ray(Point(5, -100, 5), Vector(0, 1, 0));
	double colorAt[3];

	double t = sp->intersect(ray, colorAt, 0);
	getchar();
	exit(0);
}

int main(int argc, char **argv){

	//absolute_test();

	///====================Ray tracing functions=========================
	loadTestData();
	///--end ray tracing functions----

	glutInit(&argc,argv);
	glutInitWindowSize(window_width, window_height);
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
