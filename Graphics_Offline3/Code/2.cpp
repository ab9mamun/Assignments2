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
    int id;

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
    Triangle(Point points[3], Color col, int id){
        for (int i=0; i<3; i++)
            this->points[i] = points[i];

        color = col;
        this->id = id;
        find_plane();
    }

    void set_id(int id){
        this->id = id;
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

class Edge{
public:
    double ymin, ymax, del_x;
    int tr_id;
    double x_at_ymin;
    Edge(){
    }
    Edge(double ymin, double ymax, double del_x, int tr_id, double x_at_ymin){
         set( ymin,  ymax,  del_x,  tr_id,  x_at_ymin);
    }
    void set(double ymin, double ymax, double del_x, int tr_id, double x_at_ymin){
        this->ymin = ymin;
        this->ymax = ymax;
        this->del_x = del_x;
        this->tr_id = tr_id;
        this->x_at_ymin = x_at_ymin;
    }
    void print(){
        cout<<"TRID: "<<tr_id<<" ymin: "<<ymin<<" ymax: "<<ymax<<" x_at_ymin: "<<x_at_ymin<<" del_x: "<<del_x<<endl;
    }
};

bool compare_edges(Edge& e1, Edge& e2){
    return e1.x_at_ymin < e2.x_at_ymin;
}

///============================Global Variables========================
int screen_height, screen_width;
double x_left_limit, x_right_limit, y_top_limit, y_bottom_limit;
double z_front_limit, z_rear_limit;
double dx, dy;
double bottom_y, left_x, right_x, top_y;
double absolute_miny, absolute_maxy;

vector<Edge>* edge_table;
vector<Edge> active_edge_table;
vector<bool> active_polygon_table;
Color** frame_buffer;
vector<Triangle> triangles;




bool compare_depth(pair<int, pair<double, double> >& a, pair<int, pair<double, double> >& b){
    int id1, id2;
    id1 = a.first;
    id2 = b.first;
    return triangles[id1].find_z(a.second.first, a.second.second) < triangles[id2].find_z(b.second.first, b.second.second);
}


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



///helper--
int y_to_row(double y){
    int r =  (int) ((y - y_bottom_limit) / dy);
    if (r<0){
        return 0;
    }
    else if (r>=screen_height){
        return screen_height -1;
    }
    else return r;
}

int x_to_col(double x){
    int c =  (int) ((x - x_left_limit) / dx );
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


///-------------------Main functions start here-----------------------------------------------
void read_data(){

    ifstream config("config.txt"), stage3("stage3.txt");
    config>>screen_width>>screen_height;
    config>>x_left_limit>>y_bottom_limit;
    config>>z_front_limit>>z_rear_limit;



    ///read triangles--
    double x, y, z;
    int id=0;
    while(stage3>>x>>y>>z){
        Point points[3];
        points[0].set(x,y,z);
        stage3>>x>>y>>z;
        points[1].set(x,y,z);
        stage3>>x>>y>>z;
        points[2].set(x,y,z);

        Color col(rand()%256, rand()%256, rand()%256);
        triangles.push_back(Triangle(points, col, id++));

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
    absolute_miny = 99999.22;
    absolute_maxy = -absolute_miny;
}


void add_edge(double x1, double y1, double x2, double y2, int id){
    if(eq(y1, y2)) return;
    if(y1 > y2){
        swap(x1, x2);
        swap(y1,y2);
    }
    int row = y_to_row(y1);
    double del_x;

    del_x = (x2-x1)/(y2-y1);

    Edge e(y1, y2, del_x, id, x1);
    edge_table[row].push_back(e);

    absolute_miny = min(absolute_miny, y1);
    absolute_maxy = max(absolute_maxy, y2);
}

void add_edges(Triangle t){

    double x1, x2, x3, y1, y2, y3;
    int id = t.id;

    y1 = t.points[0].y;
    y2 = t.points[1].y;
    y3 = t.points[2].y;
    x1 = t.points[0].x;
    x2 = t.points[1].x;
    x3 = t.points[2].x;

    ///edges between points ---
    add_edge(x1, y1, x2, y2, id);
    add_edge(x1, y1, x3, y3, id);
    add_edge(x3, y3, x2, y2, id);
}

void initialize_edge_table_and_polygon_table(){
///polygon table already initialized in read_data-----------------
    edge_table = new vector<Edge>[screen_height];
    for (int k=0; k<triangles.size(); k++){
        active_polygon_table.push_back(false);
        add_edges(triangles[k]);
    }





    frame_buffer = new Color*[screen_width];
    for(int i=0; i<screen_width; i++){
        frame_buffer[i] = new Color[screen_height];
    }

}
void pre_process2(){

}

void apply_procedure(){
    double bottom_scanline, top_scanline, left_scanline, right_scanline;
    double bottom_row, top_row, left_col, right_col;
    double lowest, highest, leftmost, rightmost;
    double leftest, rightest;
    int kase;
    double x, y, z;
    double oldx, oldy;

    bottom_row = y_to_row(absolute_miny);
    top_row = y_to_row(absolute_maxy);
    //cout<<"BOTTOM: "<<bottom_row<<" TOP: "<<top_row<<endl;

    ///==============SCANLINE STARTS===============================
    for(int row = bottom_row; row<=top_row; row++){
        //update active_edge_table---

        ///at first update x for existing
        for(int k =0; k<active_edge_table.size(); k++){
            Edge e = active_edge_table[k];
            active_edge_table[k].ymin = e.ymin + dy;
            active_edge_table[k].x_at_ymin = e.x_at_ymin + dy*e.del_x;
        }
        ///add new edges
        for(int k = 0; k<edge_table[row].size(); k++){
            Edge e = edge_table[row][k];
            oldy = e.ymin;
            e.ymin = bottom_y + row*dy;
            e.x_at_ymin = e.x_at_ymin + (e.ymin - oldy)* e.del_x;
            active_edge_table.push_back(e);
        }
        ///remove invalid edges
        for(int k=0; k<active_edge_table.size(); k++){
            Edge e = active_edge_table[k];
            if (e.ymin > e.ymax){
                active_edge_table.erase(active_edge_table.begin()+k);
            }
        }
        sort(active_edge_table.begin(), active_edge_table.end(), compare_edges);
        //end update aet

        ///-for each edge but the last one
        for(int k = 0; k <active_edge_table.size()-1; k++){
            Edge e = active_edge_table[k];
            active_polygon_table[e.tr_id] = !active_polygon_table[e.tr_id];

            ///Now find the color
            double z_min = z_rear_limit;
            Color color(0,0,0);
            x = e.x_at_ymin;
            y = e.ymin;

            vector< pair<int, pair<double, double> > > active_triangles;
            for(int tr_id=0; tr_id<active_polygon_table.size(); tr_id++){
                if(!active_polygon_table[tr_id]) continue;
                //if(!Point)
                active_triangles.push_back(make_pair(tr_id, make_pair(x, y)));
            }
            sort(active_triangles.begin(), active_triangles.end(), compare_depth);
            ///color upto the next edge
            Edge next = active_edge_table[k+1];
            left_col = x_to_col(x);
            right_col = x_to_col(next.x_at_ymin);

            for(int col = left_col; col <= right_col; col++){
                ///clipping--------
                x = left_x + col*dx;
                y = bottom_y + row*dy;
                int winner = -1;
                for(int k=0; k<active_triangles.size(); k++){
                    int id = active_triangles[k].first;
                    Triangle t  = triangles[id];
                    if(!PointInTriangle(Point(x,y,0), t.points[0], t.points[1], t.points[2])) continue;
                    z = t.find_z(x, y);
                    if(z >= z_rear_limit) break;
                    winner = id;
                    break;
                }
                if(winner >=0) frame_buffer[col][screen_height-row-1] = triangles[winner].color;
            }

            //if(next.x_at_ymin > x_right_limit) break;

        }


        ///disable all active polygons
        for(int k = 0; k<active_polygon_table.size(); k++){
            active_polygon_table[k] = false;
        }//




    }

    ///


}//end_function

void save(){
    bitmap_image image(screen_width,screen_height);
    for(int i=0;i<screen_width;i++){
        for(int j=0;j<screen_height;j++){
            image.set_pixel(i,j,frame_buffer[i][j].r,frame_buffer[i][j].g,frame_buffer[i][j].b);
        }
    }
    image.save_image("2.bmp");

}

void post_process(){
    int kase = 0;
    for (int i=0; i<screen_height; i++){
        kase+= edge_table[i].size();
        for(int j=0; j<edge_table[i].size(); j++){
            cout<<"Edge: "; edge_table[i][j].print();
        }
    }
    cout<<"TOTAL EDGES: "<<kase<<endl;

}

void free_memory(){
    for(int i=0; i<screen_width; i++){
        delete frame_buffer[i];
    }
    delete[] frame_buffer;

    delete[] edge_table;
}

int main(){

    srand(34);

    read_data();
    pre_process();

    initialize_edge_table_and_polygon_table();
    pre_process2();

    apply_procedure();
    save();

   // post_process();
    free_memory();

    return 0;
}
