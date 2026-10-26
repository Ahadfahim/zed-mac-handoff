// Buffer.hpp -- from lecture slip on Sepetember 15, 2026
// Declarations only -- see Buffer.cpp for implementations.
#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstddef>

class Buffer {
public:
    explicit Buffer(std::size_t size);
    ~Buffer();

    Buffer(const Buffer& other);

    // NEW today -- buggy, see grow() in Buffer.cpp
    void grow(std::size_t newSize);

    int& at(std::size_t i);
    std::size_t size() const;
    
    Buffer& operator= (const Buffer& other);

private:
    int* data_;
    std::size_t size_;
};

#endif
