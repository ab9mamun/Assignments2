#include <bits/stdc++.h>

#define CAP 300
#define pi (2*acos(0.0))
using namespace std;



int main(){
    vector<int> vec;
    vec.push_back(23);
    vec.push_back(22);
    vec.push_back(88);
    vec.erase(vec.begin()+ 1);

    for(int i=0; i<vec.size(); i++){
        cout<<vec[i]<<endl;
    }
    return 0;
}

