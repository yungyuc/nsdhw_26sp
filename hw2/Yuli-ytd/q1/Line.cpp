#include <utility>
#include <cstddef>
#include "Line.hpp"

Line::Line(){}
Line::Line(Line const & other) : m_x(other.m_x), m_y(other.m_y){}
Line::Line(Line &&other)
    : m_x(std::move(other.m_x)), 
      m_y(std::move(other.m_y)){}
Line::Line(std::size_t size):m_x(size), m_y(size){}
Line & Line::operator=(Line const & other){
    if(this == &other) return *this;

    m_x = other.m_x;
    m_y = other.m_y;

    return *this;
}
Line & Line::operator=(Line && other){
    if(this == &other) return *this;
    
    m_x = std::move(other.m_x);
    m_y = std::move(other.m_y);

    return *this;
}
std::size_t Line::size() const{ return m_x.size(); }
float const & Line::x(std::size_t it) const { return m_x[it]; }
float const & Line::y(std::size_t it) const { return m_y[it]; }
float & Line::x(std::size_t it) { return m_x[it]; }
float & Line::y(std::size_t it) { return m_y[it]; }