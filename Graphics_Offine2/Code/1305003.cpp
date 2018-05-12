#include <bits/stdc++.h>
#define CAP 300
using namespace std;

void matcopy(double** to, double** from, int r=4, int c=4);
void matprint(double** mat, int r=4, int c=4);
void matmul(double** res, double** mat1, double**mat2, int r1=4, int c1 =4, int c2 = 4);
void randmat(double** mat, int r = 4, int c=4);
double** newmat(int r=4, int c=4);
void delmat(double** mat, int r=4, int c=4);
double** eye(int r=4);

void ASSERT(bool cond, const char* str){
    if(!cond){
        cout<<str<<endl;
        cout<<"Aborted"<<endl;
        exit(1);
    }
}


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
ofstream stage1, stage2, stage3;
double** top;
Stack* stk;




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
    int i, j, k;
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++)
        {
            res[i][j] = 0;
            for (k = 0; k < c1; k++)
                res[i][j] += mat1[i][k]*mat2[k][j];
        }
    }
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

double** eye(int r){
    double** mat = newmat(r,r);
    for(int i=0; i<r; i++){
        for(int j=0; j<r; j++)
            if(i==j) mat[i][j] = 1;
            else mat[i][j] = 0;
    }
    return mat;
}






void open_files(){
    scene.open("scene.txt");
    stage1.open("stage1.txt");
    stage2.open("stage2.txt");
    stage3.open("stage3.txt");
}

void close_files(){
    scene.close();
    stage1.close();
    stage2.close();
    stage3.close();
}

void finish(){

}

int main(){

    srand(3434); ///seed--
    open_files();
    top = newmat();
    stk = new Stack();



    double** I = eye(4);
    double** A = newmat();
    randmat(A);
    double**B = newmat();
    randmat(B);
    double** C = newmat();
  //  matprint(A);
   // matprint(B);

    stk->push(I);
    stk->push(A);

    matmul(C, A, B);
    stk->push(C);

    while(!stk->empty()){
        stk->pop(top);
        matprint(top);
    }




    close_files();
    delmat(I);
    delmat(A);
    delmat(B);
    delmat(C);
    delmat(top);


}
