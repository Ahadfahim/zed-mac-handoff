// Buffer.cpp -- all Buffer member functions are implemented here.
#include "Buffer.hpp"

Buffer::Buffer(std::size_t size)
    : data_(new int[size]{}), size_(size) {
}

Buffer::~Buffer() {
    delete[] data_;
}

Buffer::Buffer(const Buffer& other)
    : data_(new int[other.size_]), size_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

// Buffer::grow() is NEW code, added today -- buggy on purpose, see the
// lecture slip's Q1.
void Buffer::grow(std::size_t newSize) {
    int* newData = new int[newSize]{};
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
    }
    data_ = newData;
    size_ = newSize;
}

int& Buffer::at(std::size_t i) {
    return data_[i];
}

std::size_t Buffer::size() const {
    return size_;
}

// --- FIXED grow() (for after the demo) ----------------------------------
// void Buffer::grow(std::size_t newSize) {
//     int* newData = new int[newSize]{};
//     for (std::size_t i = 0; i < size_; ++i) {
//         newData[i] = data_[i];
//     }
//     delete[] data_;      // free the OLD array first
//     data_ = newData;
//     size_ = newSize;
// }

Buffer& Buffer::operator= (const Buffer& other) {
    if (this == &other) {
        return *this;
    }

    int* newData = new int[other.size_]; // allocate
    for (std::size_t i = 0; i < other.size_; i++) { // copy
        // cannot write other.size_ by size_ since size_ is not updated yet
        newData[i] = other.data_[i];
    }

    delete[] data_; // release old data
    data_ = newData;
    
    size_ = other.size_; // set data member size_

    return *this;
}
