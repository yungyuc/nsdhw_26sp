#ifndef ALLOC_HPP
#define ALLOC_HPP

#include <cstdlib>
#include <cstddef>
#include <new>
#include <atomic>

struct ByteCounter{
    std::atomic_size_t g_bytes{0};
    std::atomic_size_t g_allocated{0};
    std::atomic_size_t g_deallocated{0};
};
extern ByteCounter g_counter;

template <class T>
struct CustomAllocator{
    using value_type = T;

    CustomAllocator() = default;

    T* allocate(std::size_t n){
        
        if(n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();


        std::size_t bytes = n * sizeof(T);
        T* p = static_cast<T *>(std::malloc(bytes));
        if(p){
            g_counter.g_bytes += bytes;
            g_counter.g_allocated += bytes;
            return p;
        }
        else throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t n) noexcept{
        
        std::size_t bytes = n * sizeof(T);
        g_counter.g_bytes -= bytes;
        g_counter.g_deallocated += bytes;

        std::free(p);
    }
};

template <typename T, typename U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&) { return false; }

#endif