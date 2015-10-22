

#include <iostream>

using namespace std;

class trapSlave{
    public:
        float left, right;  // endpoints of individual trapezoid
        float delta;
        float arbFunction(float x){
            return (x*x) + (2*x) + 4;
        }
        float area(){
            cout << "       f(left): " << arbFunction(left) << endl;
            cout << "       f(right): " << arbFunction(right) << endl;
            return (((arbFunction(left) + arbFunction(right))/2) * delta);
        }
        
};