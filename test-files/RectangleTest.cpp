//File name: /Users/laptopuser/Documents/courses_macbook_pro/cs235_f26/class_day_f26/day1_classOverView/code/RectangleTest.cpp
#include <iostream>
#include "Rectangle.hpp" 

int main() {
  // Instantiate Rectangle object r1 using the default constructor.
  Rectangle r1; 
  r1.print();

  // Instantiate rectangle r2 with length 3 and width 1.
  Rectangle r2(3, 1);
  r2.print();

  // Attempt to change the length of r2 to -1. Then change its width to 2.
  r2.setLength(-1); // invalid operation
  r2.setWidth(2);
  r2.print();

  Rectangle r3(r2);  // direct-initialization
  Rectangle r4 = r3; // copy-initialization

  Rectangle r5;      // default constructor
  r5 = r3;           // overloaded assignment operator
 
  return 0;
}
