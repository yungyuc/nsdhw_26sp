#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace py = pybind11;

double calculate_angle(std::vector<double> v1, std::vector<double> v2) {
    if (v1.size() != 2 || v2.size() != 2) {
        throw std::invalid_argument("Vectors must be 2-dimensional.");
    }
    
    double mag1 = std::sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    double mag2 = std::sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    
    if (mag1 == 0.0 || mag2 == 0.0) {
        throw std::invalid_argument("Vector length cannot be zero.");
    }
    
    double dot = v1[0] * v2[0] + v1[1] * v2[1];
    
    double val = dot / (mag1 * mag2);
    if (val > 1.0) val = 1.0;
    if (val < -1.0) val = -1.0;
    
    return std::acos(val);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "C++ module for vector operations";
    m.def("angle", &calculate_angle, "Calculates the angle (in radians) between two 2D vectors");
}