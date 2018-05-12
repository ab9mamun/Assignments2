#include <bits/stdc++.h>
using namespace std;

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


void init(){
    open_files();
}


void finish(){

    close_files();
}
int main(){

    init();

    finish();
}
