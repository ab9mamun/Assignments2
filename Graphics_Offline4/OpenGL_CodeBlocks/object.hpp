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
	}
	glPopMatrix();
}

bool is_in(Point p, Point start, Point end_){
    return p.x >= start.x && p.x < end_.x
                && p.y >=start.y && p.y < end_.y
                && p.z >= start.z && p.z < end_.z;
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
	double source_factor = 1.0, eta_ref = 1.5;
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

	virtual Vector getNormal(Point intersectionPoint) = 0;
	Vector getReflection(Ray ray, Vector normal) {
		Vector ref = ray.dir - 2 * ray.dir.dot(normal)*normal ;
		return ref.normalize();
	}


	double intersect(Ray ray, double current_color[3], int level);

	virtual double getIntersectingT(Ray ray) = 0;

	Vector  getRefraction(Ray ray, Vector normal);

	void illuminati(Ray ray, Point intersectionPoint, double current_color[3], int level);

	virtual Vector normal_preprocess(Vector dir, Vector normal){
        return normal;
	}
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

class Triangle :public Object {
public:
	Point p1, p2, p3;
	Triangle(Point p1, Point p2, Point p3) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
	}

	Vector getNormal(Point intersectionPoint) {
		return (p2 - p1)*(p3 - p1);
	}

	double getIntersectingT(Ray ray);
	void draw();
	Vector normal_preprocess(Vector dir, Vector normal){
        if(dir.dot(normal) > 0) return -normal;  /// >0 = acute angle, ray and normal exits to the same direction of the plane
                                                ///.. so, need to reverse the normal--
        return normal;
	}

};

class GeneralQuadric: public Object {
public:
    double a, b, c, d, e, f, g, h, i, j;
    double width, height;
    GeneralQuadric(double coeffs[10], Point ref_point, double length, double width, double height){
        a = coeffs[0];
        b = coeffs[1];
        c = coeffs[2];
        d = coeffs[3];
        e = coeffs[4];
        f = coeffs[5];
        g = coeffs[6];
        h = coeffs[7];
        i = coeffs[8];
        j = coeffs[9];
        reference_point = ref_point;
        this->length = length;
        this->width = width;
        this->height = height;
    }

    void draw(){}

	double getIntersectingT(Ray ray);

	Vector getNormal(Point intersectionPoint);
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
	}
	glPopMatrix();
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

void Triangle::draw() {
    glColor3f(color[R], color[G], color[B]);
	glBegin(GL_TRIANGLES); {
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	}
	glEnd();
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
	if(t <0) return -1;
	Point intersectPoint = ray.start + ray.dir*t;
	if( intersectPoint.x > abs(reference_point.x) || intersectPoint.y > abs(reference_point.y)) return -1;

	return t;

}


///https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
double Triangle::getIntersectingT(Ray ray) {
    const float EPSILON = 0.0000001;
    Point vertex0 = p1;
    Point vertex1 = p2;
    Point vertex2 = p3;
    Vector edge1, edge2, h, s, q;
    float a,f,u,v;

    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = ray.dir*edge2;
    a = edge1.dot(h);

    if (a > -EPSILON && a < EPSILON)
        return -1;

    f = 1/a;
    s = ray.start - vertex0;
    u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
        return -1;

    q = s*(edge1);
    v = f * ray.dir.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return -1;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dot(q);
    if (t > EPSILON) // ray intersection
        return t;
    else // This means that there is a line intersection but not a ray intersection.
        return -1;
}

 Vector GeneralQuadric::getNormal(Point intersection) {

    return Vector( 2 * a * intersection.x + d * intersection.y + f * intersection.z  + g,
                2 * b * intersection.y + d * intersection.x + e * intersection.z  + h,
                2 * c * intersection.z + e * intersection.y + f * intersection.x  + i).normalize();
}

double GeneralQuadric::getIntersectingT(Ray ray) {

        double a1, b1, c1, d1;
        double t1, t2;
        bool t1in, t2in;
        double x1, y1, z1, x2, y2, z2;

        x1 = ray.start.x;
        y1 = ray.start.y;
        z1 = ray.start.z;
        x2 = ray.dir.x;
        y2 = ray.dir.y;
        z2 = ray.dir.z;

        a1 = a * x2 * x2 + b * y2 * y2 + c * z2*z2
                    +d*x2*y2 + e*y2*z2 + f*z2*x2;

        b1 = 2 * (a * x1 * x2 + b * y1*y2 + c * z1*z2)
                    +d*(x1*y2+x2*y1) + e*(y1*z2 + y2*z1) + f*(z1*x2 + z2*x1)
                    +g*x2 + h*y2 + i*z2;

        c1 = a* x1*x1 + b*y1*y1 + c*z1*z1
                    +d*x1*y1 + e*y1*z1 + f*z1*x1
                    +g*x1 + h*y1 + i*z1 + j;


        d1 = b1*b1 - 4*a1*c1;


        if (d1 < 0) return -1;

        t1 = (- b1 + sqrt(d1)) / (2.0*a1);
        t2 = (- b1 - sqrt(d1)) / (2.0*a1);

        Point p1, p2;
        p1= ray.start + ray.dir * t1;
        p2 = ray.start + ray.dir * t2;


        Point start, end_;

        start.set( reference_point.x, reference_point.y, reference_point.z);
        end_.set(start.x + length, start.y + width, start.z + height);


        t1in =  is_in(p1, start, end_) && (t1 > 0);
        t2in = is_in(p2, start, end_) && (t2 >0);

        if (t1in && t2in) return min(t1, t2);
        if(t1in) return t1;
        if(t2in) return t2;
        return -1;
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

	illuminati(ray, intersectionPoint, current_color, level);

	return t;


}

Vector Object::getRefraction(Ray ray, Vector normal) {
///https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel

	double cosi = max(-1.0, min(1.0, Vector::dot(normal, ray.dir) ));
	double k = 1.0 - eta_ref * eta_ref * (1.0 - cosi * cosi);

	if(k<0) return Vector(0,0,0);
    Vector refr = ray.dir * eta_ref - normal * (eta_ref * cosi + sqrt(k));
    return refr.normalize();
}


void Object::illuminati(Ray ray, Point intersectionPoint, double current_color[3], int level) {

	Vector normal = getNormal(intersectionPoint);
	normal = normal_preprocess(ray.dir, normal);
    Vector reflection = getReflection(ray, normal);
    Vector refraction = getRefraction(ray, normal);

	double t, t_min;
	bool obscured;

	double colorAt[3];

	getColorAt(colorAt, intersectionPoint);
	for(int c=0; c<3; c++)
        current_color[c] = colorAt[c]*co_efficients[AMBIENT];

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
        double dummyColorAt[3];
		/*for each object check if the ray is obscured*/
		for (int j = 0; j < objects.size(); j++) {
			t = objects[j]->intersect(L, dummyColorAt, 0);

			if (t >= 0 && t<= len) {
				obscured = true;
				break;
			}
		}


		//obscured = false;
		if (!obscured) {
        ///https://stackoverflow.com/questions/15619830/raytracing-how-to-combine-diffuse-and-specular-color
			double lambert = Vector::dot(L.dir, normal);
			double phong =  -Vector::dot(reflection, -L.dir);

			lambert = max(0.0, lambert);
			phong = max(0.0, phong);
			phong = pow(phong, shine);

			for (int c = 0; c<3; c++) {
                debug_print(source_factor, lambert, phong, co_efficients[DIFFUSE], co_efficients[SPECULAR], color[c]);
				current_color[c] += source_factor * lambert * co_efficients[DIFFUSE]*colorAt[c];  ///major bugfix
				current_color[c] += source_factor * phong * co_efficients[SPECULAR]*colorAt[c];  ///major bugfix
			}

			for(int c=0; c<3; c++){
                current_color[c] = max(0.0, min(1.0, current_color[c]));
			}
		}





        reflection = getReflection(ray, normal);
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
					current_color[c] = max(0.0, min(1.0, current_color[c]));  //set between 0 to 1
				}
			}

			//end---
			if (!refraction_enabled) continue;

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
					current_color[c] += ref_color[c] * 0.4;
				}
				for (int c = 0; c < 3; c++) {
					current_color[c] = max(0.0, min(1.0, current_color[c]));  //set between 0 to 1
				}
			}
		}


	}

}


#endif //OBJECT_HPP
