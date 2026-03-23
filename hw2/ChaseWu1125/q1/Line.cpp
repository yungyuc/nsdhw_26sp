#include "Line.hpp"
#include <utility>

// default constructor
Line::Line() : m_x(), m_y() {}

// copy constructor
Line::Line(Line const & other) : m_x(other.m_x), m_y(other.m_y) {}

// move constructor
Line::Line(Line && other) : m_x(std::move(other.m_x)), m_y(std::move(other.m_y)) {}

// copy assignment operator
Line & Line::operator=(Line const & other)
{
    if (this != &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
    }
    return *this;
}

// move assignment operator
Line & Line::operator=(Line && other)
{
    if (this != &other)
    {
        m_x = std::move(other.m_x);
        m_y = std::move(other.m_y);
    }
    return *this;
}

// parameterized constructor
Line::Line(size_t size) : m_x(size, 0.0f), m_y(size, 0.0f) {}

// destructor
Line::~Line() = default;

// get the number of points in the line
size_t Line::size() const {
    return m_x.size();
}

// get the x-coordinate (read-only)
float const & Line::x(size_t it) const {
    return m_x.at(it);
}

// get the x-coordinate (read-write)
float & Line::x(size_t it) {
    return m_x.at(it);
}

// get the y-coordinate (read-only)
float const & Line::y(size_t it) const {
    return m_y.at(it);
}

// get the y-coordinate (read-write)
float & Line::y(size_t it) {
    return m_y.at(it);
}