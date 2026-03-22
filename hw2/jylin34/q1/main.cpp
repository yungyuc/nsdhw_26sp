#include <cstddef>
#include <iostream>
#include <vector>

struct Point {
    float x;
    float y;
};

class Line {
public:
   Line(size_t size) { // constructor
       m_points.resize(size);
   }
   Line(const Line& line) = default; // copy constructor, actually takes O(N) of time
   size_t size() const { // tell compiler that it is a read-only function
       return m_points.size();
   }
   float& x(size_t it) { // x setter 
       return m_points[it].x;
   }
   const float& x(size_t it) const { // x getter
       return m_points[it].x;
   }
   float& y(size_t it) { // y setter 
       return m_points[it].y;
   }
   const float& y(size_t it) const { // y getter 
       return m_points[it].y;
   }
private:
    std::vector<Point> m_points;
};

int main(int, char **) {
    Line line(3); // constructor
    line.x(0) = 0; line.y(0) = 1; // use setter
    line.x(1) = 1; line.y(1) = 3;
    line.x(2) = 2; line.y(2) = 5;

    Line line2(line); // copy constructor ?
    line2.x(0) = 9;

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it) {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it) {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}
