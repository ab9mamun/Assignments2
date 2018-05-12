#include <bits/stdc++.h>
#define CAP 300
using namespace std;

void matcopy(int** to, int** from, int r=4, int c=4);
void matprint(int** mat, int r=4, int c=4);
void matmul(int** res, int** mat1, int**mat2, int r1=4, int c1 =4, int c2 = 4);
void randmat(int** mat, int r = 4, int c=4);
int** newmat(int r=4, int c=4);
void delmat(int** mat, int r=4, int c=4);
int** eye(int r=4);

void ASSERT(bool cond, const char* str){
    if(!cond){
        cout<<str<<endl;
        cout<<"Aborted"<<endl;
        exit(1);
    }
}


class Stack{
public:
    int*** mats;
    int top;

    Stack(){
        top = 0;
        mats = new int**[CAP];
        for(int k=0; k<CAP; k++){
            mats[k] = newmat();
        }
    }

    void push(int** mat){
        ASSERT(top<CAP, "Stack overrunning");
        matcopy(mats[top], mat);
        top++;
    }

    void pop(int** mat){
        ASSERT(top>0, "Stack underrunning");
        top--;
        matcopy(mat, mats[top]);

    }

    ~Stack(){
        for(int k=0; k<CAP; k++){

            for(int i=0; i<4; i++){
               delete[] mats[k][i];
            }
            delete[] mats[k];
        }
        delete[] mats;
        mats = NULL;
    }

};

void matcopy(int** to, int** from, int r, int c){
    cout<<"Here I am"<<endl;
    for(int i=0; i<r; i++)
        for(int j=0; j<c; j++)
            to[i][j] = from[i][j];

}

void matprint(int** mat, int r, int c){
    cout<<"Matrix:"<<endl;
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++)
            cout<<mat[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void matmul(int** res, int** mat1, int** mat2, int r1, int c1, int c2) {
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

void randmat(int** mat, int r, int c){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            mat[i][j] = rand()%100 - 50;
}

int** newmat(int r, int c){
    int** mat = new int*[r];
    for(int i=0; i<r; i++){
        mat[i] = new int[c];
    }
    return mat;
}
void delmat(int** mat, int r, int c){
    for(int i=0; i<r; i++){
        delete[] mat[i];
    }
    delete[] mat;
    mat = NULL;
}

int** eye(int r){
    int** mat = newmat(r,r);
    for(int i=0; i<r; i++){
        for(int j=0; j<r; j++)
            if(i==j) mat[i][j] = 1;
            else mat[i][j] = 0;
    }
    return mat;
}

///-------global variables------------------
ifstream scene;
ofstream stage1, stage2, stage3;
int top[4][4];




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


    int** I = eye(4);
    int** A = newmat();
    randmat(A);
    int**B = newmat();
    randmat(B);
    int** C = newmat();
    matprint(A);
    matprint(B);

    matmul(C, A, I);
    matprint(C);

    close_files();
    delmat(I);
    delmat(A);
    delmat(B);
    delmat(C);


}
