//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/Square.hpp
#ifndef SQUARE_HPP
#define SQUARE_HPP
#include "Rectangle.hpp"
class Square : public Rectangle { // Square inherits from Rectangle
  public:
    Square();
    Square(double side);
    double side() const;
    void setSide(double side);
    // virtual void print() const override; // ok
    void print() const override; // better

  // No additional data members for a square compared with a rectangle
};
#endif
