#include <pybind11/pybind11.h>
#include <cmath>
#include <stdexcept>

namespace py = pybind11;

/**
 * Calculate the angle (in radians) between two 2D vectors.
 * v1 = (x1, y1), v2 = (x2, y2)
 * Uses: theta = acos( (v1 . v2) / (|v1| * |v2|) )
 */
double angle_between(double x1, double y1, double x2, double y2)
{
    double dot = x1 * x2 + y1 * y2;
    double mag1 = std::sqrt(x1 * x1 + y1 * y1);
    double mag2 = std::sqrt(x2 * x2 + y2 * y2);

    if (mag1 == 0.0 || mag2 == 0.0)
        throw std::invalid_argument("Zero-length vector has no defined angle.");

    // Clamp to [-1, 1] to guard against floating-point rounding past ±1
    double cos_val = dot / (mag1 * mag2);
    cos_val = std::max(-1.0, std::min(1.0, cos_val));
    return std::acos(cos_val);
}

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "Compute the angle (radians) between two 2-D vectors via pybind11";
    m.def("angle_between", &angle_between,
          py::arg("x1"), py::arg("y1"),
          py::arg("x2"), py::arg("y2"),
          "Return the angle in radians between vectors (x1,y1) and (x2,y2).");
}