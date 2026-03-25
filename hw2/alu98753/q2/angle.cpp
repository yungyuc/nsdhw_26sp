#include <pybind11/pybind11.h>

#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace py = pybind11;

double angle(double x1, double y1, double x2, double y2)
{
    double len1 = std::sqrt(x1 * x1 + y1 * y1);
    double len2 = std::sqrt(x2 * x2 + y2 * y2);

    // check if len1 or len2 is zero , edge case
    if (len1 == 0.0 || len2 == 0.0)
    {
        throw std::invalid_argument(
            "Cannot calculate angle with zero-length vector"
        );
    }

    double dot = x1 * x2 + y1 * y2;
    double cos_theta = dot / (len1 * len2);

    // clamp cos_theta to [-1, 1] to avoid floating point errors
    cos_theta = std::clamp(cos_theta, -1.0, 1.0);

    // use acos to calculate the angle
    return std::acos(cos_theta);
}

PYBIND11_MODULE(_vector, m)
{
    m.def("angle", &angle, "Calculate angle between two 2D vectors");
}
