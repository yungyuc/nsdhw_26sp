#include <pybind11/pybind11.h>
#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace py = pybind11;

double calculate_angle(double x1, double y1, double x2, double y2) {
    // Check if either vector has zero length
    if ((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0)) {
        throw std::invalid_argument("Vector length cannot be zero.");
    }

    // Calculate the angle using atan2(cross product, dot product)
    double dot = x1 * x2 + y1 * y2;
    double cross = x1 * y2 - y1 * x2;
    double angle = std::atan2(cross, dot);

    return std::abs(angle);
}

PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 module for calculating the angle between two vectors.";
    m.def(
        "angle",
        &calculate_angle, 
        "Calculate the angle between two 2D vectors",
        py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2")
    );
}
