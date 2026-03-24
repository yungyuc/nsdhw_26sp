#include <array>
#include <cmath>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

double angle_between(const std::array<double, 2> & a,
                     const std::array<double, 2> & b)
{
    double ax = a[0];
    double ay = a[1];
    double bx = b[0];
    double by = b[1];

    double norm_a = std::sqrt(ax * ax + ay * ay);
    double norm_b = std::sqrt(bx * bx + by * by);

    if (norm_a == 0.0 || norm_b == 0.0)
    {
        throw std::invalid_argument("zero-length vector");
    }

    double dot = ax * bx + ay * by;
    double cos_theta = dot / (norm_a * norm_b);
    if (cos_theta > 1.0)
    {
        cos_theta = 1.0;
    }
    else if (cos_theta < -1.0)
    {
        cos_theta = -1.0;
    }

    return std::acos(cos_theta);
}

namespace py = pybind11;

PYBIND11_MODULE(_vector, m)
{
    m.doc() = "2D vector angle module";
    m.def("angle_between", &angle_between, "angle between two 2D vectors");
}
