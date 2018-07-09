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

template <typename T>
T max(T a, T b, T c){
    return max(max(a,b), c);
}
template <typename T>
T min(T a, T b, T c){
    return min(min(a,b), c);
}


bool eq(double x, double y){
    return abs(x-y) < 0.0000001;
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
    double a, b, c, d; ///coefficients of plane equation#ax+by+cz=d

    Triangle(Point A, Point B, Point C, Color col){
        points[0] = A;
        points[1] = B;
        points[2] = C;
        color = col;
        find_plane();
    }
    Triangle(Point points[3], Color col){
        for (int i=0; i<3; i++)
            this->points[i] = points[i];

        color = col;
        find_plane();
    }
    void print(){
        for(int i=0; i<3; i++)
            points[i].print();
        color.print();
    }

    void find_plane(){
        Vector vab = points[1] - points[0];
        Vector vac = points[2] - points[0];
        Vector norm = vab*vac;
        norm = norm.unit();
        a = norm.x;
        b = norm.y;
        c = norm.z;
        d = a*points[0].x + b*points[0].y + c*points[0].z;
    }
    double find_z(double x, double y){
        return (d - a*x - b*y)/c;
    }

};


///============================Global Variables========================
int screen_height, screen_width;
double x_left_limit, x_right_limit, y_top_limit, y_bottom_limit;
double z_front_limit, z_rear_limit;
double dx, dy;
double bottom_y, left_x, right_x, top_y;
double** z_buffer;
Color** frame_buffer;
vector<Triangle> triangles;






///==

double sign (Point p1, Point p2, Point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (Point pt, Point v1, Point v2, Point v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0;
    b2 = sign(pt, v2, v3) < 0.0;
    b3 = sign(pt, v3, v1) < 0.0;

    return ((b1 == b2) && (b2 == b3));
}



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
    x_right_limit = -x_left_limit;
    y_top_limit = -y_bottom_limit;
    dx = x_right_limit*2/screen_width;
    dy = y_top_limit*2/screen_width;
    bottom_y = y_bottom_limit + dy/2;
    left_x = x_left_limit + dx/2;

    right_x = -left_x;
    top_y = -bottom_y;
}

void initialize_z_buffer_and_frame_buffer(){

    z_buffer = new double*[screen_width];
    frame_buffer = new Color*[screen_width];

    for(int i=0; i<screen_width; i++){
        z_buffer[i] = new double[screen_height];
        frame_buffer[i] = new Color[screen_height];

        for(int j=0; j<screen_height; j++)
            z_buffer[i][j] = z_rear_limit+1;
    }

}
void pre_process2(){

}

///helper--
int y_to_row(double y){
    int r =  (int) ((y - y_bottom_limit) / dy + 0.5);
    if (r<0){
        return 0;
    }
    else if (r>=screen_height){
        return screen_height -1;
    }
    else return r;
}

int x_to_col(double x){
    int c =  (int) ((x - x_left_limit) / dx + 0.5);
    if (c<0){
        return 0;
    }
    else if (c>=screen_width){
        return screen_width -1;
    }
    else return c;
}

//sets right, returns left
double get_intersection_y_to_x(Triangle t, double y, double& right, int& kase){
    double x1 = t.points[0].x;
    double y1 = t.points[0].y;
    double x2 = t.points[1].x;
    double y2 = t.points[1].y;
    double x3 = t.points[2].x;
    double y3 = t.points[2].y;
    double int1, int2;

    ///if two points are on the line

    if(eq(y,y1)){

        if(eq(y, y2)){
            right = max(x1,x2);
            kase = 1;
            return min(x1, x2);
        }
        if (eq(y, y3)){
            right = max(x1, x3);
            kase = 2;
            return min(x1, x3);
        }
        right = x1;
        kase = 3;
        return x1;
    }
    if(eq(y,y2)){
        if(eq(y,y3)){
            right = max(x2, x3);
            kase = 4;
            return min(x2, x3);
        }
        right = x2;
        kase =5;
        return x2;
    }
    if (eq(y, y3)){
        right = x3;
        kase = 6;
        return x3;
    }

    ///if two points are on the same side
    if ((y2-y)*(y3-y) >= 0){
        if(eq(y1, y2)){
            right = max(x1,x2);
            kase = 7;
            return min(x1, x2);
        }
        if(eq(y1, y3)){
            right = max(x1,x3);
            kase = 7;
            return min(x1, x3);
        }
        int1 = x2 + (x1-x2)/(y1-y2) * (y - y2);
        int2 = x3 + (x1-x3)/(y1-y3) * (y-y3);
        right = max(int1, int2);
        kase = 7;
        return min(int1, int2);
    }
    if ((y3-y)*(y1-y) >= 0 ){
        if(eq(y2, y3)){
            right = max(x2,x3);
            kase = 8;
            return min(x2, x3);
        }
        if(eq(y1, y2)){
            right = max(x1,x2);
            kase = 8;
            return min(x1, x2);
        }

        int1 = x3 + (x2-x3)/(y2-y3) * (y - y3);
        int2 = x1 + (x2-x1)/(y2-y1) * (y-y1);
        right = max(int1, int2);
        kase = 8;
        return min(int1, int2);
    }
    if ((y1-y)*(y2-y) >= 0){
        if(eq(y3, y2)){
            right = max(x2,x3);
            kase = 9;
            return min(x2, x3);
        }
        if(eq(y1, y3)){
            right = max(x1,x3);
            kase = 9;
            return min(x1, x3);
        }

        int1 = x1 + (x3-x1)/(y3-y1) * (y - y1);
        int2 = x2 + (x3-x2)/(y3-y2) * (y - y2);
        right = max(int1, int2);
        kase = 9;
        return min(int1, int2);

    }
    ASSERT(false, "Should not reach here\n");
}

void apply_procedure(){
    double bottom_scanline, top_scanline, left_scanline, right_scanline;
    double bottom_row, top_row, left_col, right_col;
    double lowest, highest, leftmost, rightmost;
    double leftest, rightest;
    int kase;
    double x, y, z;

    for(int k=0; k<triangles.size(); k++){
        Triangle t = triangles[k];
        lowest = min(t.points[0].y, t.points[1].y, t.points[2].y);
        highest = max(t.points[0].y, t.points[1].y, t.points[2].y);
        bottom_row = y_to_row(lowest) +1;
        top_row = y_to_row(highest)-1;

        leftest = min(t.points[0].x, t.points[1].x, t.points[2].x);
        rightest = max(t.points[0].x, t.points[1].x, t.points[2].x);

        for(int row = bottom_row; row <= top_row; row++){
            leftmost = get_intersection_y_to_x(t, y, rightmost, kase);  //min(t.points[0].x, t.points[1].x, t.points[2].x);
            left_col = x_to_col(leftmost);
            right_col = x_to_col(rightmost);

            y = bottom_y + row*dy;
            if (y < lowest || y>highest) continue;

            for(int col = left_col; col<=right_col; col++){
                x = left_x + col*dx;
                ///check may be added-- if normal to the screen in (x,y) intersects the triangle
                if (x < leftest || x > rightest) continue;
                if (!PointInTriangle(Point(x, y, 0), t.points[0], t.points[1], t.points[2])) continue;

                z = t.find_z(x, y);
                if(z<z_buffer[col][screen_height-row -1]){
                    z_buffer[col][screen_height-row-1] = z;
                    frame_buffer[col][screen_height-row-1] = t.color;
                }
            } //end col
        }//end row
    }//end triangle
}//end_function

void save(){
    bitmap_image image(screen_width,screen_height);
    for(int i=0;i<screen_width;i++){
        for(int j=0;j<screen_height;j++){
            image.set_pixel(i,j,frame_buffer[i][j].r,frame_buffer[i][j].g,frame_buffer[i][j].b);
        }
    }
    image.save_image("1.bmp");

    ///save z_buffer.txt
    FILE* z_txt = fopen("z_buffer.txt", "w");
    for(int j=0; j<screen_height; j++){
        for(int i=0; i<screen_width; i++){
            //cout<<"i'm here"<<endl;
            if(z_buffer[i][j]<z_rear_limit){
               fprintf(z_txt, "%.6lf\t", z_buffer[i][j]);
                //cout<<"sdfsfkjkjl"<<endl;
            }
        }
        fprintf(z_txt, "\n");
    }
    fclose(z_txt);
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
