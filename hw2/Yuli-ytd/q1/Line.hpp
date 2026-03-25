#include <vector>
#include <cstddef>

class Line{
public:

    Line();
    Line(Line const & );
    Line(Line       &&);
    Line(std::size_t size);
    Line & operator=(Line const & );
    Line & operator=(Line       &&);
    std::size_t size() const;
    float const & x(std::size_t it) const; // read only
    float const & y(std::size_t it) const;
    float & x(std::size_t it); // read and write
    float & y(std::size_t it);

private:

    std::vector<float> m_x;
    std::vector<float> m_y;

};