#include <iostream>
#include <vector>
#include <stdexcept>

class Line
{
public:
    Line() = default;
    Line(Line const & other) = default;
    Line(Line && other) = default;
    Line & operator=(Line const & other) = default;
    Line & operator=(Line && other) = default;

    Line(size_t size)
        : m_x(size, 0.0f), m_y(size, 0.0f)
    {}

    ~Line() = default;

    size_t size() const { return m_x.size(); }

    float const & x(size_t it) const { return m_x.at(it); }
    float       & x(size_t it)       { return m_x.at(it); }

    float const & y(size_t it) const { return m_y.at(it); }
    float       & y(size_t it)       { return m_y.at(it); }

private:
    std::vector<float> m_x;
    std::vector<float> m_y;
};

int main(int, char **)
{
    Line line(3);
    line.x(0) = 0; line.y(0) = 1;
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line);
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}