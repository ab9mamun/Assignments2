#include <bits/stdc++.h>
#include "bitmap_image.hpp"

#define CAP 300
#define pi (2*acos(0.0))
using namespace std;



void ASSERT(bool cond, const char* str){
    if(!cond){
        cout<<str<<endl;
        cout<<"Aborted"<<endl;
        exit(1);
    }
}

class Vector{

public:
    double x,y,z;
    Vector(){}
    Vector(double x, double y, double z){
        this->x = x; this->y = y; this->z = z;
    }
    static Vector zero(){
        return Vector(0,0,0);
    }
    static Vector X(){
        return Vector(1,0,0);
    }
    static Vector Y(){
        return Vector(0,1,0);
    }
    static Vector Z(){
        return Vector(0,0,1);
    }


    Vector operator-(){
        return Vector(-x, -y, -z);
    }
    Vector operator+(Vector v){
        return Vector(x+v.x, y+v.y, z+v.z);
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
        return Vector(x*c, y*c, z*c);
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
        return Vector(y * v.z - z * v.y,
                 -(x * v.z - z * v.x),
                 x * v.y - y * v.x);
    }

    double operator[](int i){
        if(i==0) return x;
        if (i==1) return y;
        if(i==2) return z;
        return 0;
    }

    /// n is a perpendicular vector to the surface
    Vector reflect(Vector n){
        n = n.unit();
        Vector a = *this;
        return a - n* 2* a.dot(n);
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
        return Point(x + v.x, y+v.y, z+v.z);
       // printf("%lf %lf %lf\n", p.x, p.y, p.z);
	}
    Vector operator-(Point p){
        return Vector(x - p.x, y-p.y, z-p.z);
    }

     void set(double x, double y, double z){
        this->x = x; this->y = y; this->z = z;
    }
    void print(){
        printf("%.2lf %.2lf %.2lf\n", x, y, z);
    }
};

class Color{
public:
    int r;
    int g;
    int b;
    Color(){
        set(0,0,0);
    }
    Color(int r, int g, int b){
        set(r, g, b);
    }
    void set(int r, int g, int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
    void print(){
        cout<<r<<" "<<g<<" "<<b<<endl;
    }
};

class Triangle{
public:
    Point points[3];
    Color color;

    Triangle(Point A, Point B, Point C, Color col){
        points[0] = A;
        points[1] = B;
        points[2] = C;
        color = col;
    }
    Triangle(Point points[3], Color col){
        for (int i=0; i<3; i++)
            this->points[i] = points[i];

        color = col;
    }
    void print(){
        for(int i=0; i<3; i++)
            points[i].print();
        color.print();
    }

};


///============================Global Variables========================
int screen_height, screen_width;
double x_left_limit, x_right_limit, y_top_limit, y_bottom_limit;
double z_front_limit, z_rear_limit;
double dx, dy;
double top_y, left_x;
double** z_buffer;
Color** frame_buffer;
vector<Triangle> triangles;

///==

void read_data(){

ifstream config("config.txt"), stage3("stage3.txt");
config>>screen_width>>screen_height;
config>>x_left_limit>>y_bottom_limit;
config>>z_front_limit>>z_rear_limit;

///read triangles--
double x, y, z;
while(stage3>>x>>y>>z){
    Point points[3];
    points[0].set(x,y,z);
    stage3>>x>>y>>z;
    points[1].set(x,y,z);
    stage3>>x>>y>>z;
    points[2].set(x,y,z);

    Color col(rand()%256, rand()%256, rand()%256);
    triangles.push_back(Triangle(points, col));

}

config.close();
stage3.close();
}

void pre_process(){
    screen_width = 500;
    screen_height = 300;
}

void initialize_z_buffer_and_frame_buffer(){

    z_buffer = new double*[screen_width];
    frame_buffer = new Color*[screen_width];

    for(int i=0; i<screen_width; i++){
        z_buffer[i] = new double[screen_height];
        frame_buffer[i] = new Color[screen_height];
    }
}
void pre_process2(){
    for(int i=0; i<200; i++){
        for(int j=0; j<100; j++){
            frame_buffer[i][j].set(255,255,0);
        }
    }
}

void apply_procedure(){

}

void save(){
    bitmap_image image(screen_width,screen_height);
    for(int i=0;i<screen_width;i++){
        for(int j=0;j<screen_height;j++){
            image.set_pixel(i,j,frame_buffer[i][j].r,frame_buffer[i][j].g,frame_buffer[i][j].b);
        }
    }
    image.save_image("1.bmp");

    ///save z_buffer.txt
}

void post_process(){
    cout<<"Config"<<endl;
    cout<<screen_width<<" "<<screen_height<<endl;
    cout<<x_left_limit<<endl;
    cout<<y_bottom_limit<<endl;
    cout<<z_front_limit<<" "<<z_rear_limit<<endl;
    cout<<"Stage3"<<endl;
    for(int i=0; i<triangles.size(); i++){
        triangles[i].print();
    }

}

void free_memory(){
    for(int i=0; i<screen_width; i++){
        delete[] z_buffer[i];
        delete frame_buffer[i];
    }
    delete[] z_buffer;
    delete[] frame_buffer;
}

int main(){

    srand(34);

    read_data();
    pre_process();

    initialize_z_buffer_and_frame_buffer();
    pre_process2();

    apply_procedure();
    save();

    post_process();
    free_memory();

    return 0;
}
