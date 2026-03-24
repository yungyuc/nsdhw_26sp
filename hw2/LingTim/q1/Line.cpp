#include "Line.h"
#include <utility> // for std::move

Line::Line() {}

Line::Line(Line const & other) : m_x(other.m_x), m_y(other.m_y) {}

Line::Line(Line && other) : m_x(std::move(other.m_x)), m_y(std::move(other.m_y)) {}

Line & Line::operator=(Line const & other) {
    if (this != &other) {
        m_x = other.m_x;
        m_y = other.m_y;
    }
    return *this;
}

Line & Line::operator=(Line && other) {
    if (this != &other) {
        m_x = std::move(other.m_x);
        m_y = std::move(other.m_y);
    }
    return *this;
}

Line::Line(size_t size) : m_x(size, 0.0f), m_y(size, 0.0f) {}

Line::~Line() {}

size_t Line::size() const { 
    return m_x.size(); 
}

float const & Line::x(size_t it) const { 
    return m_x[it]; 
}

float & Line::x(size_t it) { 
    return m_x[it]; 
}

float const & Line::y(size_t it) const { 
    return m_y[it]; 
}

float & Line::y(size_t it) { 
    return m_y[it]; 
}
