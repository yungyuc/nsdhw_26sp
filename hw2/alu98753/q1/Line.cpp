#include "Line.hpp"

Line::Line() {}

Line::Line(Line const & other)
    : m_x(other.m_x)
    , m_y(other.m_y) // copy constructor
{
}

Line::Line(Line && other)
    : m_x(std::move(other.m_x))
    , m_y(std::move(other.m_y)) // move constructor
{
}

Line & Line::operator=(Line const & other)
{
    if (this != &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
    }
    return *this; // return *this to support chain assignment
}

Line & Line::operator=(Line && other) // move assignment operator
{
    if (this != &other)
    {
        m_x = std::move(other.m_x);
        m_y = std::move(other.m_y);
    }
    return *this;
}

Line::Line(size_t size)
    : m_x(size, 0)  // create size x coordinates, initialized to 0
    , m_y(size, 0)
{
}

Line::~Line() {}

size_t Line::size() const
{
    return m_x.size();
}

float const & Line::x(size_t it) const
{
    return m_x[it];
}

float & Line::x(size_t it)
{
    return m_x[it];
}

float const & Line::y(size_t it) const
{
    return m_y[it];
}

float & Line::y(size_t it)
{
    return m_y[it];
}
