#include <pybind11/pybind11.h>
#include <cmath>
#include <pybind11/stl.h>
#include <iostream>
#include <numbers>
#include <vector>

namespace py = pybind11;

std::vector<float> normalized(std::vector<float> vec){
    
    float norm= sqrt(pow(vec[0],2)+pow(vec[1],2));
    
    return {vec[0]/norm,vec[1]/norm};

    
}

float radian(std::vector<float>  a, std::vector<float> b){
    float result=0.0f;
    if(!a.empty() && !b.empty() && a.size()==2 && b.size()==2){
        a=normalized(a);
        b=normalized(b);
        float check = a[0]*b[1]-a[1]*b[0];
        float dot = a[0]*b[0]+a[1]*b[1];
        if (dot<=-1){
            dot = -1;
        }else if(dot >=1){
            dot = 1;
        }
        if(check<0){
            result=acos(dot)+acos(-1);
        }else{
            result=acos(dot);
        }
        
    }
    return result;
};

PYBIND11_MODULE(_vector, m) {
    m.def("radian",&radian,"A function to calculate radian", py::arg("a"),py::arg("b"));
}

