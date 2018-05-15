#include <bits/stdc++.h>
#define CAP 300
#define pi (2*acos(0.0))
using namespace std;

void matcopy(double** to, double** from, int r=4, int c=4);
void matprint(double** mat, int r=4, int c=4);
void matmul(double** res, double** mat1, double**mat2, int r1=4, int c1 =4, int c2 = 4);
void randmat(double** mat, int r = 4, int c=4);
double** newmat(int r=4, int c=4);
void delmat(double** mat, int r=4, int c=4);
double** matI(int r=4);
void transpose(double** res, double** mat, int r=4, int c=4);


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
                 -(x * v.z - z * v.x),
                 x * v.y - y * v.x);
        return w;
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


class Stack{
    double*** mats;
    int top;
public:

    Stack(){
        top = 0;
        mats = new double**[CAP];
        for(int k=0; k<CAP; k++){
            mats[k] = newmat();
        }
    }

    void push(double** mat){
        ASSERT(top<CAP, "Stack overrunning");
        matcopy(mats[top], mat);
        top++;
    }

    void pop(double** mat){
        ASSERT(top>0, "Stack underrunning");
        top--;
        matcopy(mat, mats[top]);

    }
    bool empty(){
        return top==0;
    }

    ~Stack(){
        for(int k=0; k<CAP; k++){
            delmat(mats[k]);
        }
        delete[] mats;
        mats = NULL;
    }

};

///-------global variables------------------
ifstream scene;
FILE* stage1, *stage2, *stage3;
double** top;
Stack* stk;
double eye[3]; ///eyeX, eyeY, eyeZ
double look[3]; ///lookX, lookY, lookZ
double up[3]; ///upX, upY, upZ
double gluPerspective[4]; ///fovY, aspectRatio, near, far
double x[] = {1,0,0};
double y[] = {0,1,0};
double z[] = {0, 0, 1};


void matcopy(double** to, double** from, int r, int c){
    for(int i=0; i<r; i++)
        for(int j=0; j<c; j++)
            to[i][j] = from[i][j];

}

void matprint(double** mat, int r, int c){
    cout<<"Matrix:"<<endl;
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            cout<<mat[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void matmul(double** res, double** mat1, double** mat2, int r1, int c1, int c2) {
    double** temp = newmat(r1, c2);
    int i, j, k;
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++)
        {
            temp[i][j] = 0;
            for (k = 0; k < c1; k++){
                temp[i][j] += mat1[i][k]*mat2[k][j];
               // cout<<mat1[i][k]<<" "<<mat2[k][j]<<endl;

            }
        }
    }
    matcopy(res, temp, r1, c2);
    delmat(temp);
}

void randmat(double** mat, int r, int c){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++) {
            double f = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
            mat[i][j] = f*100 - 50;
        }

}

double** newmat(int r, int c){
    double** mat = new double*[r];
    for(int i=0; i<r; i++){
        mat[i] = new double[c];
    }
    return mat;
}
void delmat(double** mat, int r, int c){
    for(int i=0; i<r; i++){
        delete[] mat[i];
    }
    delete[] mat;
    mat = NULL;
}

double** matI(int r){
    double** mat = newmat(r,r);
    for(int i=0; i<r; i++){
        for(int j=0; j<r; j++)
            if(i==j) mat[i][j] = 1;
            else mat[i][j] = 0;
    }
    return mat;
}

void transpose(double** res, double** mat, int r, int c){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            res[j][i] = mat[i][j];
    }
}

void arrayprint(double a[], int l){
    cout<<"Array:"<<endl;
    for(int i=0; i<l; i++)
        cout<<a[i]<<" ";
    cout<<endl<<endl;
}




void open_files(){
    scene.open("scene.txt");

    stage1 = fopen("stage1.txt", "w");
    stage2 = fopen("stage2.txt", "w");
    stage3 = fopen("stage3.txt", "w");

}

void close_files(){
    scene.close();
    fclose(stage1);
    fclose(stage2);
    fclose(stage3);
}

void init(){
    srand(3434); ///seed--
    open_files();
    top = newmat();
    stk = new Stack();

    for(int i=0; i<3; i++)
        scene>>eye[i];
    for(int i=0; i<3; i++)
        scene>>look[i];
    for(int i=0; i<3; i++)
        scene>>up[i];
    for(int i=0; i<4; i++)
        scene>>gluPerspective[i];

   // arrayprint(eye, 3);
  //  arrayprint(look, 3);
   // arrayprint(up, 3);
  //  arrayprint(gluPerspective, 4);
}

void finish(){
    close_files();
    delmat(top);
    delete stk;
}

double rad(double deg_angle){
    return deg_angle*pi/180.0;
}
double deg(double rad_angle){
    return rad_angle*180.0/pi;
}

Vector R(Vector v, Vector a, double angle){
   double c, s;
   c = cos(rad(angle));
   s = sin(rad(angle));

   return c*v + (1-c)* a.dot(v)* a + s*(a*v);
}


///-- io

void input_triangle(ifstream& in, double** p1, double** p2, double** p3){
    for(int i=0; i<3; i++)
        in>>p1[i][0];
    p1[3][0] = 1;
    for(int i=0; i<3; i++)
        in>>p2[i][0];
    p2[3][0] = 1;
    for(int i=0; i<3; i++)
        in>>p3[i][0];
    p3[3][0] = 1;
}


void output_triangle(FILE* out, double** p1t, double** p2t, double**p3t){
    for(int i=0; i<3; i++)
        fprintf(out,"%.7lf ", p1t[i][0]);
    fprintf(out, "\n");
    for(int i=0; i<3; i++)
        fprintf(out,"%.7lf ", p2t[i][0]);
    fprintf(out, "\n");
    for(int i=0; i<3; i++)
        fprintf(out,"%.7lf ", p3t[i][0]);
    fprintf(out, "\n\n");
}

int main(){

    init();

    double** I = matI(4);
    double** A = newmat();
    double**B = newmat();
    double** C = newmat();

    double** p1, **p2, **p3;
    p1 = newmat(4,1);
    p2 = newmat(4,1);
    p3 = newmat(4,1);
    double** p1t, **p2t, **p3t;
    p1t = newmat(4,1);
    p2t = newmat(4,1);
    p3t = newmat(4,1);



    Vector c1, c2, c3, a;
    double angle;
  //  matprint(A);
   // matprint(B);
   /// stage 1
    matcopy(top, I);
    stk->push(I);
    string str;
    while(true){
        scene>>str;
        if(str == "end") break;
        //cout<<str<<endl;
        if(str == "push")
            stk->push(top);
        else if(str=="pop")
            stk->pop(top);
        else if(str == "translate"){
            //continue;
            matcopy(A, I);
            for(int i = 0; i<3; i++)
                scene>>A[i][3];
            matmul(top, top, A);

        }
        else if(str == "rotate"){
                //continue;
            scene>>angle;
            scene>>a.x>>a.y>>a.z;

            a = a.unit();

            c1 = R(Vector::X() , a, angle);
            c2 = R(Vector::Y(), a, angle);
            c3 = R(Vector::Z(), a, angle);
            matcopy(A, I);
            for(int i=0; i<3; i++) {
                A[i][0] = c1[i];
                A[i][1] = c2[i];
                A[i][2] = c3[i];
            }
            matmul(top, top, A);

        }
        else if(str == "scale"){
                //continue;
            matcopy(A, I);
            for(int i = 0; i<3; i++)
                scene>>A[i][i];
          //  matprint(A);
           // matprint(top);
            matmul(top, top, A);
           // matprint(top);
        }
        else if(str == "triangle"){


            input_triangle(scene, p1, p2, p3);
            ///stage1
            ///modeling transformation-- top = Transform matrix
            matmul(p1t, top, p1, 4, 4, 1);
            matmul(p2t, top, p2, 4, 4, 1);
            matmul(p3t, top, p3, 4, 4, 1);

            output_triangle(stage1, p1t, p2t, p3t);

            ///stage2
             ///View Transformation---- A = T, B = R
            Vector l(look[0] - eye[0], look[1] - eye[1], look[2] - eye[2]);
            l = l.unit();
            //cout<<l.x<<" "<<l.y<<" "<<l.z<<endl;
            Vector upp(up[0], up[1], up[2]);
            //cout<<upp.x<<" "<<upp.y<<" "<<upp.z<<endl;
            Vector r = l*upp;
            r = r.unit();
            //cout<<r.x<<" "<<r.y<<" "<<r.z<<endl;


            Vector u = r*l;
            //u = u.unit();
            //cout<<u.x<<" "<<u.y<<" "<<u.z<<endl;



            matcopy(A, I);
            for(int i=0; i<3; i++){
                A[i][3] = -eye[i];
            }

            matcopy(B, I);
            for(int j = 0; j<3; j++){
                B[0][j] = r[j];
                B[1][j] = u[j];
                B[2][j] = -l[j];
            }

            matprint(A);
            matprint(B);

            matmul(C, B, A);  ///major bugfix -----this should not be AB--------
            matmul(C, C, top);

            matmul(p1t, C, p1, 4, 4, 1);
            matmul(p2t, C, p2, 4, 4, 1);
            matmul(p3t, C, p3, 4, 4, 1);

            output_triangle(stage2, p1t, p2t, p3t);

            ///stage3 ---
            ///Projection -- C = P--- need to normalize by weight at the end...
            double fovY, fovX, near, far, t_, r_;
            fovY = gluPerspective[0];
            fovX = gluPerspective[0]*gluPerspective[1];
            near = gluPerspective[2];
            far = gluPerspective[3];

            t_ = near*tan(rad(fovY/2));
            r_ = near*tan(rad(fovX/2));

            matcopy(A, I);
            A[0][0] = near/r_;
            A[1][1] = near/t_;
            A[2][2] = -(far+near)/(far -near);
            A[2][3] = -(2*far*near) / (far - near);
            A[3][2] = -1;
            A[3][3] = 0;
            //matprint(A);

            matmul(C, A, C);
            matmul(p1t, C, p1, 4, 4, 1);
            matmul(p2t, C, p2, 4, 4, 1);
            matmul(p3t, C, p3, 4, 4, 1);
            for(int i = 0; i<4; i++){  ///bugfix, normalizing by the weight--
                p1t[i][0] = p1t[i][0] / p1t[3][0];
                p2t[i][0] = p2t[i][0] / p2t[3][0];
                p3t[i][0] = p3t[i][0] / p3t[3][0];
            }

            output_triangle(stage3, p1t, p2t, p3t);
        }

    }

    delmat(I);
    delmat(A);
    delmat(B);
    delmat(C);
    delmat(p1);
    delmat(p2);
    delmat(p3);
    delmat(p1t);
    delmat(p2t);
    delmat(p3t);

    finish();


}
