#ifndef LINE_HPP
#define LINE_HPP

#include <vector>
#include <stdexcept>

class Line {
public:
    Line() {}
    Line(size_t size) : m_x(size), m_y(size) {}

    // Copy Constructor (深拷貝：vector 會自動幫你複製內容)
    Line(Line const & other) : m_x(other.m_x), m_y(other.m_y) {}

    // Move Constructor (移動：直接接管資源)
    Line(Line && other) : m_x(std::move(other.m_x)), m_y(std::move(other.m_y)) {}

    // Copy Assignment
    Line & operator=(Line const & other) {
        if (this != &other) { m_x = other.m_x; m_y = other.m_y; }
        return *this;
    }

    // Move Assignment
    Line & operator=(Line && other) {
        if (this != &other) { m_x = std::move(other.m_x); m_y = std::move(other.m_y); }
        return *this;
    }

    ~Line() {}

    size_t size() const { return m_x.size(); }

    // 存取介面：左值版本 (可用於修改)
    float & x(size_t it) { return m_x.at(it); }
    float & y(size_t it) { return m_y.at(it); }

    // 存取介面：右值版本 (唯讀)
    float const & x(size_t it) const { return m_x.at(it); }
    float const & y(size_t it) const { return m_y.at(it); }

private:
    std::vector<float> m_x;
    std::vector<float> m_y;
};

#endif