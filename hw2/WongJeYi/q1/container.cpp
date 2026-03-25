#include <vector>
#include <iostream>
class Container
{
public:
    size_t m_size;
    std::vector<int> x_vec;
    std::vector<int> y_vec;
    int& x(int i){
        return x_vec[i];
    }
    int& y(int i){
        return y_vec[i];
    }
    Container();
    Container(Container const &);
    Container(Container &&);
    Container & operator=(Container const &);
    Container & operator=(Container  &&);
    Container(size_t size){
        m_size=size;
        x_vec=std::vector<int>(m_size);
        y_vec=std::vector<int>(m_size);
    }
    Container container();
    ~Container(){}
       
    
private:
    // Member data.
    //Copy constructor
    
}; /* end class Container */

Container::Container(Container const & other){
    if(other.m_size){
        m_size=other.m_size;
        x_vec=other.x_vec;
        y_vec=other.y_vec;
    }else{
        if(m_size){
            m_size=0;
        }
    }
}
    
    //Move constructor
Container::Container(Container       && other){
    m_size=other.m_size;
    x_vec=other.x_vec;
    y_vec=other.y_vec;
}
    //Copy assignment operator
Container & Container::operator=(Container const & other){
    if(this==&other){
        return *this;
    }
    if(other.m_size){
        m_size=other.m_size;
        x_vec=other.x_vec;
        y_vec=other.y_vec;
    }else{
        if(m_size){
            m_size=0;
        }
    }
    return *this;
}
//move assignement operator
Container & Container::operator=(Container && other){
    if (this == &other) { return *this; }
    m_size=other.m_size;
    x_vec=other.x_vec;
    y_vec=other.y_vec;
    return *this;
}
int main(int, char **)
{
    Container container(3);
    container.x(0) = 0; container.y(0) = 1;
    container.x(1) = 1; container.y(1) = 3;
    container.x(2) = 2; container.y(2) = 5;

    Container container2(container);
    container2.x(0) = 9;

    std::cout << "line: number of points = " << container.m_size << std::endl;
    for (size_t it=0; it<container.m_size; ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << container.x(it)
                  << " y = " << container.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << container.m_size << std::endl;
    for (size_t it=0; it<container.m_size; ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << container2.x(it)
                  << " y = " << container2.y(it) << std::endl;
    }

    return 0;
}