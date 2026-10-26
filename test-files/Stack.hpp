// Stack.hpp
#ifndef STACK_HPP
#define STACK_HPP
template <typename T>
class Stack {
  public:
    void push(const T& value);
    // For simplicity, no pop, top, size, empty methods in this example.
    
  private:
    T data_[100]; // Assume that there are at most 100 elements in the stack.
    int size_ = 0;
};

// For a template class, including the .tpp file at the end of the .hpp file 
// is IMPORTANT.
// Template definitions must be visible when the compiler 
// instantiates the template.
#include "Stack.tpp" 

#endif
