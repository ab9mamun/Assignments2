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


int main(){
    cout<<max(2.0,34.3, 22.0)<<endl;
    cout<<min(-23.0, 33.0, 11.0)<<endl;
    cout<<min(2, 2.3)<<endl;
    return 0;
}

