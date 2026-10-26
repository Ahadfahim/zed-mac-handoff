//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/Rectangle.cpp

#include <iostream> // std::cout
#include "Rectangle.hpp"

Rectangle::Rectangle() : length_(2), width_(1) { // set length and width to be 2 and 1 respectively
}

Rectangle::Rectangle(double length, double width)  : length_(length > 0 ? length : 2), width_(width > 0 ? width : 1) {

}

Rectangle& Rectangle::operator=(const Rectangle& rect) {
  length_ = rect.length_;
  width_ = rect.width_;
  return *this;
}

Rectangle::Rectangle(const Rectangle& rect) : length_(rect.length_), width_(rect.width_) {
  
}

double Rectangle::length() const {
  return length_;
}

double Rectangle::width() const {
  return width_;
}

double Rectangle::area() const {
  return length_ * width_;
}

double Rectangle::perimeter() const {
  return 2 * length_ + 2 * width_;
}

void Rectangle::setLength(double length) {
  if (length > 0) {
    length_ = length;
  }
}

void Rectangle::setWidth(double width) {
  if (width > 0) {
    width_ = width;
  }
}

void Rectangle::print() const {
  std::cout << "rectangle length: " << length_
    << ", rectangle width: " << width_ << '\n';
  std::cout << "area: " << area() << '\n';
  std::cout << "perimeter: " << perimeter() << "\n\n";
}
