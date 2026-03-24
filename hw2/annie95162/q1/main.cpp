#include <iostream>
#include <vector>
#include <utility>   // std::move
#include <cstddef>   // size_t

class Line
{
public:
    Line() : m_points() {}

    Line(size_t size) : m_points(size) {}

    Line(Line const & other) : m_points(other.m_points) {}

    Line(Line && other) : m_points(std::move(other.m_points)) {}

    Line & operator=(Line const & other)
    {
        if (this != &other)
        {
            m_points = other.m_points;
        }
        return *this;
    }

    Line & operator=(Line && other)
    {
        if (this != &other)
        {
            m_points = std::move(other.m_points);
        }
        return *this;
    }

    ~Line() {}

    size_t size() const
    {
        return m_points.size();
    }

    float const & x(size_t it) const
    {
        return m_points.at(it).x;
    }

    float & x(size_t it)
    {
        return m_points.at(it).x;
    }

    float const & y(size_t it) const
    {
        return m_points.at(it).y;
    }

    float & y(size_t it)
    {
        return m_points.at(it).y;
    }

private:
    struct Point
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    std::vector<Point> m_points;
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
