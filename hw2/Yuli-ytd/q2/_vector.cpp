#include <cmath>
#include <stdexcept>
#include <pybind11/pybind11.h>

double calculate_angle(double x1, double y1, double x2, double y2){
    
    double dot, sq_len1, sq_len2, input;
    double eps = 1e-12;

    dot = x1 * x2 + y1 * y2;

    sq_len1 = x1 * x1 + y1 * y1;
    sq_len2 = x2 * x2 + y2 * y2;

    if(sq_len1 < eps || sq_len2 < eps){
        throw std::runtime_error("Zero-length vector is invalid.");
    }
    
    input = dot / (std::sqrt(sq_len1) * std::sqrt(sq_len2));
    if(input > 1.0) input = 1.0;
    if(input < -1.0) input = -1.0;

    return std::acos(input);
}

PYBIND11_MODULE(_vector, m){
    
    m.doc() = "pybind11 plugin for vector angle calculation";

    m.def("calculate_angle", &calculate_angle, "Calculate the angle between two 2D vectors");
}