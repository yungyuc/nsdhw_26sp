#include <pybind11/pybind11.h>
#include <cmath>
#include <stdexcept>

double calculate_angle(double x1, double y1, double x2, double y2) {
    // calculate dot_product
    double dot_product = x1 * x2 + y1 * y2;

    // calculate the length of two vectors
    double mag1 = std::sqrt(std::pow(x1, 2) + std::pow(y1, 2));
    double mag2 = std::sqrt(std::pow(x2, 2) + std::pow(y2, 2));

    if (mag1 == 0 || mag2 == 0) {
        throw std::invalid_argument("Input vector cannot be a zero vector.");
    }

    double cos_theta = dot_product / (mag1 * mag2);

    if (cos_theta >= 1.0) {
        return 0.0; // If cos is 1 or more, angle is 0
    }
    if (cos_theta <= -1.0) {
        return std::acos(-1.0); // If cos is -1 or less, angle is pi
    }

    return std::acos(cos_theta);
}

namespace py = pybind11;

PYBIND11_MODULE(_vector, m) {
    m.def("calculate_angle", &calculate_angle, "A function that calculates the angle (in radians) between two 2D vectors",
          py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"));
}
